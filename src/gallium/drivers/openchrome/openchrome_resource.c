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

#include "pipebuffer/pb_bufmgr.h"
#include "util/u_transfer.h"
#include "util/u_memory.h"
#include "util/u_format.h"

#include "openchrome_screen.h"
#include "openchrome_resource.h"

static void
openchrome_buffer_destroy(struct pipe_screen *screen,
							struct pipe_resource *resource)
{
	struct openchrome_resource *res = MALLOC_STRUCT(openchrome_resource);

	FREE(res);
}

static void *
openchrome_buffer_transfer_map(struct pipe_context *ctx,
								struct pipe_resource *resource, unsigned level,
								unsigned usage, const struct pipe_box *box,
								struct pipe_transfer **ptransfer)
{
	return NULL;
}

struct u_resource_vtbl openchrome_buffer_vtbl =
{
	u_default_resource_get_handle,		/* get_handle */
	openchrome_buffer_destroy,			/* resource_destroy */
	openchrome_buffer_transfer_map,		/* transfer_map */
	u_default_transfer_flush_region,	/* transfer_flush_region */
	u_default_transfer_unmap,			/* transfer_unmap */
	u_default_transfer_inline_write		/* transfer_inline_write */
};

static struct pipe_resource *
openchrome_buffer_create(struct pipe_screen *screen,
							const struct pipe_resource *templ)
{
	struct openchrome_resource *res = MALLOC_STRUCT(openchrome_resource);
	struct openchrome_screen *pscreen = openchrome_screen(screen);
	struct pb_desc desc;
	unsigned int stride;
	pb_size size;

	if (!res)
		goto err;

	stride = util_format_get_stride(templ->format, templ->width0);
	size = align(stride * templ->height0 * templ->depth0, 16);

	memset(&desc, 0, sizeof(desc));
	desc.alignment = 16;
	desc.usage = PB_USAGE_GPU_READ | PB_USAGE_GPU_WRITE;

	res->buf = pscreen->mgr->create_buffer(pscreen->mgr, size, &desc);
	if (!res->buf) {
		FREE(res);
		res = NULL;
		goto err;
	}

	res->base.b = *templ;
	res->base.vtbl = &openchrome_buffer_vtbl;
	pipe_reference_init(&res->base.b.reference, 1);
	res->base.b.screen = screen;

err:
	return &res->base.b;
}

static struct pipe_resource *
openchrome_resource_create(struct pipe_screen *screen,
							const struct pipe_resource *template)
{
	if (template->target == PIPE_BUFFER)
		return openchrome_buffer_create(screen, template);
	else
		return NULL; //openchrome_texture_create(screen, template, FALSE);
}

static struct pipe_resource *
openchrome_resource_from_handle(struct pipe_screen * screen,
								const struct pipe_resource *template,
								struct winsys_handle *whandle)
{
	if (template->target == PIPE_BUFFER)
		return NULL;
	else
		return NULL; //openchrome_texture_from_handle(screen, template, whandle);
}

void openchrome_screen_resource_init(struct pipe_screen *screen)
{
	screen->resource_create = openchrome_resource_create;
	screen->resource_from_handle = openchrome_resource_from_handle;
	screen->resource_get_handle = u_resource_get_handle_vtbl;
	screen->resource_destroy = u_resource_destroy_vtbl;
}
