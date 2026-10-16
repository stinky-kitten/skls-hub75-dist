#!/usr/bin/env python3
"""Finalize an audited Stage 1 candidate; never compile, approve, commit or publish."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import zipfile

ROOT = Path(__file__).resolve().parents[1]
VERSION = "0.1.0-experimental.1"
NAME = "skls-hub75-" + VERSION + "-rp2350-arm-s"
ASSET = NAME + ".zip"
BASE = {
    "include/module.modulemap", "include/SKLSHUB75.apinotes",
    *("include/skls_hub75/" + x + ".h" for x in ["skls_hub75", "skls_hub75_config", "skls_hub75_config_validation", "skls_hub75_driver", "skls_hub75_presets", "skls_hub75_profile", "skls_hub75_render_request", "skls_hub75_binary_config"]),
    "lib/libskls_hub75.a", "cmake/SKLSHUB75Config.cmake", "cmake/SKLSHUB75ConfigVersion.cmake",
    "README.md", "docs/ABI.md", "docs/SWIFT.md", "docs/RELEASE.md",
    "LICENSES/NOTICE.md", "LICENSES/PICO-SDK-BSD-3-Clause.txt",
    "LICENSES/SKLS-HUB75-BINARY-LICENCE-1.0.txt",
}
LICENCE = "LICENSES/SKLS-HUB75-BINARY-LICENCE-1.0.txt"
OVERLAYS = ["README.md", "docs/RELEASE.md", "LICENSES/PIO-PROVENANCE.md", "LICENSES/PICO-EXAMPLES-BSD-3-Clause.txt"]
SYNC = [x for x in BASE if x.startswith("include/")] + ["docs/ABI.md", "docs/SWIFT.md", "LICENSES/NOTICE.md", "LICENSES/PICO-SDK-BSD-3-Clause.txt", LICENCE]

def digest(data):
    return hashlib.sha256(data).hexdigest()

def checked_manifest(files):
    manifest = json.loads(files["manifest.json"])
    if manifest["version"] != VERSION or manifest["abi_version"] != 1:
        raise ValueError("Unexpected package version or ABI")
    if manifest.get("legal_status") != "approved-binary-and-interface-licence-v1.0" or manifest.get("binary_licence") != LICENCE:
        raise ValueError("Candidate must include the approved binary/interface licence")
    if set(files) != set(manifest["files"]) | {"manifest.json"}:
        raise ValueError("Manifest file inventory mismatch")
    for name, expected in manifest["files"].items():
        if digest(files[name]) != expected:
            raise ValueError("Payload checksum mismatch: " + name)
    if digest(files["lib/libskls_hub75.a"]) != manifest["archive"]["sha256"]:
        raise ValueError("Archive checksum does not match producer metadata")
    return manifest

def audit_paths(files):
    for name, data in files.items():
        if re.search(rb"(?:/Users/|/home/|/private/|/tmp/|[A-Za-z]:\\Users\\)", data):
            raise ValueError("Private absolute path in " + name)

def check(asset):
    with zipfile.ZipFile(asset) as archive:
        files = {}
        for item in archive.infolist():
            path = Path(item.filename)
            if path.is_absolute() or ".." in path.parts or "\\" in item.filename or path.parts[0] != NAME:
                raise ValueError("Unsafe archive member")
            if (item.external_attr >> 16) & 0o170000 == 0o120000:
                raise ValueError("Symlinks are not release payloads")
            name = str(Path(*path.parts[1:]))
            if name in files or item.is_dir():
                raise ValueError("Duplicate or unexpected archive member")
            files[name] = archive.read(item)
    if set(files) != BASE | set(OVERLAYS) | {"manifest.json"}:
        raise ValueError("Final payload allowlist mismatch")
    manifest = checked_manifest(files)
    audit_paths(files)
    release = json.loads((ROOT / "release.json").read_text())
    for name in SYNC + OVERLAYS:
        if (ROOT / name).read_bytes() != files[name]:
            raise ValueError("Public snapshot differs from asset: " + name)
    if digest(Path(asset).read_bytes()) != release["asset_sha256"]:
        raise ValueError("Release asset checksum mismatch")
    if release["component_sha256"] != manifest["files"] or release["manifest_sha256"] != digest(files["manifest.json"]):
        raise ValueError("Release component metadata mismatch")
    print("Verified immutable asset, manifest, interfaces, approved licence and third-party notices")

def prepare(candidate, output):
    candidate = candidate.resolve()
    paths = list(candidate.rglob("*"))
    if any(p.is_symlink() for p in paths):
        raise ValueError("Candidate contains symlinks")
    files = {str(p.relative_to(candidate)): p.read_bytes() for p in paths if p.is_file()}
    if set(files) != BASE | {"manifest.json"}:
        raise ValueError("Supply only the Stage 1 allowlisted candidate, no private records")
    manifest = checked_manifest(files)
    audit_paths(files)
    if manifest["archive"]["size_bytes"] != 37228 or len(manifest["archive"]["members"]) != 12:
        raise ValueError("Unexpected initial archive: requalify a changed driver")
    output.mkdir(parents=True, exist_ok=True)
    asset = output / ASSET
    if asset.exists() or (output / "SHA256SUMS").exists():
        raise ValueError("Refusing to overwrite retained release output; choose a new candidate directory")
    # Public interfaces and canonical licence/notices remain producer outputs.
    for name in OVERLAYS:
        files[name] = (ROOT / name).read_bytes()
    manifest["publication_status"] = "prepared-not-published-pending-release-approval"
    manifest["legal_status"] = "approved-binary-and-interface-licence-v1.0"
    manifest["original_rights_holder"] = "Stinky Kitten Limited"
    manifest["company_number"] = "07306644"
    manifest["binary_licence"] = LICENCE
    manifest["files"] = {name: digest(data) for name, data in sorted(files.items()) if name != "manifest.json"}
    files["manifest.json"] = (json.dumps(manifest, indent=2, sort_keys=True) + "\n").encode()
    audit_paths(files)
    for name in SYNC:
        p = ROOT / name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(files[name])
    with zipfile.ZipFile(asset, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=9) as archive:
        for name, data in sorted(files.items()):
            info = zipfile.ZipInfo(NAME + "/" + name, date_time=(2026, 1, 1, 0, 0, 0))
            info.create_system = 3
            info.external_attr = 0o100644 << 16
            info.compress_type = zipfile.ZIP_DEFLATED
            archive.writestr(info, data, compresslevel=9)
    checksum = digest(asset.read_bytes())
    (output / "SHA256SUMS").write_text(checksum + "  " + ASSET + "\n")
    release = {
        "version": VERSION, "abi_version": 1, "tag": "v" + VERSION,
        "repository": "stinky-kitten/skls-hub75-dist", "asset": ASSET,
        "asset_url": "https://github.com/stinky-kitten/skls-hub75-dist/releases/download/v" + VERSION + "/" + ASSET,
        "asset_sha256": checksum, "asset_bytes": asset.stat().st_size,
        "manifest_sha256": digest(files["manifest.json"]), "component_sha256": manifest["files"],
        "publication_status": "prepared-not-published", "legal_status": "approved-binary-and-interface-licence-v1.0",
    }
    (ROOT / "release.json").write_text(json.dumps(release, indent=2, sort_keys=True) + "\n")
    check(asset)
    print("Retain exactly:", asset)
    print("SHA256:", checksum)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--candidate", type=Path)
    parser.add_argument("--output", type=Path)
    parser.add_argument("--check", type=Path)
    args = parser.parse_args()
    try:
        if args.check:
            if args.candidate or args.output:
                parser.error("Use --check separately from preparation inputs")
            check(args.check)
        elif args.candidate and args.output:
            prepare(args.candidate, args.output)
        else:
            parser.error("Use --candidate and --output, or --check")
    except (OSError, ValueError, KeyError, zipfile.BadZipFile) as error:
        raise SystemExit(str(error))
