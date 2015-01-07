/**************************************************************************
 *
 * Copyright 2013 The Openchrome Project [openchrome.org]
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/

#include "util/u_string.h"
#include "util/u_memory.h"

#include "openchrome_screen.h"

static const char *
openchrome_get_vendor(struct pipe_screen *screen)
{
	return "VIA Technologies, Inc.";
}

static const char *
openchrome_get_name(struct pipe_screen *screen)
{
	struct openchrome_screen *pscreen = openchrome_screen(screen);
	static char buffer[128];
	const char *chipset;

	switch (pscreen->chipset_id) {
	case PCI_CHIP_CLE3122:
		chipset = "CLE266";
		break;

	case PCI_CHIP_VT3205:
		chipset = "KM400";
		break;

	case PCI_CHIP_VT3204:
		chipset = "K8M800";
		break;

	case PCI_CHIP_VT3259:
		chipset = "PM800";
		break;

	case PCI_CHIP_VT3314:
		chipset = "VM800";
		break;

	case PCI_CHIP_VT3324:
		chipset = "CX700";
		break;

	case PCI_CHIP_VT3336:
		chipset = "K8M890";
		break;

	case PCI_CHIP_VT3327:
		chipset = "P4M890";
		break;

	case PCI_CHIP_VT3364:
		chipset = "P4M900";
		break;

	case PCI_CHIP_VT3353:
		chipset = "VX800";
		break;

	case PCI_CHIP_VT3409:
		chipset = "VX855/VX875";
		break;

	case PCI_CHIP_VT3410:
		chipset = "VX900";
		break;

	case PCI_CHIP_VT3293:
		chipset = "CN750";
		break;

	default:
		chipset = "unknown";
		break;
	}

    util_snprintf(buffer, sizeof(buffer), "via (chipset: %s)", chipset);
    return buffer;
}

static int
openchrome_get_param(struct pipe_screen *screen, enum pipe_cap param)
{
	return 0;
}

static float
openchrome_get_paramf(struct pipe_screen *screen, enum pipe_capf param)
{
	return 0.0f;
}

static int
openchrome_get_shader_param(struct pipe_screen *screen, unsigned shader,
                            enum pipe_shader_cap param)
{
	return 0;
}

static int
openchrome_get_video_param(struct pipe_screen *screen,
                            enum pipe_video_profile profile,
                            enum pipe_video_cap param)
{
	return 0;
}

static int
openchrome_get_compute_param(struct pipe_screen *screen,
                            enum pipe_compute_cap param, void *ret)
{
	return 0;
}

static uint64_t
openchrome_get_timestamp(struct pipe_screen *screen)
{
	return 0;
}

static void
openchrome_destroy_screen(struct pipe_screen *screen)
{
	struct openchrome_screen *pscreen = openchrome_screen(screen);

	/*if (pscreen->ws)
		pscreen->ws->destroy(pscreen->ws);*/

	FREE(pscreen);
}

struct pipe_screen *
openchrome_screen_create(struct pb_manager *mgr, int chipset_id)
{
	struct openchrome_screen *screen = CALLOC_STRUCT(openchrome_screen);
	struct pipe_screen *pscreen;

	if (!screen)
		return NULL;

	pscreen = &screen->base;

	screen->base.get_name = openchrome_get_name;
	screen->base.get_vendor = openchrome_get_vendor;
	screen->base.destroy = openchrome_destroy_screen;
	screen->base.get_param = openchrome_get_param;
	screen->base.get_paramf = openchrome_get_paramf;
	screen->base.get_shader_param = openchrome_get_shader_param;
	screen->base.get_video_param = openchrome_get_video_param;
	screen->base.get_compute_param = openchrome_get_compute_param;
	screen->base.get_timestamp = openchrome_get_timestamp;

	screen->chipset_id = chipset_id;
	screen->mgr = mgr;

	openchrome_screen_resource_init(pscreen);

	return pscreen;
}
