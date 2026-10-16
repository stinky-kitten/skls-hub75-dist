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
Original-work binary/interface terms are unapproved and are not included in Git.
The retained local archive contains an unapproved legal draft for private review;
its hashes in release.json identify those exact bytes, not approved permissions.
Finalize the terms, prepare a new candidate and update/revalidate its checksum and
consumer pin before publication. No original-work permissions are granted here.
Do not publish this draft candidate before resolving LICENSES/RELEASE-BLOCKERS.md.

Intended publisher: Stinky Kitten Limited as proposed commercial licensee, subject
to an executed, legally reviewed owner-to-company agreement and final approval.
Third-party terms and ownership are preserved separately.
