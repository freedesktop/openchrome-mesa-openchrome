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

#define _FILE_OFFSET_BITS 64

#include "pipebuffer/pb_buffer.h"
#include "util/u_memory.h"
#include "os/os_mman.h"

#include <stdio.h>
#include <errno.h>
#include "via_drm.h"
#include <xf86drm.h>

#include "openchrome_drm_bufmgr.h"
#include "openchrome_drm_winsys.h"

static void *
openchrome_bo_map(struct pb_buffer *buf, unsigned flags, void *flush_ctx)
{
	struct openchrome_bo *bo = (struct openchrome_bo *) buf;
	void *ptr = NULL;

	if (bo->ptr)
		return bo->ptr;

	ptr = os_mmap(0, bo->base.size, PROT_READ | PROT_WRITE,
				  MAP_SHARED, bo->mgr->ws->fd, bo->map_offset);
	if (ptr == MAP_FAILED) {
		debug_printf("mmap for 0x%08X failed with error %d\n",
				bo->handle, -errno);
		ptr = NULL;
	}
	bo->ptr = ptr;

	return ptr;
}

static void
openchrome_bo_unmap(struct pb_buffer *buf)
{
	struct openchrome_bo *bo = (struct openchrome_bo *) buf;

	if (bo->ptr)
		os_munmap(bo->ptr, bo->base.size);
}

static enum pipe_error
openchrome_bo_validate(struct pb_buffer *buf, struct pb_validate *vl,
					   unsigned flags)
{
	return PIPE_OK;
}

static void
openchrome_bo_fence(struct pb_buffer *buf, struct pipe_fence_handle *fence)
{
}

static void
openchrome_bo_get_base_buffer(struct pb_buffer *buf, struct pb_buffer **base_buf,
							  pb_size *offset)
{
	*base_buf = buf;
	*offset = 0;
}

static void
openchrome_bo_destroy(struct pb_buffer *buf)
{
	struct openchrome_bo *bo = (struct openchrome_bo *) buf;
	struct drm_gem_close args;

	openchrome_bo_unmap(buf);

	memset(&args, 0, sizeof(args));
	args.handle = bo->handle;
	drmIoctl(bo->mgr->ws->fd, DRM_IOCTL_GEM_CLOSE, &args);
	FREE(bo);
}

const struct pb_vtbl openchrome_vtbl = {
	openchrome_bo_destroy,
	openchrome_bo_map,
	openchrome_bo_unmap,
	openchrome_bo_validate,
	openchrome_bo_fence,
	openchrome_bo_get_base_buffer,
};

static void openchrome_bufmgr_destroy(struct pb_manager *mgr)
{
	FREE(mgr);
}

static struct pb_buffer *
openchrome_bufmgr_create_bo(struct pb_manager *mgr, pb_size size,
							const struct pb_desc *desc)
{
	struct openchrome_bufmgr *bomgr = openchrome_bomgr(mgr);
	struct openchrome_bo *buf = NULL;
	struct drm_via_gem_create args;

	buf = CALLOC_STRUCT(openchrome_bo);
	if (!buf)
		return NULL;

	memset(&args, 0, sizeof(args));

	switch (desc->usage) {
	case PB_USAGE_CPU_READ:
	case PB_USAGE_CPU_WRITE:
		args.domains = TTM_PL_FLAG_TT;
		break;

	case PB_USAGE_GPU_READ:
	case PB_USAGE_GPU_WRITE:
		args.domains = TTM_PL_FLAG_VRAM;
		break;
	}
	args.alignment = desc->alignment;
	args.size = size;
	if (drmIoctl(bomgr->ws->fd, DRM_VIA_GEM_CREATE, &args)) {
		FREE(buf);
		return NULL;
	}
	buf->map_offset = args.map_handle;
	buf->handle = args.handle;
	buf->mgr = bomgr;

	buf->base.alignment = args.alignment;
	buf->base.usage = desc->usage;
	buf->base.size = args.size;
	buf->base.vtbl = &openchrome_vtbl;

	return &buf->base;
}

static void
openchrome_bufmgr_flush(struct pb_manager *mgr)
{
}

static boolean
openchrome_bufmgr_is_buffer_busy(struct pb_manager *mgr,
								struct pb_buffer *buf)
{
	return FALSE;
}

struct pb_manager *
openchrome_create_bufmgr(struct openchrome_drm_winsys *ws)
{
	struct openchrome_bufmgr *mgr;

	mgr = CALLOC_STRUCT(openchrome_bufmgr);
	if (!mgr)
		return NULL;

	mgr->base.destroy = openchrome_bufmgr_destroy;
	mgr->base.create_buffer = openchrome_bufmgr_create_bo;
	mgr->base.flush = openchrome_bufmgr_flush;
	mgr->base.is_buffer_busy = openchrome_bufmgr_is_buffer_busy;

	mgr->ws = ws;

	return &mgr->base;
}
