# Release status and qualification

**v0.1.0-experimental.2** publishes the expanded Embedded Swift annotations,
complete 23-file producer payload and ABI **1**. The approved ZIP and SHA256SUMS are
listed in [release notes](../release-notes.md); the tagged `release.json` records
their identity. **v0.1.0-experimental.1 remains unchanged.**

The published ZIP preserves every qualified producer byte. Its manifest, CMake
metadata and bundled documentation retain private preparation status as provenance;
that historical status does not override the public release. The tagged repository's
`release.json` records `published` and preserves the original status separately in
`preparation_publication_status`. Private qualification records and source provenance
are retained outside the public package.

Formal on-device scanout/timing qualification remains outstanding. Successful
compiler, importer and CMake linkage tests do not establish a refresh-rate guarantee.

The private producer requires an explicitly selected SemVer via
`-DSKLS_HUB75_PACKAGE_VERSION=...` (no `v` prefix). No next release version is
preselected. The direct packager's existing `--version` argument uses the same
validation. Empty/malformed identifiers and versions recorded as already published
are rejected. CMake derives the numeric MAJOR.MINOR.PATCH core; it is not a separate
version choice. The full identifier, including prerelease/build metadata, remains
the package identity.

Every generated candidate has publication status
`prepared-not-published-pending-release-approval`, even with a stable-looking version.
The manifest, CMake files, generated README identity and archive-derived build ID
are checked for agreement during staging and comprehensive validation. This status
describes local output, not the existing public release. A future release needs a
new immutable version, final-byte qualification and explicit release approval.
Public download, checksum and consumer tag verification belong to release publishing.
After each publication, update the producer's historical `PUBLISHED_PACKAGE_VERSIONS`
record in `tools/binary_package_common.py`; this is a maintained history,
not a live query of remote releases. The producer does not infer publication from
a Git tag, selected version or successful validation.

The public finalizer accepts the complete producer payload and its matching successful
validation record. That record binds the manifest SHA-256 and file count to the
comprehensive checks. The finalizer preserves every payload byte, verifies the
selected version and checksums, and writes an isolated review snapshot and assets.
It never updates the public checkout or publishes. Normal preparation checks local
and remote tags plus the existing release record; rehearsal mode skips remote
lookup and explicitly marks its output as rehearsal-only. Follow the public
repository's `docs/FINALIZATION.md` for preparation, SwiftPM verification and the
separate publication boundary. Historical first-release assets remain unchanged.

The approved Stinky Kitten HUB75 Binary Library Licence — Version 1.0 is included
in `LICENSES/SKLS-HUB75-BINARY-LICENCE-1.0.txt`. Stinky Kitten Limited owns its
original contributions; Raspberry Pi-derived material retains BSD-3-Clause rights.

The separate public distribution repository must retain independent, clean Git history.
It should contain approved integration files, documentation and immutable versioned
binary assets. Never expose a private clone's history by deleting source files.
Consumers should pin a complete release asset and SHA-256, not a moving branch.
Local development should override an unpacked **binary package** path using the same
imported-target interface, never require private source or credentials.

The manifest records the experimental version, ABI, target/boards, compiler/SDK,
profiling, clocks/capabilities, archive imports/members/checksum and every other
payload file's checksum. Its own checksum belongs to a future enclosing release asset.
The build ID combines the selected version and archive SHA-256 prefix; it identifies
the archive, not every documentation/metadata byte. Manifest hashes cover the payload. Exact private source provenance
is deliberately stored outside the distributable directory in the producer's private
build record. No source checkout paths or private repository URLs belong in a release.

Profiling is fixed 0/0. Any public-layout, enum-value/width, calling-convention or
ownership-contract change requires ABI review and matching headers. Performance
experimentation can continue privately; new compatible builds get new immutable
versions. Preserve the exact assets required by each released consumer commit.
No arbitrary target/SDK compatibility or reproducible byte-identical rebuild claim
is made just because compiler LTO has been disabled.

Before each subsequent public release: retain the approved binary/interface licence,
complete third-party notice review, validate the final stripped candidate with both supported boards,
rerun Embedded Swift positive/negative imports and source-free consumer builds,
and retain the private provenance/validation record. Hardware scanout/timing
qualification is separate from compiler/link validation. The radio-clock migration
and its existing Debug `posix_memalign` runtime issue belong to later consumer work.
