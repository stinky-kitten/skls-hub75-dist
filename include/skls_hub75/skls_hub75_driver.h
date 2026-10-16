//
//  skls_hub75_driver.h
//  skls-hub75
//
//  Created by Gary J.H. Atkinson on 2026-09-18.
//  Copyright © 2026 Stinky Kitten Ltd. All rights reserved.
//

#pragma once

#include <skls_hub75/skls_hub75_binary_config.h>

#include <stdbool.h>
#include <stdint.h>

#include <skls_hub75/skls_hub75_config.h>
#include <skls_hub75/skls_hub75_render_request.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SKLS_HUB75_LIFECYCLE_UNINITIALIZED,
    SKLS_HUB75_LIFECYCLE_CONFIGURING,
    SKLS_HUB75_LIFECYCLE_CALIBRATING,
    SKLS_HUB75_LIFECYCLE_RUNNING,
    SKLS_HUB75_LIFECYCLE_QUIESCING,
    SKLS_HUB75_LIFECYCLE_STOPPED,
    SKLS_HUB75_LIFECYCLE_FAILED,
} skls_hub75_lifecycle_state_t;

typedef enum {
    SKLS_HUB75_LIFECYCLE_RESULT_OK,
    SKLS_HUB75_LIFECYCLE_RESULT_INVALID_CONFIG,
    SKLS_HUB75_LIFECYCLE_RESULT_STORAGE_UNAVAILABLE,
    SKLS_HUB75_LIFECYCLE_RESULT_STOP_TIMEOUT,
    SKLS_HUB75_LIFECYCLE_RESULT_START_TIMEOUT,
    SKLS_HUB75_LIFECYCLE_RESULT_CALIBRATION_FAILED,
    SKLS_HUB75_LIFECYCLE_RESULT_HARDWARE_FAILED,
    SKLS_HUB75_LIFECYCLE_RESULT_ALREADY_INITIALIZED,
} skls_hub75_lifecycle_result_t;

typedef enum {
    SKLS_HUB75_TARGET_RESULT_ACHIEVED,
    SKLS_HUB75_TARGET_RESULT_CONSTRAINT_LIMITED,
    SKLS_HUB75_TARGET_RESULT_FAILED,
    SKLS_HUB75_TARGET_RESULT_FIXED_TIMING,
} skls_hub75_target_result_t;

// Compact runtime telemetry. Requested descriptor/configuration objects are
// intentionally not embedded here or in the Core 1 -> Core 0 status sample.
typedef struct {
    uint32_t generation;
    skls_hub75_lifecycle_state_t lifecycle_state;
    uint32_t pulse_base_ticks;
    uint32_t data_sm_frequency_hz;
    uint32_t row_sm_frequency_hz;
    // Actual selected RP2350 dividers, in units of 1/256.
    uint32_t data_sm_divider256;
    uint32_t row_sm_divider256;
    uint32_t presentation_divisor;
    uint32_t physical_refresh_period_us;
    uint32_t physical_refresh_millihz;
    uint32_t presentation_millihz;
    uint32_t requested_physical_refresh_millihz;
    // PIO-only maximum, not the expected or observed refresh rate.
    uint32_t modelled_max_physical_refresh_millihz;
    uint8_t calibration_attempts;
    skls_hub75_target_result_t target_result;
} skls_hub75_status_t;

// Validate, resolve and allocate the initial generation before firmware launches
// Core 1. Returns ALREADY_INITIALIZED if any generation was committed, including
// one subsequently stopped; use skls_hub75_reconfigure() to restart it. Invalid
// requests return INVALID_CONFIG and allocation failure returns
// STORAGE_UNAVAILABLE without committing a generation; success returns OK.
skls_hub75_lifecycle_result_t skls_hub75_init(const skls_hub75_hardware_descriptor_t* hardware, const skls_hub75_config_t* config,
                                    const skls_hub75_resolution_policy_t* resolution_policy);

// Core-0 control-plane operations; callers serialize them with Core-0 render
// work. STOP is idempotent and releases framebuffer storage only after the
// final published render deadline. Reconfigure validates and allocates before
// stopping a running generation, then restarts under a new generation on the
// already launched Core-1 worker. A stopped or failed driver is restarted
// through reconfigure. Waits are bounded;
// a timeout leaves any storage still owned by Core 1 intact for a later stop.
skls_hub75_lifecycle_result_t skls_hub75_stop(void);
skls_hub75_lifecycle_result_t skls_hub75_reconfigure(const skls_hub75_hardware_descriptor_t* hardware, const skls_hub75_config_t* config,
                                           const skls_hub75_resolution_policy_t* resolution_policy);
bool skls_hub75_render_request_is_current(const skls_hub75_render_request_t* request);
// Consume the latest request, if one has been published since the last take.
bool skls_hub75_take_render_request(skls_hub75_render_request_t* request);

// Read status without waiting for Core 1. A null pointer returns false without
// writing. Before any generation has been committed, returns false and fills
// generation=0 and lifecycle_state=UNINITIALIZED. Once a generation exists,
// returns true and fills its generation and lifecycle state. Telemetry is filled
// only when a stable sample from that generation is available; otherwise it
// remains zero/default (including during startup, stop/reconfiguration, and a
// concurrent status publication). A true return alone does not imply that
// telemetry has been published.
bool skls_hub75_get_status(skls_hub75_status_t* status);

// Firmware launches Core 1 once, after its own platform setup. This worker
// remains alive and accepts later committed generations after stop/failure.
void skls_hub75_core1_run(void);

#ifdef __cplusplus
} // extern "C"
#endif

#if defined(__arm__)
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_status_t) == 56, "ABI 1: sizeof(skls_hub75_status_t)");
SKLS_HUB75_BINARY_ASSERT(SKLS_HUB75_BINARY_ALIGNOF(skls_hub75_status_t) == 4, "ABI 1: SKLS_ABI_ALIGNOF(skls_hub75_status_t)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, generation) == 0, "ABI 1: offsetof(skls_hub75_status_t, generation)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, lifecycle_state) == 4, "ABI 1: offsetof(skls_hub75_status_t, lifecycle_state)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, pulse_base_ticks) == 8, "ABI 1: offsetof(skls_hub75_status_t, pulse_base_ticks)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, data_sm_frequency_hz) == 12, "ABI 1: offsetof(skls_hub75_status_t, data_sm_frequency_hz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, row_sm_frequency_hz) == 16, "ABI 1: offsetof(skls_hub75_status_t, row_sm_frequency_hz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, data_sm_divider256) == 20, "ABI 1: offsetof(skls_hub75_status_t, data_sm_divider256)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, row_sm_divider256) == 24, "ABI 1: offsetof(skls_hub75_status_t, row_sm_divider256)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, presentation_divisor) == 28, "ABI 1: offsetof(skls_hub75_status_t, presentation_divisor)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, physical_refresh_period_us) == 32, "ABI 1: offsetof(skls_hub75_status_t, physical_refresh_period_us)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, physical_refresh_millihz) == 36, "ABI 1: offsetof(skls_hub75_status_t, physical_refresh_millihz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, presentation_millihz) == 40, "ABI 1: offsetof(skls_hub75_status_t, presentation_millihz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, requested_physical_refresh_millihz) == 44, "ABI 1: offsetof(skls_hub75_status_t, requested_physical_refresh_millihz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, modelled_max_physical_refresh_millihz) == 48, "ABI 1: offsetof(skls_hub75_status_t, modelled_max_physical_refresh_millihz)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, calibration_attempts) == 52, "ABI 1: offsetof(skls_hub75_status_t, calibration_attempts)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_status_t, target_result) == 53, "ABI 1: offsetof(skls_hub75_status_t, target_result)");
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_lifecycle_state_t) == 1, "ABI 1: sizeof(skls_hub75_lifecycle_state_t)");
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_lifecycle_result_t) == 1, "ABI 1: sizeof(skls_hub75_lifecycle_result_t)");
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_target_result_t) == 1, "ABI 1: sizeof(skls_hub75_target_result_t)");
#endif
