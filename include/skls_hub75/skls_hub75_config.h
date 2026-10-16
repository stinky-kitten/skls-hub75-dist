//
//  skls_hub75_config.h
//  skls-hub75
//
//  Created by Gary J.H. Atkinson on 2026-09-18.
//  Copyright © 2026 Stinky Kitten Ltd. All rights reserved.
//

#pragma once

#include <skls_hub75/skls_hub75_binary_config.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Visible display geometry and supported Pico wiring for the current
// straight-row, two-bank topology. Pin groups remain consecutive.
typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t address_gpio_count;
    uint8_t rgb_base_pin;
    uint8_t row_address_base_pin;
    uint8_t clock_pin;
    uint8_t latch_pin;
    uint8_t output_enable_pin;
} skls_hub75_hardware_descriptor_t;

// The supported topology drives two RGB banks, so R = H / 2.
// Configuration validation checks the required geometry preconditions.
static inline uint16_t skls_hub75_hardware_addressed_rows(const skls_hub75_hardware_descriptor_t* hardware) {
    return (uint16_t)(hardware->height / 2u);
}

// Return ceil(log2(R)), where R is derived from H. R=1 requires no
// address bits. Configuration validation checks geometry before resolution.
static inline uint8_t skls_hub75_hardware_address_bits(const skls_hub75_hardware_descriptor_t* hardware) {
    uint16_t row_value = skls_hub75_hardware_addressed_rows(hardware);
    uint8_t address_bits = 0u;

    if (row_value != 0u) {
        --row_value;
    }

    while (row_value != 0u) {
        ++address_bits;
        row_value >>= 1u;
    }

    return address_bits;
}

typedef enum {
    SKLS_HUB75_TRANSFER_MODE_LINEAR,
    SKLS_HUB75_TRANSFER_MODE_GAMMA,
} skls_hub75_transfer_mode_t;

// Precisely the established 8-bit to six displayed bits over four physical
// refreshes conversion, including low-bit phase distribution and saturation.
typedef enum {
    SKLS_HUB75_DISPLAY_POLICY_CURRENT_6BIT_4PHASE,
} skls_hub75_display_policy_t;

typedef enum {
    SKLS_HUB75_TARGET_POLICY_STRICT,
    SKLS_HUB75_TARGET_POLICY_BEST_EFFORT,
    SKLS_HUB75_TARGET_POLICY_FIXED_TIMING,
} skls_hub75_target_policy_t;

typedef enum {
    SKLS_HUB75_SCAN_BITPLANE_MAJOR = 0,
    SKLS_HUB75_SCAN_ROW_MAJOR,
    SKLS_HUB75_SCAN_LOW3_ROW_GROUPED,
    SKLS_HUB75_SCAN_LOW4_ROW_GROUPED,
    SKLS_HUB75_SCAN_ROW_MAJOR_INTERLEAVED,
    SKLS_HUB75_SCAN_ROW_MAJOR_FOLDED,
    SKLS_HUB75_SCAN_SCHEDULE_COUNT,
} skls_hub75_scan_schedule_t;

// Desired display behaviour, including target-driven operation and an
// optional expert fixed-timing request.
typedef struct {
    uint8_t bitplane_count;
    uint8_t dither_phase_count;
    skls_hub75_transfer_mode_t transfer_mode;
    skls_hub75_display_policy_t display_policy;
    float gamma;
    // Integer millihertz matches runtime reporting while retaining fractional-Hz precision.
    uint32_t target_refresh_millihz;
    uint32_t presentation_divisor;
    skls_hub75_target_policy_t target_policy;
    // Optional expert raw timing request. It is authoritative only when the
    // matching resolution policy is FIXED; 316 is not an architectural default.
    uint32_t pulse_base_ticks;
    // Requested instruction rates in Hz. Only the matching FIXED policy makes
    // a request authoritative; resolution reports the quantised actual rate.
    uint32_t data_sm_frequency_hz;
    uint32_t row_sm_frequency_hz;
    // The current row program has fixed transition delays, not adjustable blanking.
    // A nonzero request is explicitly unsupported.
    uint32_t additional_timing_blanking_ns;
    // Immutable for a running generation. Zero retains the qualified order.
    skls_hub75_scan_schedule_t scan_schedule;
} skls_hub75_config_t;

typedef enum {
    SKLS_HUB75_RESOLUTION_AUTO,
    SKLS_HUB75_RESOLUTION_FIXED,
} skls_hub75_resolution_mode_t;

// Each field describes resolver authority for the like-named config field.
// AUTO permits driver selection; FIXED makes the requested pulse or rate
// authoritative. SM rate AUTO currently selects only the qualified default.
typedef struct {
    skls_hub75_resolution_mode_t pulse_base_ticks;
    skls_hub75_resolution_mode_t data_sm_frequency_hz;
    skls_hub75_resolution_mode_t row_sm_frequency_hz;
} skls_hub75_resolution_policy_t;

#ifdef __cplusplus
} // extern "C"
#endif

#if defined(__arm__)
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_hardware_descriptor_t) == 10, "ABI 1: sizeof(skls_hub75_hardware_descriptor_t)");
SKLS_HUB75_BINARY_ASSERT(SKLS_HUB75_BINARY_ALIGNOF(skls_hub75_hardware_descriptor_t) == 2, "ABI 1: SKLS_ABI_ALIGNOF(skls_hub75_hardware_descriptor_t)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_hardware_descriptor_t, width) == 0, "ABI 1: offsetof(skls_hub75_hardware_descriptor_t, width)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_hardware_descriptor_t, height) == 2, "ABI 1: offsetof(skls_hub75_hardware_descriptor_t, height)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_hardware_descriptor_t, address_gpio_count) == 4, "ABI 1: offsetof(skls_hub75_hardware_descriptor_t, address_gpio_count)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_hardware_descriptor_t, rgb_base_pin) == 5, "ABI 1: offsetof(skls_hub75_hardware_descriptor_t, rgb_base_pin)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_hardware_descriptor_t, row_address_base_pin) == 6, "ABI 1: offsetof(skls_hub75_hardware_descriptor_t, row_address_base_pin)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_hardware_descriptor_t, clock_pin) == 7, "ABI 1: offsetof(skls_hub75_hardware_descriptor_t, clock_pin)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_hardware_descriptor_t, latch_pin) == 8, "ABI 1: offsetof(skls_hub75_hardware_descriptor_t, latch_pin)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_hardware_descriptor_t, output_enable_pin) == 9, "ABI 1: offsetof(skls_hub75_hardware_descriptor_t, output_enable_pin)");
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_config_t) == 40, "ABI 1: sizeof(skls_hub75_config_t)");
SKLS_HUB75_BINARY_ASSERT(SKLS_HUB75_BINARY_ALIGNOF(skls_hub75_config_t) == 4, "ABI 1: SKLS_ABI_ALIGNOF(skls_hub75_config_t)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, bitplane_count) == 0, "ABI 1: offsetof(skls_hub75_config_t, bitplane_count)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, dither_phase_count) == 1, "ABI 1: offsetof(skls_hub75_config_t, dither_phase_count)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, transfer_mode) == 2, "ABI 1: offsetof(skls_hub75_config_t, transfer_mode)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, display_policy) == 3, "ABI 1: offsetof(skls_hub75_config_t, display_policy)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, gamma) == 4, "ABI 1: offsetof(skls_hub75_config_t, gamma)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, target_refresh_millihz) == 8, "ABI 1: offsetof(skls_hub75_config_t, target_refresh_millihz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, presentation_divisor) == 12, "ABI 1: offsetof(skls_hub75_config_t, presentation_divisor)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, target_policy) == 16, "ABI 1: offsetof(skls_hub75_config_t, target_policy)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, pulse_base_ticks) == 20, "ABI 1: offsetof(skls_hub75_config_t, pulse_base_ticks)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, data_sm_frequency_hz) == 24, "ABI 1: offsetof(skls_hub75_config_t, data_sm_frequency_hz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, row_sm_frequency_hz) == 28, "ABI 1: offsetof(skls_hub75_config_t, row_sm_frequency_hz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, additional_timing_blanking_ns) == 32, "ABI 1: offsetof(skls_hub75_config_t, additional_timing_blanking_ns)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_config_t, scan_schedule) == 36, "ABI 1: offsetof(skls_hub75_config_t, scan_schedule)");
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_resolution_policy_t) == 3, "ABI 1: sizeof(skls_hub75_resolution_policy_t)");
SKLS_HUB75_BINARY_ASSERT(SKLS_HUB75_BINARY_ALIGNOF(skls_hub75_resolution_policy_t) == 1, "ABI 1: SKLS_ABI_ALIGNOF(skls_hub75_resolution_policy_t)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_resolution_policy_t, pulse_base_ticks) == 0, "ABI 1: offsetof(skls_hub75_resolution_policy_t, pulse_base_ticks)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_resolution_policy_t, data_sm_frequency_hz) == 1, "ABI 1: offsetof(skls_hub75_resolution_policy_t, data_sm_frequency_hz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_resolution_policy_t, row_sm_frequency_hz) == 2, "ABI 1: offsetof(skls_hub75_resolution_policy_t, row_sm_frequency_hz)");
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_transfer_mode_t) == 1, "ABI 1: sizeof(skls_hub75_transfer_mode_t)");
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_display_policy_t) == 1, "ABI 1: sizeof(skls_hub75_display_policy_t)");
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_target_policy_t) == 1, "ABI 1: sizeof(skls_hub75_target_policy_t)");
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_scan_schedule_t) == 1, "ABI 1: sizeof(skls_hub75_scan_schedule_t)");
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_resolution_mode_t) == 1, "ABI 1: sizeof(skls_hub75_resolution_mode_t)");
#endif
