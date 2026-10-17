# Release preparation and immutable history

The current workflow is documented in [FINALIZATION.md](FINALIZATION.md).
It accepts the complete producer payload and matching comprehensive validation
record, requires an explicit candidate version, and prepares assets and a future
public snapshot in a fresh review directory. It performs no publication action.

The published **v0.1.0-experimental.1** tag and assets remain immutable. The root
`release.json`, interfaces, notices and original release notes record that release;
its preparation-era JSON status does not mean the version is available for reuse.
The finalizer rejects a version found in that record, local tags or (during normal
preparation) remote tags, independently of any historical status string.

The first release's finalizer overlaid public documentation and two provenance/licence
files and regenerated the manifest after producer validation. That historical process
has been retired. Current producer manifests and every payload file are preserved
unchanged; the external validation record binds qualification to their exact hashes.
The full binary payload includes all five required licence/notice files.

Stinky Kitten Limited (07306644) owns its original driver contributions. The approved
Stinky Kitten HUB75 Binary Library Licence — Version 1.0 remains in
`LICENSES/SKLS-HUB75-BINARY-LICENCE-1.0.txt`. Raspberry Pi contributions retain their
BSD-3-Clause rights, complete notices and conditions. The tooling update does not
change those terms, the C ABI, driver implementation, or the published release.

The SwiftPM product remains declaration-only. The RP2350 library is supplied in the
firmware ZIP and linked through CMake; it is not a SwiftPM Apple binary target.
Follow the current guide for local rehearsal, consumer checks and the separately
authorized publication boundary.
