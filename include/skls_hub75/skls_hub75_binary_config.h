#pragma once

#include <stddef.h>

// This generated package configuration applies even without CMake.
#define SKLS_HUB75_BINARY_ABI_VERSION 1

#if defined(SKLS_HUB75_PROFILE_REFRESH) && SKLS_HUB75_PROFILE_REFRESH != 0
#error "This SKLSHUB75 binary requires SKLS_HUB75_PROFILE_REFRESH=0"
#endif
#if defined(SKLS_HUB75_PROFILE_BITPLANES) && SKLS_HUB75_PROFILE_BITPLANES != 0
#error "This SKLSHUB75 binary requires SKLS_HUB75_PROFILE_BITPLANES=0"
#endif
#ifndef SKLS_HUB75_PROFILE_REFRESH
#define SKLS_HUB75_PROFILE_REFRESH 0
#endif
#ifndef SKLS_HUB75_PROFILE_BITPLANES
#define SKLS_HUB75_PROFILE_BITPLANES 0
#endif
#if defined(__arm__) && defined(__ARM_PCS_VFP)
#error "This SKLSHUB75 binary uses the base AAPCS floating-point calling convention"
#endif

#if defined(__arm__)
#ifdef __cplusplus
#define SKLS_HUB75_BINARY_ASSERT static_assert
#define SKLS_HUB75_BINARY_ALIGNOF alignof
#else
#define SKLS_HUB75_BINARY_ASSERT _Static_assert
#define SKLS_HUB75_BINARY_ALIGNOF _Alignof
#endif
SKLS_HUB75_BINARY_ASSERT(sizeof(void*) == 4, "SKLSHUB75 ABI 1 requires 32-bit pointers");
#endif
