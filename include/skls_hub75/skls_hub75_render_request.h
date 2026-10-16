//
//  skls_hub75_render_request.h
//  skls-hub75
//
//  Created by Gary J.H. Atkinson on 2026-09-18.
//  Copyright © 2026 Stinky Kitten Ltd. All rights reserved.
//

#pragma once

#include <skls_hub75/skls_hub75_binary_config.h>

#include <stdint.h>

/*
 * Render requests use latest-value semantics.
 *
 * Requests are not queued. A renderer may miss any number of intermediate
 * requests while unavailable. When rendering is next serviced, the newest
 * available request should be used. Generation identifies the configuration
 * that owns the framebuffer; cached requests from older generations are not
 * eligible for a new render.
 *
 * framebuffer contains width x height tightly packed, row-major uint32_t
 * pixels. Each pixel has numeric value 0xAARRGGBB. HUB75 scanout consumes
 * the RGB components; it does not use the high alpha byte. This describes
 * numeric pixel values, independent of the CPU's byte order.
 *
 * presentation_time_us is the absolute time of the presentation boundary for
 * which the framebuffer is being prepared. Scene evaluation uses this time.
 * presentation_time_us and deadline_us use the Pico monotonic microsecond
 * timebase; neither is a wall-clock or UTC timestamp.
 *
 * deadline_us is an absolute deadline. The renderer must stop writing to the
 * supplied framebuffer before that deadline. It may ignore a request entirely
 * or terminate rendering early if necessary to honour the deadline.
 *
 * Failure to honour the deadline may produce display artefacts; the HUB75
 * driver is not required to delay framebuffer ownership transitions to protect
 * a late renderer.
 */
typedef struct {
    uint32_t generation;
    uint32_t* framebuffer;
    uint16_t width;
    uint16_t height;
    uint64_t presentation_time_us;
    uint64_t deadline_us;
} skls_hub75_render_request_t;

#if defined(__arm__)
SKLS_HUB75_BINARY_ASSERT(sizeof(skls_hub75_render_request_t) == 32, "ABI 1: sizeof(skls_hub75_render_request_t)");
SKLS_HUB75_BINARY_ASSERT(SKLS_HUB75_BINARY_ALIGNOF(skls_hub75_render_request_t) == 8, "ABI 1: SKLS_ABI_ALIGNOF(skls_hub75_render_request_t)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_render_request_t, generation) == 0, "ABI 1: offsetof(skls_hub75_render_request_t, generation)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_render_request_t, framebuffer) == 4, "ABI 1: offsetof(skls_hub75_render_request_t, framebuffer)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_render_request_t, width) == 8, "ABI 1: offsetof(skls_hub75_render_request_t, width)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_render_request_t, height) == 10, "ABI 1: offsetof(skls_hub75_render_request_t, height)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_render_request_t, presentation_time_us) == 16, "ABI 1: offsetof(skls_hub75_render_request_t, presentation_time_us)");
SKLS_HUB75_BINARY_ASSERT(offsetof(skls_hub75_render_request_t, deadline_us) == 24, "ABI 1: offsetof(skls_hub75_render_request_t, deadline_us)");
#endif
