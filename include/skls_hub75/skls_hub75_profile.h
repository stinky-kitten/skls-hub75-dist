//
//  skls_hub75_profile.h
//  skls-hub75
//
//  Created by Gary J.H. Atkinson on 2026-09-18.
//  Copyright © 2026 Stinky Kitten Ltd. All rights reserved.
//

#pragma once

#include <skls_hub75/skls_hub75_binary_config.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SKLS_HUB75_PROFILE_BITPLANES
#define SKLS_HUB75_PROFILE_BITPLANES 0
#endif

#ifndef SKLS_HUB75_PROFILE_REFRESH
#define SKLS_HUB75_PROFILE_REFRESH 0
#endif

#if SKLS_HUB75_PROFILE_BITPLANES
#undef SKLS_HUB75_PROFILE_REFRESH
#define SKLS_HUB75_PROFILE_REFRESH 1
#define SKLS_HUB75_PROFILE_MAX_BITPLANE_COUNT 8u

typedef struct {
    uintptr_t framebuffer_0_address;
    uintptr_t framebuffer_1_address;
    size_t framebuffer_size_bytes;
    size_t framebuffer_alignment_bytes;
    uintptr_t gamma_lut_address;
    size_t gamma_lut_size_bytes;
    size_t gamma_lut_alignment_bytes;
    uintptr_t row_pair_dma_buffer_address;
    size_t row_pair_dma_buffer_size_bytes;
    size_t row_pair_dma_buffer_alignment_bytes;
} skls_hub75_profile_memory_layout_t;

void skls_hub75_profile_get_memory_layout(skls_hub75_profile_memory_layout_t* layout);
#endif

#if SKLS_HUB75_PROFILE_REFRESH
typedef struct {
    uint64_t total_us;
    uint32_t minimum_us;
    uint32_t maximum_us;
    uint32_t sample_count;
} skls_hub75_profile_measurement_t;

#if SKLS_HUB75_PROFILE_BITPLANES
typedef struct {
    uint32_t over_50_us;
    uint32_t over_75_us;
    uint32_t over_100_us;
    uint32_t over_150_us;
    uint32_t over_250_us;
} skls_hub75_profile_build_stalls_t;

typedef struct {
    uint32_t pulse_ticks;
    skls_hub75_profile_measurement_t build;
    skls_hub75_profile_build_stalls_t build_stalls;
    skls_hub75_profile_measurement_t dma_wait;
    skls_hub75_profile_measurement_t data_wait;
    skls_hub75_profile_measurement_t row_wait;
    skls_hub75_profile_measurement_t complete_row;
} skls_hub75_profile_bitplane_t;
#endif

typedef struct {
    uint32_t physical_refresh_iterations;
    skls_hub75_profile_measurement_t refresh_period;
    skls_hub75_profile_measurement_t refresh_with_switch;
    skls_hub75_profile_measurement_t refresh_without_switch;
#if SKLS_HUB75_PROFILE_BITPLANES
    uint16_t addressed_rows;
    uint8_t bitplane_count;
    skls_hub75_profile_measurement_t unattributed_row_wait;
    skls_hub75_profile_bitplane_t bitplanes[SKLS_HUB75_PROFILE_MAX_BITPLANE_COUNT];
#endif
} skls_hub75_profile_snapshot_t;

bool skls_hub75_profile_snapshot_take(skls_hub75_profile_snapshot_t* snapshot);
#endif

#ifdef __cplusplus
} // extern "C"
#endif
