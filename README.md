# SKLSHUB75 binary distribution

**Published release history:** v0.1.0-experimental.1 is already published and
immutable. The API examples below describe that release's earlier annotations.
The private producer's newer `HUB75` interface is not yet in a subsequent release.
For current release tooling, read [FINALIZATION.md](docs/FINALIZATION.md).
Preparation writes an isolated review snapshot; it never updates published assets
or the working checkout. The root `release.json` and original release notes remain
historical preparation records, not live publication-status records.

Experimental **0.1.0-experimental.1**, ABI **1**. Precompiled C HUB75 scanout for
Raspberry Pi Pico 2 and Pico 2 W, qualified with Pico SDK 2.3.1 and 150 MHz clk_sys.
Embedded Swift is a supported consumer through the **SKLSHUB75 Clang module** and
versioned **SKLSHUB75.apinotes**. C and C++ use the same public C interface.

**Published experimental release.** The approved
[Stinky Kitten HUB75 Binary Library Licence — Version 1.0](LICENSES/SKLS-HUB75-BINARY-LICENCE-1.0.txt)
covers Stinky Kitten Limited's original contributions (company number 07306644).
It permits worldwide, royalty-free personal, educational, research and commercial
use, linking into proprietary/open-source firmware, and distribution/sale of
completed firmware and updates, including compliant redistribution received from
others. No registration, royalties or application source disclosure is required.
Public interface and integration-utility use, copying, modification and
redistribution are permitted with applicable notices, including SwiftPM manifests
and Python tooling. This does not grant modification rights to the compiled driver.
Standalone proprietary library redistribution and incorporation into publicly
distributed SDKs or reusable binary components require separate permission.
These restrictions do not apply to completed firmware or independently BSD-licensed
third-party contributions. See [NOTICE.md](LICENSES/NOTICE.md) and the full licence.

The official repository is https://github.com/stinky-kitten/skls-hub75-dist .
The immutable tag is `v0.1.0-experimental.1`, with one complete
`skls-hub75-0.1.0-experimental.1-rp2350-arm-s.zip` asset and SHA256SUMS.
The first release was published and its download/checksum/SwiftPM resolution were
verified during publication. Every subsequent release needs its own qualification,
approval and public verification; existing tags and assets must never be replaced.

## Firmware consumption

Download the complete versioned release archive and verify the published SHA-256,
then extract it. It contains matching `include/`, `lib/`, `cmake/`, documentation,
licensing and `manifest.json`; no second download is needed for firmware interfaces.
The private implementation source, internal headers and SDK are not distributed.
After `pico_sdk_init()`, use:

```cmake
find_package(SKLSHUB75 0.1.0 EXACT CONFIG REQUIRED
    PATHS "${SKLSHUB75_ROOT}/cmake" NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
target_link_libraries(my_firmware PRIVATE SKLSHUB75::hub75)
```

The application supplies its Pico SDK, startup/linker configuration, runtime and
Core 1 launch. [ABI.md](docs/ABI.md) defines public structure layouts, enum storage,
SDK dependencies, resource ownership, supported configurations and operating limits.
Core 1 and PIO0 SM0/SM1 are dedicated to the worker; the driver allocates 21 PIO0
instruction slots and dynamically claims one DMA channel. Default GPIO ownership is
RGB16–21, row6–10, clock11, LAT12, OE13. Applications own the remaining resources and
must avoid conflicts. Do not change the qualified 150 MHz system clock while operating.

## Embedded Swift

```swift
import SKLSHUB75
var request = RenderRequest()
if takeRenderRequest(&request) {
    // Unwrap its borrowed framebuffer and stop writing before deadline_us.
}
let transfer: TransferMode = .gamma
```

See [SWIFT.md](docs/SWIFT.md) for exact compiler/importer flags and limitations.
API notes provide Swift naming, selected enum naming and nullability. They do not
provide ownership, bounds checking or lifetime enforcement. Requests do not transfer
ownership or require an acknowledgement to Core 1.

For **macOS host declaration tests only**, this repository exposes a declaration-only
SwiftPM product `SKLSHUB75`. Depend on the exact `0.1.0-experimental.1` version for that published release. It includes the same generated headers, module map and notes
as the complete firmware asset, with no Arm archive and no autolink. It cannot supply
hardware implementation functions to a host executable. See the release process for
byte-for-byte interface consistency checks. The new finalizer stages producer-generated interfaces for review without changing
this historical checkout snapshot.

## Qualification and release preparation

Initial policy is six bitplanes/four dither phases; driver profiling is fixed 0/0.
Compilation/link tests cover both boards. Debug radio-clock builds currently encounter
an existing Embedded Swift `posix_memalign` linker issue. On-device timing/scanout has
not been qualified by these tests. This is an experimental API, not a stable 1.0 API.
Feature version and ABI version are separate: compatible features can change the
experimental version; layout/enum/calling-convention changes require ABI review.

[RELEASE.md](docs/RELEASE.md) links the current release preparation, authoritative inputs,
immutable assets and the approval/publication boundary. `release.json` records exact
asset and component hashes. Do not commit generated Arm libraries into this repository.

Existing interface notice text is preserved verbatim. Stinky Kitten Limited owns
its original driver contributions; Raspberry Pi retains ownership of its BSD-licensed
contributions. [PIO provenance/modifications](LICENSES/PIO-PROVENANCE.md) and complete
Raspberry Pi BSD notices accompany the binary. The custom licence does not override
independently granted BSD rights. No proprietary driver source is distributed.
