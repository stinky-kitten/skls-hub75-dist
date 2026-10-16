# Experimental release preparation

Version `0.1.0-experimental.1`; ABI 1; intended tag `v0.1.0-experimental.1`.

The private Stage 1 `skls_hub75_package` target remains authoritative for the Arm
archive, seven API headers, binary configuration header, module map, API notes,
CMake files, ABI metadata and canonical licensing notices. The approved licence
and NOTICE.md originate in `distribution/notices/` and are copied to `LICENSES/`.
Do not compile another archive or manually edit generated interfaces here.

Stinky Kitten Limited (07306644) owns its original driver contributions. Its
approved Stinky Kitten HUB75 Binary Library Licence — Version 1.0 is included as
`LICENSES/SKLS-HUB75-BINARY-LICENCE-1.0.txt`. Raspberry Pi's contributions
retain their BSD-3-Clause rights, complete notices and conditions. No proprietary
source is opened by this licence. Earlier unapproved drafts are obsolete private
review records, not release inputs or operative terms.

Prepare a new candidate from the updated producer:

```sh
python3 tools/prepare_release.py --candidate "$CANDIDATE" --output "$NEW_OUTPUT_DIR"
python3 tools/prepare_release.py --check "$NEW_OUTPUT_DIR/skls-hub75-0.1.0-experimental.1-rp2350-arm-s.zip"
```

The finalizer verifies the producer allowlist, every component hash, licence status
and archive identity. It preserves code, headers, API notes and CMake bytes. It
overlays public release documentation and the existing PIO provenance/BSD notices,
then synchronizes public interfaces, ABI/Swift documents and canonical licences.
Strict check mode compares the retained ZIP, manifest, snapshots and release.json.
No external draft input or documentation-drift exception is used for the licensed
candidate. Outputs are deterministic ZIPs; existing assets/SHA256SUMS are never
overwritten. Exact private build records remain outside distributable assets.

Keep all earlier retained candidates unchanged. New licence/notice bytes require
a newly identified candidate directory, a new checksum and a reviewed consumer-pin
update. Do not silently replace an approved asset or reuse a published version for
different bytes. The current version is still unpublished and experimental.
No .a/ZIP, full SDK, private record, internal source/header or private .git history
belongs in this independent distribution Git repository.

## Publishing the experimental release

Publication requires the owner's approval. Publish the reviewed independent
repository snapshot under `v0.1.0-experimental.1` and upload the retained ZIP and
SHA256SUMS. As part of publishing, verify anonymous public download, the archive
checksum and exact-version SwiftPM tag resolution from a credential-free consumer.
Record the actual resolved revision once the real tag exists. Never move a published
tag or replace published assets; subsequent corrections are separate changes.
Radio-clock open-source publication is a separate release.

Planned URL, not a current download:
https://github.com/stinky-kitten/skls-hub75-dist/releases/download/v0.1.0-experimental.1/skls-hub75-0.1.0-experimental.1-rp2350-arm-s.zip

Both boards share the 12-object archive. Pico SDK helpers and adapted PIO programs/
C helpers retain the inventory in NOTICE.md. The driver archive contains no SDK
startup/runtime, CYW43, TinyUSB, lwIP, newlib or libgcc implementation objects;
complete firmware has additional notice obligations. Hardware scanout/timing and
the existing Debug posix_memalign issue remain separate qualification limits.
