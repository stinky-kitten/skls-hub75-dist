#!/usr/bin/env python3
"""Verify a qualified producer payload and prepare isolated release review files. Never publish."""
import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import subprocess
import zipfile

ROOT = Path(__file__).resolve().parents[1]
REPOSITORY = 'stinky-kitten/skls-hub75-dist'
PRIVATE_STATUS = 'prepared-not-published-pending-release-approval'
# Required attribution policy, not a second payload allowlist. The validated
# producer manifest defines the full inventory, including all public headers.
NOTICES = {
    'LICENSES/NOTICE.md', 'LICENSES/PICO-SDK-BSD-3-Clause.txt',
    'LICENSES/PICO-EXAMPLES-BSD-3-Clause.txt', 'LICENSES/PIO-PROVENANCE.md',
    'LICENSES/SKLS-HUB75-BINARY-LICENCE-1.0.txt',
}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def load_json(data):
    def unique(pairs):
        result = {}
        for key, value in pairs:
            if key in result:
                raise ValueError('Duplicate JSON key: ' + key)
            result[key] = value
        return result
    return json.loads(data, object_pairs_hook=unique)


def numeric_version(version):
    match = re.fullmatch(
        r'(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)'
        r'(?:-([0-9A-Za-z-]+(?:\.[0-9A-Za-z-]+)*))?'
        r'(?:\+([0-9A-Za-z-]+(?:\.[0-9A-Za-z-]+)*))?', version)
    if not match or (match[4] and any(p.isdigit() and len(p) > 1 and p[0] == '0'
                                                  for p in match[4].split('.'))):
        raise ValueError('Supply an explicit SemVer without a v prefix')
    return '.'.join(match.group(1, 2, 3))


def identity(version):
    numeric_version(version)
    name = 'skls-hub75-' + version + '-rp2350-arm-s'
    asset = name + '.zip'
    tag = 'v' + version
    return name, asset, tag, f'https://github.com/{REPOSITORY}/releases/download/{tag}/{asset}'


def safe_path(name):
    path = PurePosixPath(name)
    if (not name or '\\' in name or path.is_absolute() or '..' in path.parts or
            str(path) != name or '\x00' in name):
        raise ValueError('Unsafe payload path: ' + name)
    return path


def read_candidate(candidate):
    if candidate.is_symlink() or not candidate.is_dir():
        raise ValueError('Candidate must be a regular directory')
    files = {}
    for path in candidate.rglob('*'):
        if path.is_symlink() or not (path.is_dir() or path.is_file()):
            raise ValueError('Candidate contains a symlink or special file')
        if path.is_file():
            name = path.relative_to(candidate).as_posix()
            safe_path(name)
            files[name] = path.read_bytes()
    return files


def archive_members(data):
    """Read GNU ar names without assuming a host ar understands Arm archives."""
    if not data.startswith(b'!<arch>\n'):
        raise ValueError('Expected a regular driver archive')
    offset, names, table = 8, [], b''
    while offset < len(data):
        header = data[offset:offset + 60]
        if len(header) != 60 or header[58:] != b'`\n':
            raise ValueError('Malformed driver archive')
        size = int(header[48:58])
        end = offset + 60 + size
        if size < 0 or end + size % 2 > len(data):
            raise ValueError('Malformed driver archive size')
        name = header[:16].decode('ascii').strip()
        contents = data[offset + 60:end]
        if name == '//':
            table = contents
        elif name not in ('/', '/SYM64/'):
            if name.startswith('/'):
                start = int(name[1:])
                stop = table.find(b'/\n', start)
                if start < 0 or stop < start:
                    raise ValueError('Invalid GNU archive name')
                name = table[start:stop].decode('ascii')
            else:
                name = name.rstrip('/')
            safe_path(name)
            names.append(name)
        offset = end + size % 2
    return names


def checked_payload(files, version, report):
    numeric = numeric_version(version)
    manifest = load_json(files['manifest.json'])
    expected = manifest['files']
    if not isinstance(expected, dict) or set(files) != set(expected) | {'manifest.json'}:
        raise ValueError('Manifest file inventory mismatch')
    if not NOTICES.issubset(expected):
        raise ValueError('Missing required licence/provenance file')
    for name, checksum in expected.items():
        safe_path(name)
        if digest(files[name]) != checksum:
            raise ValueError('Payload checksum mismatch: ' + name)
    for name, data in files.items():
        if re.search(rb'(?:/Users/|/home/|/private/|/tmp/|[A-Za-z]:\\Users\\)', data):
            raise ValueError('Private absolute path in ' + name)
    if manifest['version'] != version or manifest['cmake_version'] != numeric:
        raise ValueError('Candidate version does not match requested release version')
    if (manifest['package'] != 'skls-hub75' or manifest['target'] != 'rp2350-arm-s' or
            manifest['abi_version'] != 1 or
            manifest['abi_id'] != 'rp2350-arm-s-aapcs-softfp-short-enums-v1-profile00'):
        raise ValueError('Unexpected candidate package/target/ABI')
    if (manifest['publication_status'] != PRIVATE_STATUS or
            manifest['legal_status'] != 'approved-binary-and-interface-licence-v1.0' or
            manifest['binary_licence'] != 'LICENSES/SKLS-HUB75-BINARY-LICENCE-1.0.txt' or
            manifest['original_rights_holder'] != 'Stinky Kitten Limited'):
        raise ValueError('Candidate must retain private status and approved licensing metadata')
    archive = files['lib/libskls_hub75.a']
    audit = manifest['archive']
    if (digest(archive) != audit['sha256'] or len(archive) != audit['size_bytes'] or
            archive_members(archive) != audit['members']):
        raise ValueError('Archive identity/membership does not match producer metadata')
    if manifest['build_id'] != version + '-' + digest(archive)[:16]:
        raise ValueError('Candidate build ID mismatch')
    for file, variable, value in (
        ('SKLSHUB75Config.cmake', 'SKLSHUB75_PACKAGE_VERSION', version),
        ('SKLSHUB75Config.cmake', 'SKLSHUB75_CMAKE_VERSION', numeric),
        ('SKLSHUB75Config.cmake', 'SKLSHUB75_PUBLICATION_STATUS', PRIVATE_STATUS),
        ('SKLSHUB75ConfigVersion.cmake', 'PACKAGE_VERSION', numeric),
    ):
        matches = re.findall(r'^set\(' + variable + r' "([^"\n]*)"\)\s*$',
                             files['cmake/' + file].decode(), re.MULTILINE)
        if matches != [value]:
            raise ValueError('CMake identity mismatch: ' + variable)
    prefix = (f'Private candidate version: `{version}`\n\nCMake numeric version: `{numeric}`\n\n'
              f'Publication status: `{PRIVATE_STATUS}`\n\n').encode()
    if not files['README.md'].startswith(prefix):
        raise ValueError('Candidate README identity mismatch')
    # The external record binds the producer's comprehensive allowlist/ABI tests
    # to these exact bytes. It is a trusted local input, not a signed attestation.
    if (report.get('passed') is not True or
            report.get('manifest_sha256') != digest(files['manifest.json']) or
            report.get('payload_file_count') != len(files) or
            report.get('package_version') != version or report.get('cmake_version') != numeric or
            report.get('package_build_id') != manifest['build_id'] or
            report.get('publication_status') != PRIVATE_STATUS or
            not report.get('checks') or any(c.get('passed') is not True for c in report['checks'])):
        raise ValueError('Validation record does not qualify this exact candidate manifest')
    for board in ('pico2', 'pico2_w'):
        result = report.get('boards', {}).get(board, {})
        if (result.get('linked') is not True or result.get('c_and_cpp') is not True or
                result.get('archive_sha256') != digest(archive)):
            raise ValueError('Validation record lacks matching board qualification: ' + board)
    return manifest


def git_output(arguments):
    result = subprocess.run(['git', *arguments], cwd=ROOT, capture_output=True, text=True, timeout=45)
    if result.returncode:
        raise ValueError('Cannot establish existing release tags; no preparation performed: ' + result.stderr.strip())
    return result.stdout


def reject_used_version(version, rehearsal):
    # A preparation-era status field is never proof that a version is unused.
    recorded = load_json((ROOT / 'release.json').read_bytes())['version']
    tags = git_output(['for-each-ref', '--format=%(refname:strip=2)', 'refs/tags']).splitlines()
    if not rehearsal:
        remote = git_output(['ls-remote', '--tags', 'origin'])
        tags += [line.split('refs/tags/', 1)[1].removesuffix('^{}')
                 for line in remote.splitlines() if 'refs/tags/' in line]
    versions = {recorded.split('+', 1)[0]} | {t[1:].split('+', 1)[0] for t in tags if t.startswith('v')}
    if version.split('+', 1)[0] in versions:
        raise ValueError('Refusing to reuse an existing recorded/tagged release version')


def swiftpm_checks(snapshot, asset, swift, scratch):
    environment = dict(os.environ, CLANG_MODULE_CACHE_PATH=str(scratch / 'clang-cache'),
                       SWIFT_MODULECACHE_PATH=str(scratch / 'swift-cache'))
    def run(arguments):
        result = subprocess.run([swift, *arguments], capture_output=True, text=True,
                                env=environment, timeout=120)
        if result.returncode:
            raise ValueError('SwiftPM verification failed: ' + result.stderr)
        return result.stdout
    package = load_json(run(['package', '--package-path', str(snapshot), '--scratch-path', str(scratch / 'build'),
                             '--cache-path', str(scratch / 'cache'), '--manifest-cache', 'none', 'dump-package']))
    targets = package['targets']
    if (len(targets) != 1 or targets[0]['name'] != 'SKLSHUB75' or targets[0]['type'] != 'regular' or
            targets[0]['sources'] != ['swiftpm/Interface.c'] or targets[0]['publicHeadersPath'] != 'include' or
            targets[0].get('dependencies') or targets[0].get('settings')):
        raise ValueError('SwiftPM must remain the declaration-only SKLSHUB75 target, with no Arm binary/autolink')
    if run(['package', 'compute-checksum', str(asset)]).strip() != digest(asset.read_bytes()):
        raise ValueError('SwiftPM archive checksum mismatch')


def snapshot_files(files):
    result = {name: data for name, data in files.items()
              if name == 'README.md' or name.startswith(('include/', 'docs/', 'LICENSES/'))}
    for name in ('Package.swift', 'swiftpm/Interface.c', 'tools/prepare_release.py', 'tools/tests/test_prepare_release.py', 'docs/FINALIZATION.md'):
        result[name] = (ROOT / name).read_bytes()
    return result


def review_notes(version, record):
    return (f'# Private release preparation {version}\n\nIntended tag: `{record["tag"]}`. '
            f'Not published; explicit approval and public verification are still required.\n\n'
            f'Firmware asset: `{record["asset"]}`. SHA-256: `{record["asset_sha256"]}`.\n\n'
            'SwiftPM supplies declarations only; the firmware ZIP supplies the Arm library.\n').encode()


def release_record(files, manifest, version, asset, rehearsal, snapshot):
    _, filename, tag, url = identity(version)
    return {
        'version': version, 'cmake_version': manifest['cmake_version'], 'abi_version': 1,
        'tag': tag, 'repository': REPOSITORY, 'asset': filename, 'asset_url': url,
        'asset_sha256': digest(asset.read_bytes()), 'asset_bytes': asset.stat().st_size,
        'manifest_sha256': digest(files['manifest.json']), 'component_sha256': manifest['files'],
        'publication_status': PRIVATE_STATUS, 'legal_status': manifest['legal_status'],
        'rehearsal': rehearsal, 'remote_tags_checked': not rehearsal,
        'swiftpm': {'kind': 'declaration-only', 'manifest_sha256': digest(snapshot['Package.swift']),
                    'anchor_sha256': digest(snapshot['swiftpm/Interface.c']),
                    'interface_sha256': {n: digest(d) for n, d in files.items() if n.startswith('include/')},
                    'firmware_asset': filename, 'firmware_sha256': digest(asset.read_bytes())},
    }


def check(asset, version, report, swift):
    name, filename, _, _ = identity(version)
    if asset.name != filename:
        raise ValueError('Release asset filename does not match version')
    files = {}
    with zipfile.ZipFile(asset) as archive:
        for entry in archive.infolist():
            path = safe_path(entry.filename)
            if len(path.parts) < 2 or path.parts[0] != name or entry.is_dir():
                raise ValueError('Unexpected ZIP root/directory')
            if (entry.external_attr >> 16) & 0o170000 != 0o100000:
                raise ValueError('ZIP entries must be regular files')
            relative = PurePosixPath(*path.parts[1:]).as_posix()
            if relative in files:
                raise ValueError('Duplicate archive member')
            files[relative] = archive.read(entry)
    manifest = checked_payload(files, version, report)
    output = asset.parent
    record = load_json((output / 'release.json').read_bytes())
    if type(record.get('rehearsal')) is not bool:
        raise ValueError('Missing explicit rehearsal status')
    snapshot = snapshot_files(files)
    expected = release_record(files, manifest, version, asset, record['rehearsal'], snapshot)
    if record != expected:
        raise ValueError('Release metadata/asset checksum mismatch')
    if (output / 'SHA256SUMS').read_text() != expected['asset_sha256'] + '  ' + filename + '\n':
        raise ValueError('SHA256SUMS mismatch')
    for relative, data in snapshot.items():
        if (output / 'repository' / relative).is_symlink() or (output / 'repository' / relative).read_bytes() != data:
            raise ValueError('Prepared repository differs from authoritative inputs: ' + relative)
    if (output / 'repository/release.json').read_bytes() != (output / 'release.json').read_bytes():
        raise ValueError('Prepared repository release record mismatch')
    if (output / 'repository/release-notes.md').read_bytes() != review_notes(version, record):
        raise ValueError('Prepared release notes mismatch')
    swiftpm_checks(output / 'repository', asset, swift, output / 'verification')
    print('PASS: unchanged producer payload, manifest, asset, release metadata and declaration-only SwiftPM package')


def prepare(candidate, output, version, report, swift, rehearsal=False):
    identity(version)
    files = read_candidate(candidate)
    manifest = checked_payload(files, version, report)
    reject_used_version(version, rehearsal)
    candidate, output = candidate.resolve(), output.resolve()
    if output == candidate or output.is_relative_to(candidate) or candidate.is_relative_to(output):
        raise ValueError('Candidate and output directories must be disjoint')
    # Exclusive creation: never overwrite retained assets/records or the checkout.
    output.mkdir(parents=True, exist_ok=False)
    name, filename, tag, _ = identity(version)
    asset = output / filename
    with zipfile.ZipFile(asset, 'x', compression=zipfile.ZIP_DEFLATED, compresslevel=9) as archive:
        for relative, data in sorted(files.items()):
            entry = zipfile.ZipInfo(name + '/' + relative, date_time=(2026, 1, 1, 0, 0, 0))
            entry.create_system = 3
            entry.external_attr = 0o100644 << 16
            entry.compress_type = zipfile.ZIP_DEFLATED
            archive.writestr(entry, data, compresslevel=9)
    snapshot = snapshot_files(files)
    record = release_record(files, manifest, version, asset, rehearsal, snapshot)
    snapshot['release.json'] = (json.dumps(record, indent=2, sort_keys=True) + '\n').encode()
    snapshot['release-notes.md'] = review_notes(version, record)
    for relative, data in snapshot.items():
        path = output / 'repository' / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)
    (output / 'release.json').write_bytes(snapshot['release.json'])
    (output / 'SHA256SUMS').write_text(record['asset_sha256'] + '  ' + filename + '\n')
    check(asset, version, report, swift)
    print('Prepared isolated review files:', output)
    if rehearsal:
        print('REHEARSAL ONLY: remote tags were not checked; this output is not ready for publication.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--version', required=True)
    parser.add_argument('--validation-record', type=Path, required=True)
    parser.add_argument('--candidate', type=Path)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--check', type=Path)
    parser.add_argument('--swift', default='swift')
    parser.add_argument('--rehearsal', action='store_true', help='Skip remote tag lookup and mark output rehearsal-only')
    args = parser.parse_args()
    try:
        report = load_json(args.validation_record.read_bytes())
        if args.check and not (args.candidate or args.output or args.rehearsal):
            check(args.check, args.version, report, args.swift)
        elif args.candidate and args.output and not args.check:
            prepare(args.candidate, args.output, args.version, report, args.swift, args.rehearsal)
        else:
            parser.error('Use --candidate and --output, or --check separately')
    except (OSError, ValueError, KeyError, TypeError, AttributeError, subprocess.TimeoutExpired, zipfile.BadZipFile) as error:
        raise SystemExit(str(error))
