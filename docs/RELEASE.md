# Experimental release preparation

Version `0.1.0-experimental.1`; ABI 1; intended tag `v0.1.0-experimental.1`.

The existing private Stage 1 CMake packaging target is authoritative for the Arm
archive, seven API headers, binary configuration header, module map, API notes,
CMake integration and ABI metadata. Do not compile a second archive here. Build a
clean committed producer using its documented `skls_hub75_package` target and run
its binary validator. Keep the producer build record private and outside release assets.

This repository owns the public README, release procedure and publication checklist.
Unapproved original-work terms and proposed owner/company agreements remain outside
Git. Supply draft terms explicitly from a private external file when preparing a
local review candidate; the tool does not approve them or make them operative. ABI/SWIFT documents and the SwiftPM `include/` snapshot are
synchronized outputs from the final package, never manually edited. Firmware CMake
configuration is included only in the complete asset. The SwiftPM manifest and
release-finalization tool are maintained here; no driver implementation is present.

Prepare from an already audited producer candidate:

```sh
python3 tools/prepare_release.py --candidate "$CANDIDATE" --output "$OUTPUT_DIR" \
    --terms "$PRIVATE_TERMS_FILE"
python3 tools/prepare_release.py --check "$OUTPUT_DIR/skls-hub75-0.1.0-experimental.1-rp2350-arm-s.zip"
```

The finalizer verifies every Stage 1 payload hash, copies the allowlisted candidate,
preserves all existing public-header notices verbatim, without treating historical
company wording as conclusive evidence of assignment, and overlays this repository's
public README, release guide and third-party notices, plus the explicitly supplied
external draft terms. All executable objects, public declarations, API notes, module
map and CMake files are preserved. It refreshes payload checksums and synchronizes
`include/`, `docs/ABI.md`, `docs/SWIFT.md` and notices from those same final bytes.
It does not grant or approve permissions. The third-party PIO/SDK notices remain intact.

The output is a deterministic ZIP with one versioned root, SHA256SUMS and complete
manifest/component hashes. The script refuses to overwrite an existing asset.
`--check` verifies the ZIP, manifest hashes, public snapshot and `release.json` agree.
The original-work draft is checked against its pinned manifest/component hash; it
is not read from Git. The external draft remains an unapproved review input.
Repository preparation documentation can advance while an older, immutable draft
candidate remains retained. To audit that existing candidate explicitly, use:

```sh
python3 tools/prepare_release.py --check "$RETAINED_DRAFT_ZIP" \
    --allow-preparation-doc-drift
```

This exception is limited to README.md and docs/RELEASE.md, only for metadata marked
draft/prepared-not-published. It reports every differing document and still requires
all interface/ABI/Swift/notice snapshots and all pinned asset/component hashes to
match. Strict checks remain the default and are used for newly prepared candidates.
Changed documentation or final legal text requires a new reviewed candidate before
publication; do not change the existing ZIP or the consumer's pin silently.

After final licence approval, update README/release notes, the publication checklist,
this procedure, tool legal-status/terms naming, and release metadata as appropriate.
Prepare the approved complete payload, update the consumer pin in a separately
authorized task, rerun validation and approve those exact bytes before publishing.
The current draft archive name retains PROPOSED-TERMS.md; that draft is not an
operative licence or a Git-tracked file.

Keep that exact archive and its checksum; no regeneration after approval. A change
requires a new candidate review and checksum/pin update. If an asset has been
published, never reuse its version for changed bytes.

No `.a`, full SDK, private build record, internal source/header or `.git` directory
belongs in the Git repository. This repository starts with independent history;
there is no need to rewrite private history to create it.

## Approval and later publication — do not execute during preparation

1. Resolve the blockers in LICENSES, including the owner-to-company commercial licence and final terms, and the
   clock source/font/photo/history review. Finalize and validate any changed payload.
2. Review the technical distribution and radio-clock commits. Local commits require
   authorization and do not approve licences or publication. Keep original-work
   drafts outside Git. The preparation tool never commits or creates tags.
3. Separately authorize creation of `stinky-kitten/skls-hub75-dist` and pushing the
   reviewed independent distribution history. Do not expose private driver history.
4. Tag the reviewed public interfaces `v0.1.0-experimental.1`. Publish an experimental
   prerelease with the retained ZIP and SHA256SUMS. Never upload private records.
5. Verify anonymous ZIP download, checksum, contents and exact SwiftPM version
   resolution from a clean consumer without credentials. Record the real tag revision
   in the clock's Package.resolved and re-run firmware/host/VS Code checks.
6. Only after clock source rights/history review, separately approve its publication.

No remote repository or release is assumed to exist today. Planned official URL:
https://github.com/stinky-kitten/skls-hub75-dist/releases/download/v0.1.0-experimental.1/skls-hub75-0.1.0-experimental.1-rp2350-arm-s.zip

## Third-party review

The actual producer dependency inventory identifies Pico SDK 2.3.1 hardware/platform
headers carrying 2020/2024/2025/2026 Raspberry Pi notices and BSD-3-Clause. Adapted PIO
instructions/helper code derive from Raspberry Pi's HUB75 PIO example, whose 2020
copyright and BSD SPDX identifier remain in the private source. The attached notice
and exact SDK licence text cover those identified incorporated portions. The twelve
archive members contain only driver objects: no SDK runtime/startup, CYW43, TinyUSB,
lwIP, newlib or libgcc objects. Inline SDK helpers are nevertheless incorporated and
must retain their notices in binary documentation. Consumer firmware has additional
notice obligations. See https://github.com/raspberrypi/pico-sdk/tree/2.3.1 and
https://github.com/raspberrypi/pico-examples/tree/master/pio/hub75 .
