/*
 * Copyright © 2014 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */


#ifndef BRW_CS_H
#define BRW_CS_H

#ifdef __cplusplus
extern "C" {
#endif

struct brw_cs_parameters {
   unsigned group_size;
   unsigned simd_size;
   unsigned threads;
};

struct brw_cs_parameters
brw_cs_get_parameters(const struct brw_context *brw);

void
brw_upload_cs_prog(struct brw_context *brw);

void
brw_cs_populate_key(struct brw_context *brw, struct brw_cs_prog_key *key);
void
brw_cs_populate_default_key(const struct brw_compiler *compiler,
                            struct brw_cs_prog_key *key,
                            struct gl_program *prog);

#ifdef __cplusplus
}
#endif

#endif /* BRW_CS_H */
