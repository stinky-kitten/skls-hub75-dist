//
//  skls_hub75_presets.h
//  skls-hub75
//
//  Created by Gary J.H. Atkinson on 2026-09-18.
//  Copyright © 2026 Stinky Kitten Ltd. All rights reserved.
//

#pragma once

#include <skls_hub75/skls_hub75_binary_config.h>

#include <skls_hub75/skls_hub75_config.h>

#ifdef __cplusplus
extern "C" {
#endif

// Return caller-selected geometry with the recommended mutable Pico wiring.
skls_hub75_hardware_descriptor_t skls_hub75_pico_default_hardware(uint16_t width, uint16_t height);

// Populate caller-owned values for the generic 6-bit, 4-phase, 240 Hz
// operating profile. Both output pointers must be non-null.
void skls_hub75_config_preset_6bit_4phase_240hz(skls_hub75_config_t* config, skls_hub75_resolution_policy_t* resolution_policy);

#ifdef __cplusplus
} // extern "C"
#endif
