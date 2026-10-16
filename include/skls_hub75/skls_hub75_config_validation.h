//
//  skls_hub75_config_validation.h
//  skls-hub75
//
//  Created by Gary J.H. Atkinson on 2026-09-18.
//  Copyright © 2026 Stinky Kitten Ltd. All rights reserved.
//

#pragma once

#include <skls_hub75/skls_hub75_binary_config.h>

#include <stdbool.h>

#include <skls_hub75/skls_hub75_config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SKLS_HUB75_CONFIG_RESULT_OK,
    SKLS_HUB75_CONFIG_RESULT_INVALID_ARGUMENT,
    SKLS_HUB75_CONFIG_RESULT_INVALID_GEOMETRY,
    SKLS_HUB75_CONFIG_RESULT_UNSUPPORTED_GEOMETRY,
    SKLS_HUB75_CONFIG_RESULT_INVALID_PIN_LAYOUT,
    SKLS_HUB75_CONFIG_RESULT_UNSUPPORTED_PIN_LAYOUT,
    SKLS_HUB75_CONFIG_RESULT_INVALID_BIT_DEPTH,
    SKLS_HUB75_CONFIG_RESULT_UNSUPPORTED_BIT_DEPTH,
    SKLS_HUB75_CONFIG_RESULT_INVALID_DITHER,
    SKLS_HUB75_CONFIG_RESULT_UNSUPPORTED_DITHER,
    SKLS_HUB75_CONFIG_RESULT_INVALID_DISPLAY_POLICY,
    SKLS_HUB75_CONFIG_RESULT_INVALID_TRANSFER_MODE,
    SKLS_HUB75_CONFIG_RESULT_INVALID_GAMMA,
    SKLS_HUB75_CONFIG_RESULT_INVALID_TARGET,
    SKLS_HUB75_CONFIG_RESULT_INVALID_TARGET_POLICY,
    SKLS_HUB75_CONFIG_RESULT_INVALID_TIMING,
    SKLS_HUB75_CONFIG_RESULT_PULSE_ENCODING_OVERFLOW,
    SKLS_HUB75_CONFIG_RESULT_INVALID_RESOLUTION_POLICY,
    SKLS_HUB75_CONFIG_RESULT_UNRESOLVED_AUTO,
    SKLS_HUB75_CONFIG_RESULT_INVALID_SM_RATE,
    SKLS_HUB75_CONFIG_RESULT_UNSUPPORTED_SM_RATE,
    SKLS_HUB75_CONFIG_RESULT_UNSUPPORTED_BLANKING,
} skls_hub75_config_result_t;

// Concrete, off-path output from configuration resolution. This is a compact
// boundary object, not the complete private scanout state used by Core 1. For
// target-driven AUTO timing, pulse_base_ticks is the deterministic initial
// valid candidate, suitable for preparing storage, timing and startup.
// skls_hub75_init() prepares optional measurement refinement for Core 1 startup;
// the final running pulse value and status may differ from this candidate.
// SM dividers/rates are resolved for the supplied clk_sys_hz and remain fixed
// through startup measurement refinement.
typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t addressed_rows;
    uint8_t address_bits;
    uint8_t address_gpio_count;
    uint8_t unused_address_gpio_base_pin;
    uint8_t unused_address_gpio_count;
    uint8_t bitplane_count;
    uint8_t dither_phase_count;
    skls_hub75_transfer_mode_t transfer_mode;
    skls_hub75_display_policy_t display_policy;
    float gamma;
    uint32_t pulse_base_ticks;
    // RP2350 integer + 8-bit fractional dividers, encoded in 1/256 units.
    uint32_t data_sm_divider256;
    uint32_t row_sm_divider256;
    uint32_t data_sm_frequency_hz;
    uint32_t row_sm_frequency_hz;
    uint32_t presentation_divisor;
    uint32_t target_refresh_millihz;
    skls_hub75_target_policy_t target_policy;
    bool pulse_auto;
} skls_hub75_config_resolution_t;

// Validate without modifying any caller-owned value.
skls_hub75_config_result_t skls_hub75_config_validate(const skls_hub75_hardware_descriptor_t* hardware, const skls_hub75_config_t* config,
                                            const skls_hub75_resolution_policy_t* resolution_policy);

// Resolve only caller-authorised values against the actual clk_sys frequency.
// The output is written only on OK.
// Target-driven AUTO selects an initial valid pulse candidate; hardware
// measurement may refine it during the skls_hub75_init() startup lifecycle before
// normal render requests begin. Inspect runtime status for the final value.
skls_hub75_config_result_t skls_hub75_config_resolve(const skls_hub75_hardware_descriptor_t* hardware, const skls_hub75_config_t* config,
                                           const skls_hub75_resolution_policy_t* resolution_policy, uint32_t clk_sys_hz,
                                           skls_hub75_config_resolution_t* resolution);

#ifdef __cplusplus
} // extern "C"
#endif

#if defined(__arm__)
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_config_resolution_t) == 52, "ABI 1: sizeof(skls_hub75_config_resolution_t)");
SKLS_HUB75_BINARY_ASSERT(SKLS_HUB75_BINARY_ALIGNOF(skls_hub75_config_resolution_t) == 4, "ABI 1: SKLS_ABI_ALIGNOF(skls_hub75_config_resolution_t)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, width) == 0, "ABI 1: offsetof(skls_hub75_config_resolution_t, width)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, height) == 2, "ABI 1: offsetof(skls_hub75_config_resolution_t, height)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, addressed_rows) == 4, "ABI 1: offsetof(skls_hub75_config_resolution_t, addressed_rows)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, address_bits) == 6, "ABI 1: offsetof(skls_hub75_config_resolution_t, address_bits)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, address_gpio_count) == 7, "ABI 1: offsetof(skls_hub75_config_resolution_t, address_gpio_count)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, unused_address_gpio_base_pin) == 8, "ABI 1: offsetof(skls_hub75_config_resolution_t, unused_address_gpio_base_pin)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, unused_address_gpio_count) == 9, "ABI 1: offsetof(skls_hub75_config_resolution_t, unused_address_gpio_count)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, bitplane_count) == 10, "ABI 1: offsetof(skls_hub75_config_resolution_t, bitplane_count)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, dither_phase_count) == 11, "ABI 1: offsetof(skls_hub75_config_resolution_t, dither_phase_count)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, transfer_mode) == 12, "ABI 1: offsetof(skls_hub75_config_resolution_t, transfer_mode)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, display_policy) == 13, "ABI 1: offsetof(skls_hub75_config_resolution_t, display_policy)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, gamma) == 16, "ABI 1: offsetof(skls_hub75_config_resolution_t, gamma)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, pulse_base_ticks) == 20, "ABI 1: offsetof(skls_hub75_config_resolution_t, pulse_base_ticks)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, data_sm_divider256) == 24, "ABI 1: offsetof(skls_hub75_config_resolution_t, data_sm_divider256)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, row_sm_divider256) == 28, "ABI 1: offsetof(skls_hub75_config_resolution_t, row_sm_divider256)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, data_sm_frequency_hz) == 32, "ABI 1: offsetof(skls_hub75_config_resolution_t, data_sm_frequency_hz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, row_sm_frequency_hz) == 36, "ABI 1: offsetof(skls_hub75_config_resolution_t, row_sm_frequency_hz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, presentation_divisor) == 40, "ABI 1: offsetof(skls_hub75_config_resolution_t, presentation_divisor)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, target_refresh_millihz) == 44, "ABI 1: offsetof(skls_hub75_config_resolution_t, target_refresh_millihz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, target_policy) == 48, "ABI 1: offsetof(skls_hub75_config_resolution_t, target_policy)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_resolution_t, pulse_auto) == 49, "ABI 1: offsetof(skls_hub75_config_resolution_t, pulse_auto)");
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_config_result_t) == 1, "ABI 1: sizeof(skls_hub75_config_result_t)");
#endif
