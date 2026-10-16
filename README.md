# SKLSHUB75 binary distribution

Experimental **0.1.0-experimental.1**, ABI **1**. Precompiled C HUB75 scanout for
Raspberry Pi Pico 2 and Pico 2 W, qualified with Pico SDK 2.3.1 and 150 MHz clk_sys.
Embedded Swift is a supported consumer through the **SKLSHUB75 Clang module** and
versioned **SKLSHUB75.apinotes**. C and C++ use the same public C interface.

**Prepared locally; not published.** No original-work distribution licence has
been approved. Unapproved terms and owner/company agreements are kept outside this
Git repository. The retained local candidate contains draft terms for private
review only; its checksum does not authorize publication. See
[release blockers](LICENSES/RELEASE-BLOCKERS.md). Third-party notices preserve their
existing rights separately and do not grant rights to the original driver.
No public download is live yet.

The intended official repository is https://github.com/stinky-kitten/skls-hub75-dist .
A future experimental GitHub Release `v0.1.0-experimental.1` will provide one complete
`skls-hub75-0.1.0-experimental.1-rp2350-arm-s.zip` asset and SHA256SUMS. This URL is
planned, not a claim of current availability. Original-work permissions, including
binary redistribution and finished-firmware distribution, require final approved
terms. This technical repository does not grant those permissions.

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
SwiftPM product `SKLSHUB75`. Depend on the exact `0.1.0-experimental.1` version after
its tag is published. It includes the same generated headers, module map and notes
as the complete firmware asset, with no Arm archive and no autolink. It cannot supply
hardware implementation functions to a host executable. See the release process for
byte-for-byte interface consistency checks. No manually maintained header copy exists.

## Qualification and release preparation

Initial policy is six bitplanes/four dither phases; driver profiling is fixed 0/0.
Compilation/link tests cover both boards. Debug radio-clock builds currently encounter
an existing Embedded Swift `posix_memalign` linker issue. On-device timing/scanout has
not been qualified by these tests. This is an experimental API, not a stable 1.0 API.
Feature version and ABI version are separate: compatible features can change the
experimental version; layout/enum/calling-convention changes require ABI review.

[RELEASE.md](docs/RELEASE.md) explains release preparation, authoritative inputs,
immutable assets and the approval/publication boundary. `release.json` records exact
asset and component hashes. Do not commit generated Arm libraries into this repository.

Historical interface notices are preserved verbatim; final publication notices await
formal agreement and approval. Intended publisher/distributor: Stinky Kitten Limited,
as proposed commercial licensee;
its formal licensing authority and final public terms remain approval gates.
Third-party ownership and permissions remain as stated in LICENSES.
[PIO provenance/modifications](LICENSES/PIO-PROVENANCE.md) and complete Raspberry Pi
BSD notices accompany the binary; proprietary terms do not override those rights.
