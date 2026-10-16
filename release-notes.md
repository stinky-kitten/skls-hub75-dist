# SKLSHUB75 0.1.0-experimental.1 — experimental ABI 1

**Prepared, not published.** Proposed tag: `v0.1.0-experimental.1`.

One complete binary asset supports Pico 2 and Pico 2 W on secure RP2350 Arm,
qualified with Pico SDK 2.3.1, Cortex-M33/base AAPCS/short enums and 150 MHz clk_sys.
It contains the driver-only static library, matching public headers, relocatable
CMake configuration, SKLSHUB75 Clang module, Swift API notes, ABI metadata, resource
contract and licensing/notices. The declaration-only SwiftPM product uses identical
versioned interfaces without linking the Arm archive on macOS.

Known limitations: experimental public API; six-bit/four-phase implementation only;
driver profiling fixed 0/0; no arbitrary target/SDK qualification; no on-device
scanout/timing claim. The radio-clock Debug `posix_memalign` toolchain blocker remains.
Dedicated Core 1 and PIO0 SM0/SM1, 21 PIO slots, a dynamically claimed DMA channel
and conflict-free GPIO mapping are required. See docs/ABI.md.

Asset and component checksums are recorded in release.json and SHA256SUMS.
The approved Stinky Kitten HUB75 Binary Library Licence — Version 1.0 is included
in LICENSES/SKLS-HUB75-BINARY-LICENCE-1.0.txt. Stinky Kitten Limited
(07306644) owns its original contributions. Completed firmware distribution/sale,
updates, compliant received-firmware redistribution and public interface/tooling
use, copying, modification and redistribution are permitted as specified in that
licence. The modification grant excludes the compiled driver. Standalone proprietary library and
reusable SDK/package redistribution require separate permission. Raspberry Pi's
BSD rights and notices remain independent.

The new licensed candidate supersedes the earlier draft for future release review;
earlier retained bytes remain unchanged. The validation consumer pins the actual
final archive checksum. Public download, checksum and exact-version SwiftPM tag
checks are performed as part of publishing the approved release.
