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

#include "util/u_memory.h"

#include <stdio.h>
#include <xf86drm.h>
#include "via_drm.h"

#include "openchrome_drm_winsys.h"
#include "openchrome_drm_bufmgr.h"
#include "openchrome/openchrome_screen.h"

struct pipe_screen *
openchrome_drm_winsys_create(int fd)
{
	struct openchrome_drm_winsys *ws = CALLOC_STRUCT(openchrome_drm_winsys);
	struct drm_via_param params;
	struct pb_manager *mgr;
	drmVersionPtr version;

	if (!ws)
		return NULL;

	ws->fd = fd;

	version = drmGetVersion(fd);
	if (version->version_major < 3) {
        debug_printf("%s: DRM version is %d.%d.%d but this driver requires "
		             "3.0 or above\n",  __FUNCTION__, version->version_major,
		             version->version_minor, version->version_patchlevel);
		drmFreeVersion(version);
		return NULL;
	}
	drmFreeVersion(version);

	params.param = VIA_PARAM_CHIPSET_ID;
	if (drmCommandWriteRead(fd, DRM_VIA_GETPARAM, &params,
							sizeof(params)) != 0) {
		debug_printf("%s: Failed to get PCI id\n", __FUNCTION__);
		return NULL;
	}
	mgr = openchrome_create_bufmgr(ws);

	return openchrome_screen_create(mgr, params.value);
}
