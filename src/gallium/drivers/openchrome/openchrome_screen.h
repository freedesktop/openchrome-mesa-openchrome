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

#ifndef OPENCHROME_SCREEN_H
#define OPENCHROME_SCREEN_H

#include "pipe/p_state.h"
#include "pipe/p_screen.h"

#define PCI_CHIP_CLE3122	0x3122	/* CLE266 */
#define PCI_CHIP_VT3205		0x7205	/* KM400 */
#define PCI_CHIP_VT3204		0x3108	/* K8M800 */
#define PCI_CHIP_VT3259		0x3118	/* PM800 */
#define PCI_CHIP_VT3314		0x3344	/* VM800 */
#define PCI_CHIP_VT3324		0x3157	/* CX700 */
#define PCI_CHIP_VT3336		0x3230	/* K8M890 */
#define PCI_CHIP_VT3327		0x3343	/* P4M890 */
#define PCI_CHIP_VT3364		0x3371	/* P4M900 */
#define PCI_CHIP_VT3353		0x1122	/* VX800 */
#define PCI_CHIP_VT3409		0x5122	/* VX855/VX875 */
#define PCI_CHIP_VT3410		0x7122	/* VX900 */
#define PCI_CHIP_VT3293		0x3225	/* CN750 */

/**
 * Subclass of pipe_screen
 */
struct openchrome_screen
{
    struct pipe_screen base;
	struct pb_manager *mgr;

    struct openchrome_drm_winsys *ws;
	int chipset_id;
};

static INLINE struct openchrome_screen *
openchrome_screen(struct pipe_screen *pscreen)
{
    return (struct openchrome_screen *) pscreen;
}

struct pipe_screen *openchrome_screen_create(struct pb_manager *mgr, int chipset_id);
void openchrome_screen_resource_init(struct pipe_screen *screen);

#endif	/* OPENCHROME_SCREEN_H */
