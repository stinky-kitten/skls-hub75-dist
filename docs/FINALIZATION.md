# Preparing a new binary release

The first release, **v0.1.0-experimental.1**, is already published and immutable.
Its tags, assets and historical `release.json` remain unchanged. That JSON records
preparation-era status, not current publication state. The finalizer treats a version
in that record or any release tag as used regardless of that status field.
No next release version is selected by this tooling.

## Authoritative inputs

Use the private producer's complete candidate and the `validation-results.json`
from a successful comprehensive validation of those exact bytes. Current records
bind the manifest SHA-256, selected full/numeric version, build ID and file count,
and record successful C/C++ linkage for both supported boards. Older records without
that binding must be regenerated. The record is a trusted local input, not a signed
attestation; private paths and compiler logs stay outside distributable output.

The producer manifest defines the payload inventory. The finalizer independently
verifies every hash, required licence/provenance presence, archive size/member names,
CMake identities, README identity, build ID and private publication status. It also
checks the manifest against the successful validation record, whose producer-side
allowlist enforces the complete 23-file payload. There is no second full packaging
allowlist and no repair/overlay step. All 23 files, including `manifest.json`, enter
the ZIP unchanged. This does not replace the producer's ABI/toolchain qualification.

The finalizer never compiles a driver, updates the working checkout, creates a tag,
uploads assets, approves publication or contacts a release-creation API.

## Local preparation

Python 3.9+, Git and Swift 6+ are required. Choose a new output directory that does
not already exist. Set `VERSION` to the explicitly approved candidate identifier,
without a `v` prefix; set `CANDIDATE`, `VALIDATION_RECORD` and `NEW_OUTPUT_DIR` to
local paths. The requested version must exactly match the candidate.

```sh
python3 tools/prepare_release.py \
  --version "${VERSION:?Select a candidate version}" \
  --candidate "$CANDIDATE" --validation-record "$VALIDATION_RECORD" \
  --output "$NEW_OUTPUT_DIR" --swift "$SWIFT"
python3 tools/prepare_release.py \
  --version "$VERSION" --validation-record "$VALIDATION_RECORD" \
  --check "$NEW_OUTPUT_DIR/skls-hub75-${VERSION}-rp2350-arm-s.zip" --swift "$SWIFT"
```

Normal preparation checks local tags, the existing release record and remote
`origin` tags. An unavailable remote check fails closed. It never fetches or changes
refs. A non-publishing rehearsal may add `--rehearsal` to preparation: remote lookup
is skipped, local reuse checks remain, and generated metadata explicitly marks the
output rehearsal-only. Rehearsal output cannot establish remote version availability.
Before actual publication, prepare a new reviewed directory without that option.
Check mode verifies retained bytes and records; it does not reauthorize publication
or promise the version is still available remotely.

Output contains the version-derived firmware ZIP, `SHA256SUMS`, a matching
`release.json`, and `repository/` for review as a future public snapshot. The snapshot
uses unchanged candidate headers, API notes, notices and documentation. The finalizer
adds public integration tooling, the SwiftPM manifest/anchor, release metadata and
review notes outside the firmware ZIP. Do not run this snapshot through the old
release overlay process. Existing output directories are never reused or replaced.
No byte-for-byte reproducibility guarantee follows from fixed ZIP timestamps.

## SwiftPM compatibility

`Package.swift` deliberately defines a **declaration-only Clang target**, not a
SwiftPM binary target. It contains matching generated headers/module map/API notes,
and no Arm implementation or autolink. The RP2350 archive remains in the firmware
ZIP and is linked through the packaged CMake target. SwiftPM's documented precompiled
binary-library support is for Apple platforms; a raw RP2350 `.a` is not an
XCFramework. See [SwiftPM's dependency documentation](https://docs.swift.org/swiftpm/documentation/packagemanagerdocs/addingdependencies/).

The finalizer verifies the real SwiftPM target description and compares
`swift package compute-checksum` with the firmware ZIP hash. Generated `release.json`
records that integration type, matching interface hashes, the firmware asset URL and
its checksum. No `.binaryTarget(url:checksum:)` is invented for an unsupported artifact.
Exact public tag resolution still requires a real, separately authorized publication.
A local path dependency can test declaration consumption without creating a tag.

## Publication boundary

After review and explicit approval, publish a new immutable public snapshot/tag and
upload only the reviewed release assets. Recheck remote tag/release availability
immediately before publication; this preparation tool cannot reserve a version or
prevent races in an external publishing process. Never move a tag or replace assets.
Verify anonymous download, checksum and exact-version SwiftPM resolution afterward,
and retain a publication record for the selected version. Update the private
producer's known-published-version record after publication. A generated private
candidate or a preparation-era JSON status is never evidence that publication occurred.
Keep validation records and producer provenance private.

## Regression tests

```sh
HUB75_TEST_CANDIDATE="$CANDIDATE" HUB75_TEST_VALIDATION="$VALIDATION_RECORD" \
  python3 -m unittest discover -s tools/tests -p 'test_prepare_release.py'
```

The candidate-dependent tests consume local release inputs and do not publish,
create tags or modify the public checkout. Unit tests also cover remote-tag rejection
and remote-check failure without network access.
