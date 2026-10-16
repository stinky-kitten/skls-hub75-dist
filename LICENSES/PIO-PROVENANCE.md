# Raspberry Pi HUB75 PIO provenance and modifications

The incorporated PIO implementation is derived from Raspberry Pi's official
`pico-examples/pio/hub75/hub75.pio`. The original source notice is preserved:

```text
Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
SPDX-License-Identifier: BSD-3-Clause
```

Upstream comparison reference (not a claim of the original import revision):
https://github.com/raspberrypi/pico-examples/blob/7fe60d6b4027771e45d97f207532c41b1d8c5418/pio/hub75/hub75.pio

Upstream file SHA-256: `868fb6dfea914f486e2df538dd0d1dc7f2f8f0a3b12359850b4fc2d3d3ede399`.
The private producer's actual derived implementation was compared against this
reference. Its original Raspberry Pi copyright and SPDX identifier remain intact.
No private implementation source or generated PIO header is distributed.

## Identified changes

- The row program inserts two NOP instructions with `[7]` delay, one before LAT
  and one after LAT, while keeping OE disabled. These extend latch blanking for
  timing/ghosting control. The row program consequently uses five instructions,
  compared with the upstream three. This describes the code; it does not claim
  new on-device timing qualification during release preparation.
- Comments clarify the 27-bit pulse-loop count and the blanking/LAT/OE sequence.
- The embedded C shift-patching helper explicitly casts instruction encodings to
  `uint16_t` and marks its unused state-machine parameter with `(void)sm`.
- The PIO initialisation, TX-stall and instruction-patching embedded C helpers are
  also derived from the example; their existing notices remain applicable.
- The RGB888 data program's instruction sequence is unchanged from that reference
  and uses 16 instruction slots. Together with the modified row program, the
  driver allocates 21 PIO0 instruction slots.

The derived PIO instructions and incorporated helper portions retain their
applicable BSD-3-Clause licensing and Raspberry Pi attribution. They are distinct
from the original proprietary driver implementation and public-interface grants.
The approved binary licence does not replace or override Raspberry Pi's
BSD rights in its contribution. Combining that contribution with proprietary
original code does not make Raspberry Pi's code exclusively proprietary or make
the entire original driver open source.

## Complete applicable licence

`PICO-EXAMPLES-BSD-3-Clause.txt` reproduces the complete upstream licence,
including copyright, all three conditions and the disclaimer, obtained at the
same comparison revision:
https://github.com/raspberrypi/pico-examples/blob/7fe60d6b4027771e45d97f207532c41b1d8c5418/LICENSE.TXT

Licence text SHA-256: `483f865953435b66c443dee7558debe3cc3cf8fcbb6a112fd9fc6a795d53f1f6`.
It is byte-identical to the separately attached Pico SDK 2.3.1 licence text.
Keep both applicable attributions/notices with redistributed finished firmware.
Third-party rights remain governed by those terms, including the non-endorsement
condition. No proprietary terms here withdraw those existing permissions.
