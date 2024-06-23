/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2019 - Hans-Kristian Arntzen
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

/* Modern OpenGL driver.
 *
 * Minimum version (desktop): OpenGL   3.2+ (2009)
 * Minimum version (mobile) : OpenGLES 3.0+ (2012)
 */

#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif

#include "../common/gl3_common.h"

#include <gfx/gl_capabilities.h>
#include <gfx/video_frame.h>
#include <glsym/glsym.h>
#include <string/stdstring.h>
#include <retro_math.h>

#include "../../configuration.h"
#include "../../dynamic.h"
#ifdef HAVE_REWIND
#include "../../state_manager.h"
#endif

#include "../../retroarch.h"
#include "../../verbosity.h"

#ifdef HAVE_THREADS
#include "../video_thread_wrapper.h"
#endif

#include "../font_driver.h"
#include "../../record/record_driver.h"

#ifdef HAVE_MENU
#include "../../menu/menu_driver.h"
#endif
#ifdef HAVE_GFX_WIDGETS
#include "../gfx_widgets.h"
#endif

static const struct video_ortho gl3_default_ortho = {0, 1, 0, 1, -1, 1};

void gl3_framebuffer_copy(
      GLuint fb_id,
      GLuint quad_program,
      GLuint quad_vbo,
      GLint flat_ubo_vertex,
      struct Size2D size,
      GLuint image)
{
   glBindFramebuffer(GL_FRAMEBUFFER, fb_id);
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, image);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glViewport(0, 0, size.width, size.height);
   glClear(GL_COLOR_BUFFER_BIT);

   glUseProgram(quad_program);
   if (flat_ubo_vertex >= 0)
   {
      static float mvp[16] = { 
                                2.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 2.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 2.0f, 0.0f,
                               -1.0f,-1.0f, 0.0f, 1.0f
                             };
      glUniform4fv(flat_ubo_vertex, 4, mvp);
   }

   /* Draw quad */
   glDisable(GL_CULL_FACE);
   glDisable(GL_BLEND);
   glDisable(GL_DEPTH_TEST);
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                         (void *)((uintptr_t)(0)));
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                         (void *)((uintptr_t)(2 * sizeof(float))));
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);

   glUseProgram(0);
   glBindTexture(GL_TEXTURE_2D, 0);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gl3_framebuffer_copy_partial(
      GLuint fb_id,
      GLuint quad_program, 
      GLint flat_ubo_vertex,
      struct Size2D size,
      GLuint image,
      float rx, float ry)
{
   GLuint vbo;
   const float quad_data[16] = {
      0.0f, 0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, rx,   0.0f,
      0.0f, 1.0f, 0.0f, ry,
      1.0f, 1.0f, rx,   ry,
   };

   glBindFramebuffer(GL_FRAMEBUFFER, fb_id);
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, image);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glViewport(0, 0, size.width, size.height);
   glClear(GL_COLOR_BUFFER_BIT);

   glUseProgram(quad_program);
   if (flat_ubo_vertex >= 0)
   {
      static float mvp[16] = { 
                                2.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 2.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 2.0f, 0.0f,
                               -1.0f,-1.0f, 0.0f, 1.0f
                             };
      glUniform4fv(flat_ubo_vertex, 4, mvp);
   }
   glDisable(GL_CULL_FACE);
   glDisable(GL_BLEND);
   glDisable(GL_DEPTH_TEST);
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);

   /* A bit crude, but heeeey. */
   glGenBuffers(1, &vbo);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STREAM_DRAW);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                         (void *)((uintptr_t)(0)));
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                         (void *)((uintptr_t)(2 * sizeof(float))));
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glDeleteBuffers(1, &vbo);
   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
   glUseProgram(0);
   glBindTexture(GL_TEXTURE_2D, 0);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void gl3_deinit_fences(gl3_t *gl)
{
   int i;
   for (i = 0; i < gl->fence_count; i++)
   {
      if (gl->fences[i])
         glDeleteSync(gl->fences[i]);
   }
   gl->fence_count = 0;
   memset(gl->fences, 0, sizeof(gl->fences));
}

static bool gl3_init_pbo_readback(gl3_t *gl)
{
   int i;
   struct scaler_ctx *scaler  = NULL;

   glGenBuffers(GL_CORE_NUM_PBOS, gl->pbo_readback);

   for (i = 0; i < GL_CORE_NUM_PBOS; i++)
   {
      glBindBuffer(GL_PIXEL_PACK_BUFFER, gl->pbo_readback[i]);
      glBufferData(GL_PIXEL_PACK_BUFFER,
            gl->vp.width * gl->vp.height * sizeof(uint32_t),
            NULL, GL_STREAM_READ);
   }
   glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

   scaler                    = &gl->pbo_readback_scaler;

   scaler->in_width          = gl->vp.width;
   scaler->in_height         = gl->vp.height;
   scaler->out_width         = gl->vp.width;
   scaler->out_height        = gl->vp.height;
   scaler->in_stride         = gl->vp.width * sizeof(uint32_t);
   scaler->out_stride        = gl->vp.width * 3;
   scaler->in_fmt            = SCALER_FMT_ABGR8888;
   scaler->out_fmt           = SCALER_FMT_BGR24;
   scaler->scaler_type       = SCALER_TYPE_POINT;

   if (!scaler_ctx_gen_filter(scaler))
   {
      gl->pbo_readback_enable = false;
      RARCH_ERR("[GLCore]: Failed to initialize pixel conversion for PBO.\n");
      glDeleteBuffers(4, gl->pbo_readback);
      memset(gl->pbo_readback, 0, sizeof(gl->pbo_readback));
      return false;
   }

   return true;
}

static void gl3_deinit_pbo_readback(gl3_t *gl)
{
   int i;
   for (i = 0; i < GL_CORE_NUM_PBOS; i++)
      if (gl->pbo_readback[i] != 0)
         glDeleteBuffers(1, &gl->pbo_readback[i]);
   memset(gl->pbo_readback, 0, sizeof(gl->pbo_readback));
   scaler_ctx_gen_reset(&gl->pbo_readback_scaler);
}

static void gl3_slow_readback(gl3_t *gl, void *buffer)
{
   glPixelStorei(GL_PACK_ALIGNMENT, 4);
   glPixelStorei(GL_PACK_ROW_LENGTH, 0);
   glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
#ifndef HAVE_OPENGLES
   glReadBuffer(GL_BACK);
#endif

   glReadPixels(gl->vp.x, gl->vp.y,
                gl->vp.width, gl->vp.height,
                GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}

static void gl3_pbo_async_readback(gl3_t *gl)
{
   glBindBuffer(GL_PIXEL_PACK_BUFFER,
         gl->pbo_readback[gl->pbo_readback_index++]);
   glPixelStorei(GL_PACK_ALIGNMENT, 4);
   glPixelStorei(GL_PACK_ROW_LENGTH, 0);
#ifndef HAVE_OPENGLES
   glReadBuffer(GL_BACK);
#endif
   if (gl->pbo_readback_index >= GL_CORE_NUM_PBOS)
      gl->pbo_readback_index = 0;
   gl->pbo_readback_valid[gl->pbo_readback_index] = true;

   glReadPixels(gl->vp.x, gl->vp.y,
                gl->vp.width, gl->vp.height,
                GL_RGBA, GL_UNSIGNED_BYTE, NULL);
   glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

static void gl3_fence_iterate(gl3_t *gl, unsigned hard_sync_frames)
{
   if (gl->fence_count < GL_CORE_NUM_FENCES)
   {
      /*
       * We need to do some work after the flip, or we risk fencing too early.
       * Do as little work as possible.
       */
      glEnable(GL_SCISSOR_TEST);
      glScissor(0, 0, 1, 1);
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glClear(GL_COLOR_BUFFER_BIT);
      glDisable(GL_SCISSOR_TEST);

      gl->fences[gl->fence_count++] = glFenceSync(
            GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
   }

   while (gl->fence_count > hard_sync_frames)
   {
      glClientWaitSync(gl->fences[0], GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
      glDeleteSync(gl->fences[0]);
      gl->fence_count--;
      memmove(gl->fences, gl->fences + 1, gl->fence_count * sizeof(GLsync));
   }
}

uint32_t gl3_get_cross_compiler_target_version(void)
{
   const char *version = (const char*)glGetString(GL_VERSION);
   unsigned major      = 0;
   unsigned minor      = 0;

#ifdef HAVE_OPENGLES3
   if (!version || sscanf(version, "OpenGL ES %u.%u", &major, &minor) != 2)
      return 300;
   
   if (major == 2 && minor == 0)
      return 100;
#else
   if (!version || sscanf(version, "%u.%u", &major, &minor) != 2)
      return 150;

   if (major == 3)
   {
      switch (minor)
      {
         case 2:
            return 150;
         case 1:
            return 140;
         case 0:
            return 130;
      }
   }
   else if (major == 2)
   {
      switch (minor)
      {
         case 1:
            return 120;
         case 0:
            return 110;
      }
   }
#endif

   return 100 * major + 10 * minor;
}

GLuint gl3_compile_shader(GLenum stage, const char *source)
{
   GLint status;
   GLuint shader   = glCreateShader(stage);
   const char *ptr = source;

   glShaderSource(shader, 1, &ptr, NULL);
   glCompileShader(shader);

   glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

   if (!status)
   {
      GLint length;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
      if (length > 0)
      {
         char *info_log = (char*)malloc(length);

         if (info_log)
         {
            glGetShaderInfoLog(shader, length, &length, info_log);
            RARCH_ERR("[GLCore]: Failed to compile shader: %s\n", info_log);
            free(info_log);
            glDeleteShader(shader);
            return 0;
         }
      }
   }

   return shader;
}


void gl3_framebuffer_clear(GLuint id)
{
   glBindFramebuffer(GL_FRAMEBUFFER, id);
   glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gl3_bind_scratch_vbo(gl3_t *gl, const void *data, size_t size)
{
   if (!gl->scratch_vbos[gl->scratch_vbo_index])
      glGenBuffers(1, &gl->scratch_vbos[gl->scratch_vbo_index]);
   glBindBuffer(GL_ARRAY_BUFFER, gl->scratch_vbos[gl->scratch_vbo_index]);
   glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
   gl->scratch_vbo_index++;
   if (gl->scratch_vbo_index >= GL_CORE_NUM_VBOS)
      gl->scratch_vbo_index = 0;
}

#ifdef HAVE_OVERLAY
static void gl3_free_overlay(gl3_t *gl)
{
   if (gl->overlay_tex)
      glDeleteTextures(gl->overlays, gl->overlay_tex);

   free(gl->overlay_tex);
   free(gl->overlay_vertex_coord);
   free(gl->overlay_tex_coord);
   free(gl->overlay_color_coord);
   gl->overlay_tex          = NULL;
   gl->overlay_vertex_coord = NULL;
   gl->overlay_tex_coord    = NULL;
   gl->overlay_color_coord  = NULL;
   gl->overlays             = 0;
}

static void gl3_free_scratch_vbos(gl3_t *gl)
{
   int i;
   for (i = 0; i < GL_CORE_NUM_VBOS; i++)
      if (gl->scratch_vbos[i])
         glDeleteBuffers(1, &gl->scratch_vbos[i]);
}

static void gl3_overlay_vertex_geom(void *data,
      unsigned image,
      float x, float y,
      float w, float h)
{
   GLfloat *vertex = NULL;
   gl3_t   *gl = (gl3_t*)data;

   if (!gl)
      return;

   if (image > gl->overlays)
      return;

   vertex          = (GLfloat*)&gl->overlay_vertex_coord[image * 8];

   /* Flipped, so we preserve top-down semantics. */
   y               = 1.0f - y;
   h               = -h;

   vertex[0]       = x;
   vertex[1]       = y;
   vertex[2]       = x + w;
   vertex[3]       = y;
   vertex[4]       = x;
   vertex[5]       = y + h;
   vertex[6]       = x + w;
   vertex[7]       = y + h;
}

static void gl3_overlay_tex_geom(void *data,
      unsigned image,
      GLfloat x, GLfloat y,
      GLfloat w, GLfloat h)
{
   GLfloat *tex = NULL;
   gl3_t *gl    = (gl3_t*)data;

   if (!gl)
      return;

   tex          = (GLfloat*)&gl->overlay_tex_coord[image * 8];

   tex[0]       = x;
   tex[1]       = y;
   tex[2]       = x + w;
   tex[3]       = y;
   tex[4]       = x;
   tex[5]       = y + h;
   tex[6]       = x + w;
   tex[7]       = y + h;
}

static void gl3_render_overlay(gl3_t *gl,
      unsigned width, unsigned height)
{
   int i;

   glEnable(GL_BLEND);
   glDisable(GL_CULL_FACE);
   glDisable(GL_DEPTH_TEST);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBlendEquation(GL_FUNC_ADD);

   if (gl->overlay_full_screen)
      glViewport(0, 0, width, height);

   /* Ensure that we reset the attrib array. */
   glUseProgram(gl->pipelines.alpha_blend);
   if (gl->pipelines.alpha_blend_loc.flat_ubo_vertex >= 0)
      glUniform4fv(gl->pipelines.alpha_blend_loc.flat_ubo_vertex, 4, gl->mvp_no_rot.data);

   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);

   gl3_bind_scratch_vbo(gl, gl->overlay_vertex_coord, 8 * sizeof(float) * gl->overlays);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(uintptr_t)0);
   gl3_bind_scratch_vbo(gl, gl->overlay_tex_coord, 8 * sizeof(float) * gl->overlays);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(uintptr_t)0);
   gl3_bind_scratch_vbo(gl, gl->overlay_color_coord, 16 * sizeof(float) * gl->overlays);
   glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(uintptr_t)0);

   for (i = 0; i < gl->overlays; i++)
   {
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, gl->overlay_tex[i]);
      glDrawArrays(GL_TRIANGLE_STRIP, 4 * i, 4);
   }

   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
   glDisableVertexAttribArray(2);

   glDisable(GL_BLEND);
   glBindTexture(GL_TEXTURE_2D, 0);
   if (gl->overlay_full_screen)
      glViewport(gl->vp.x, gl->vp.y, gl->vp.width, gl->vp.height);
}
#endif

static void gl3_deinit_hw_render(gl3_t *gl)
{
   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, true);

   if (gl->hw_render_fbo)
      glDeleteFramebuffers(1, &gl->hw_render_fbo);
   if (gl->hw_render_rb_ds)
      glDeleteRenderbuffers(1, &gl->hw_render_rb_ds);
   if (gl->hw_render_texture)
      glDeleteTextures(1, &gl->hw_render_texture);

   gl->hw_render_fbo     = 0;
   gl->hw_render_rb_ds   = 0;
   gl->hw_render_texture = 0;

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, false);

   gl->hw_render_enable  = false;
}

static void gl3_destroy_resources(gl3_t *gl)
{
   unsigned i;
   if (!gl)
      return;

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, false);

   if (gl->filter_chain)
      gl3_filter_chain_free(gl->filter_chain);
   gl->filter_chain = NULL;

   glBindVertexArray(0);
   if (gl->vao != 0)
      glDeleteVertexArrays(1, &gl->vao);

   for (i = 0; i < GL_CORE_NUM_TEXTURES; i++)
   {
      if (gl->textures[i].tex != 0)
         glDeleteTextures(1, &gl->textures[i].tex);
   }
   memset(gl->textures, 0, sizeof(gl->textures));

   if (gl->menu_texture != 0)
      glDeleteTextures(1, &gl->menu_texture);

   if (gl->pipelines.alpha_blend)
      glDeleteProgram(gl->pipelines.alpha_blend);
   if (gl->pipelines.font)
      glDeleteProgram(gl->pipelines.font);
   if (gl->pipelines.ribbon)
      glDeleteProgram(gl->pipelines.ribbon);
   if (gl->pipelines.ribbon_simple)
      glDeleteProgram(gl->pipelines.ribbon_simple);
   if (gl->pipelines.snow_simple)
      glDeleteProgram(gl->pipelines.snow_simple);
   if (gl->pipelines.snow)
      glDeleteProgram(gl->pipelines.snow);
   if (gl->pipelines.bokeh)
      glDeleteProgram(gl->pipelines.bokeh);

#ifdef HAVE_OVERLAY
   gl3_free_overlay(gl);
   gl3_free_scratch_vbos(gl);
#endif
   gl3_deinit_fences(gl);
   gl3_deinit_pbo_readback(gl);
   gl3_deinit_hw_render(gl);
}

static bool gl3_init_hw_render(gl3_t *gl, unsigned width, unsigned height)
{
   GLint max_fbo_size;
   GLint max_rb_size;
   GLenum status;
   struct retro_hw_render_callback *hwr = video_driver_get_hw_context();

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, true);

   RARCH_LOG("[GLCore]: Initializing HW render (%ux%u).\n", width, height);
   glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_fbo_size);
   glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &max_rb_size);
   RARCH_LOG("[GLCore]: Max texture size: %d px, renderbuffer size: %d px.\n",
             max_fbo_size, max_rb_size);

   if (width > (unsigned)max_fbo_size)
       width = max_fbo_size;
   if (width > (unsigned)max_rb_size)
       width = max_rb_size;
   if (height > (unsigned)max_fbo_size)
       height = max_fbo_size;
   if (height > (unsigned)max_rb_size)
       height = max_rb_size;

   glGenFramebuffers(1, &gl->hw_render_fbo);
   glBindFramebuffer(GL_FRAMEBUFFER, gl->hw_render_fbo);
   glGenTextures(1, &gl->hw_render_texture);
   glBindTexture(GL_TEXTURE_2D, gl->hw_render_texture);
   glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gl->hw_render_texture, 0);

   gl->hw_render_rb_ds = 0;
   gl->hw_render_bottom_left = hwr->bottom_left_origin;
   if (hwr->depth)
   {
      glGenRenderbuffers(1, &gl->hw_render_rb_ds);
      glBindRenderbuffer(GL_RENDERBUFFER, gl->hw_render_rb_ds);
      glRenderbufferStorage(GL_RENDERBUFFER, hwr->stencil ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT16,
                            width, height);
      glBindRenderbuffer(GL_RENDERBUFFER, 0);

      if (hwr->stencil)
         glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gl->hw_render_rb_ds);
      else
         glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gl->hw_render_rb_ds);
   }

   status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
   if (status != GL_FRAMEBUFFER_COMPLETE)
   {
      RARCH_ERR("[GLCore]: Framebuffer is not complete.\n");
      if (gl->use_shared_context)
         gl->ctx_driver->bind_hw_render(gl->ctx_data, false);
      return false;
   }

   if (hwr->depth && hwr->stencil)
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   else if (hwr->depth)
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   else
      glClear(GL_COLOR_BUFFER_BIT);

   gl->hw_render_enable     = true;
   gl->hw_render_max_width  = width;
   gl->hw_render_max_height = height;
   glBindTexture(GL_TEXTURE_2D, 0);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, false);

   return true;
}

static const gfx_ctx_driver_t *gl3_get_context(gl3_t *gl)
{
   unsigned major;
   unsigned minor;
   enum gfx_ctx_api api;
   gfx_ctx_flags_t flags;
   const gfx_ctx_driver_t *gfx_ctx      = NULL;
   void                      *ctx_data  = NULL;
   settings_t                 *settings = config_get_ptr();
   struct retro_hw_render_callback *hwr = video_driver_get_hw_context();

#ifdef HAVE_OPENGLES3
   api = GFX_CTX_OPENGL_ES_API;
   major = 3;
   minor = 0;
   if (hwr && hwr->context_type == RETRO_HW_CONTEXT_OPENGLES_VERSION)
   {
      major = hwr->version_major;
      minor = hwr->version_minor;
   }
#else
   api   = GFX_CTX_OPENGL_API;
   if (hwr && hwr->context_type != RETRO_HW_CONTEXT_NONE)
   {
      major = hwr->version_major;
      minor = hwr->version_minor;
      gl_query_core_context_set(hwr->context_type == RETRO_HW_CONTEXT_OPENGL_CORE);
      if (hwr->context_type == RETRO_HW_CONTEXT_OPENGL_CORE)
      {
         flags.flags = 0;
         BIT32_SET(flags.flags, GFX_CTX_FLAGS_GL_CORE_CONTEXT);
         video_context_driver_set_flags(&flags);
      }
   }
   else
   {
      major = 3;
      minor = 2;
      gl_query_core_context_set(true);
      flags.flags = 0;
      BIT32_SET(flags.flags, GFX_CTX_FLAGS_GL_CORE_CONTEXT);
      video_context_driver_set_flags(&flags);
   }
#endif

   /* Force shared context. */
   if (hwr)
      gl->use_shared_context = hwr->context_type != RETRO_HW_CONTEXT_NONE;

   gfx_ctx = video_context_driver_init_first(gl,
         settings->arrays.video_context_driver,
         api, major, minor, gl->use_shared_context, &ctx_data);

   if (ctx_data)
      gl->ctx_data = ctx_data;

   /* Need to force here since video_context_driver_init also checks for global option. */
   if (gfx_ctx->bind_hw_render)
      gfx_ctx->bind_hw_render(ctx_data, gl->use_shared_context);
   return gfx_ctx;
}

static void gl3_set_projection(gl3_t *gl,
      const struct video_ortho *ortho, bool allow_rotate)
{
   static math_matrix_4x4 rot     = {
      { 0.0f,     0.0f,    0.0f,    0.0f ,
        0.0f,     0.0f,    0.0f,    0.0f ,
        0.0f,     0.0f,    0.0f,    0.0f ,
        0.0f,     0.0f,    0.0f,    1.0f }
   };
   float radians, cosine, sine;

   /* Calculate projection. */
   matrix_4x4_ortho(gl->mvp_no_rot, ortho->left, ortho->right,
                    ortho->bottom, ortho->top, ortho->znear, ortho->zfar);

   if (!allow_rotate)
   {
      gl->mvp = gl->mvp_no_rot;
      return;
   }

   radians                 = M_PI * gl->rotation / 180.0f;
   cosine                  = cosf(radians);
   sine                    = sinf(radians);
   MAT_ELEM_4X4(rot, 0, 0) = cosine;
   MAT_ELEM_4X4(rot, 0, 1) = -sine;
   MAT_ELEM_4X4(rot, 1, 0) = sine;
   MAT_ELEM_4X4(rot, 1, 1) = cosine;
   matrix_4x4_multiply(gl->mvp, rot, gl->mvp_no_rot);

   memcpy(gl->mvp_no_rot_yflip.data, gl->mvp_no_rot.data, sizeof(gl->mvp_no_rot.data));
   MAT_ELEM_4X4(gl->mvp_no_rot_yflip, 1, 0) *= -1.0f;
   MAT_ELEM_4X4(gl->mvp_no_rot_yflip, 1, 1) *= -1.0f;
   MAT_ELEM_4X4(gl->mvp_no_rot_yflip, 1, 2) *= -1.0f;
   MAT_ELEM_4X4(gl->mvp_no_rot_yflip, 1, 3) *= -1.0f;

   memcpy(gl->mvp_yflip.data, gl->mvp.data, sizeof(gl->mvp.data));
   MAT_ELEM_4X4(gl->mvp_yflip, 1, 0) *= -1.0f;
   MAT_ELEM_4X4(gl->mvp_yflip, 1, 1) *= -1.0f;
   MAT_ELEM_4X4(gl->mvp_yflip, 1, 2) *= -1.0f;
   MAT_ELEM_4X4(gl->mvp_yflip, 1, 3) *= -1.0f;
}

static void gl3_set_viewport(gl3_t *gl,
      unsigned viewport_width,
      unsigned viewport_height,
      bool force_full, bool allow_rotate)
{
   unsigned height                 = gl->video_height;
   int x                           = 0;
   int y                           = 0;
   settings_t *settings            = config_get_ptr();
   float device_aspect             = (float)viewport_width / viewport_height;
   bool video_scale_integer        = settings->bools.video_scale_integer;
   unsigned video_aspect_ratio_idx = settings->uints.video_aspect_ratio_idx;

   if (gl->ctx_driver->translate_aspect)
      device_aspect         = gl->ctx_driver->translate_aspect(
            gl->ctx_data, viewport_width, viewport_height);

   if (video_scale_integer && !force_full)
   {
      video_viewport_get_scaled_integer(&gl->vp,
            viewport_width, viewport_height,
            video_driver_get_aspect_ratio(), gl->keep_aspect);
      viewport_width  = gl->vp.width;
      viewport_height = gl->vp.height;
   }
   else if (gl->keep_aspect && !force_full)
   {
      float desired_aspect = video_driver_get_aspect_ratio();

#if defined(HAVE_MENU)
      if (video_aspect_ratio_idx == ASPECT_RATIO_CUSTOM)
      {
         const struct video_viewport *custom = video_viewport_get_custom();
         /* GL has bottom-left origin viewport. */
         x               = custom->x;
         y               = height - custom->y - custom->height;
         viewport_width  = custom->width;
         viewport_height = custom->height;
      }
      else
#endif
      {
         float delta;

         if (fabsf(device_aspect - desired_aspect) < 0.0001f)
         {
            /* If the aspect ratios of screen and desired aspect
             * ratio are sufficiently equal (floating point stuff),
             * assume they are actually equal.
             */
         }
         else if (device_aspect > desired_aspect)
         {
            delta = (desired_aspect / device_aspect - 1.0f) / 2.0f + 0.5f;
            x     = (int)roundf(viewport_width * (0.5f - delta));
            viewport_width = (unsigned)roundf(2.0f * viewport_width * delta);
         }
         else
         {
            delta  = (device_aspect / desired_aspect - 1.0f) / 2.0f + 0.5f;
            y      = (int)roundf(viewport_height * (0.5f - delta));
            viewport_height = (unsigned)roundf(2.0f * viewport_height * delta);
         }
      }

      gl->vp.x      = x;
      gl->vp.y      = y;
      gl->vp.width  = viewport_width;
      gl->vp.height = viewport_height;
   }
   else
   {
      gl->vp.x      = gl->vp.y = 0;
      gl->vp.width  = viewport_width;
      gl->vp.height = viewport_height;
   }

#if defined(RARCH_MOBILE)
   /* In portrait mode, we want viewport to gravitate to top of screen. */
   if (device_aspect < 1.0f)
      gl->vp.y *= 2;
#endif

   glViewport(gl->vp.x, gl->vp.y, gl->vp.width, gl->vp.height);
   gl3_set_projection(gl, &gl3_default_ortho, allow_rotate);

   /* Set last backbuffer viewport. */
   if (!force_full)
   {
      gl->vp_out_width  = viewport_width;
      gl->vp_out_height = viewport_height;
   }

   gl->filter_chain_vp.x = gl->vp.x;
   gl->filter_chain_vp.y = gl->vp.y;
   gl->filter_chain_vp.width = gl->vp.width;
   gl->filter_chain_vp.height = gl->vp.height;

#if 0
   RARCH_LOG("Setting viewport @ %ux%u\n", viewport_width, viewport_height);
#endif
}

static bool gl3_init_pipelines(gl3_t *gl)
{
   static const uint32_t alpha_blend_vert[] =
#include "vulkan_shaders/alpha_blend.vert.inc"
      ;

   static const uint32_t alpha_blend_frag[] =
#include "vulkan_shaders/alpha_blend.frag.inc"
      ;

   static const uint32_t font_frag[] =
#include "vulkan_shaders/font.frag.inc"
      ;

   static const uint32_t pipeline_ribbon_vert[] =
#include "vulkan_shaders/pipeline_ribbon.vert.inc"
      ;

   static const uint32_t pipeline_ribbon_frag[] =
#include "vulkan_shaders/pipeline_ribbon.frag.inc"
      ;

   static const uint32_t pipeline_ribbon_simple_vert[] =
#include "vulkan_shaders/pipeline_ribbon_simple.vert.inc"
      ;

   static const uint32_t pipeline_ribbon_simple_frag[] =
#include "vulkan_shaders/pipeline_ribbon_simple.frag.inc"
      ;

   static const uint32_t pipeline_snow_simple_frag[] =
#include "vulkan_shaders/pipeline_snow_simple.frag.inc"
      ;

   static const uint32_t pipeline_snow_frag[] =
#include "vulkan_shaders/pipeline_snow.frag.inc"
      ;

   static const uint32_t pipeline_bokeh_frag[] =
#include "vulkan_shaders/pipeline_bokeh.frag.inc"
      ;

   gl->pipelines.alpha_blend = gl3_cross_compile_program(alpha_blend_vert, sizeof(alpha_blend_vert),
                                                             alpha_blend_frag, sizeof(alpha_blend_frag),
                                                             &gl->pipelines.alpha_blend_loc, true);
   if (!gl->pipelines.alpha_blend)
      return false;

   gl->pipelines.font = gl3_cross_compile_program(alpha_blend_vert, sizeof(alpha_blend_vert),
                                                      font_frag, sizeof(font_frag),
                                                      &gl->pipelines.font_loc, true);
   if (!gl->pipelines.font)
      return false;

   gl->pipelines.ribbon_simple = gl3_cross_compile_program(pipeline_ribbon_simple_vert, sizeof(pipeline_ribbon_simple_vert),
                                                               pipeline_ribbon_simple_frag, sizeof(pipeline_ribbon_simple_frag),
                                                               &gl->pipelines.ribbon_simple_loc, true);
   if (!gl->pipelines.ribbon_simple)
      return false;

   gl->pipelines.ribbon = gl3_cross_compile_program(pipeline_ribbon_vert, sizeof(pipeline_ribbon_vert),
                                                        pipeline_ribbon_frag, sizeof(pipeline_ribbon_frag),
                                                        &gl->pipelines.ribbon_loc, true);
   if (!gl->pipelines.ribbon)
      return false;

   gl->pipelines.bokeh = gl3_cross_compile_program(alpha_blend_vert, sizeof(alpha_blend_vert),
                                                       pipeline_bokeh_frag, sizeof(pipeline_bokeh_frag),
                                                       &gl->pipelines.bokeh_loc, true);
   if (!gl->pipelines.bokeh)
      return false;

   gl->pipelines.snow_simple = gl3_cross_compile_program(alpha_blend_vert, sizeof(alpha_blend_vert),
                                                             pipeline_snow_simple_frag, sizeof(pipeline_snow_simple_frag),
                                                             &gl->pipelines.snow_simple_loc, true);
   if (!gl->pipelines.snow_simple)
      return false;

   gl->pipelines.snow = gl3_cross_compile_program(alpha_blend_vert, sizeof(alpha_blend_vert),
                                                      pipeline_snow_frag, sizeof(pipeline_snow_frag),
                                                      &gl->pipelines.snow_loc, true);
   if (!gl->pipelines.snow)
      return false;

   return true;
}

static bool gl3_init_default_filter_chain(gl3_t *gl)
{
   if (!gl->ctx_driver)
      return false;

   gl->filter_chain = gl3_filter_chain_create_default(
         gl->video_info.smooth 
         ? GLSLANG_FILTER_CHAIN_LINEAR 
         : GLSLANG_FILTER_CHAIN_NEAREST);

   if (!gl->filter_chain)
   {
      RARCH_ERR("Failed to create filter chain.\n");
      return false;
   }

   return true;
}

static bool gl3_init_filter_chain_preset(gl3_t *gl, const char *shader_path)
{
   gl->filter_chain = gl3_filter_chain_create_from_preset(
         shader_path,
         gl->video_info.smooth
         ? GLSLANG_FILTER_CHAIN_LINEAR 
         : GLSLANG_FILTER_CHAIN_NEAREST);

   if (!gl->filter_chain)
   {
      RARCH_ERR("[GLCore]: Failed to create preset: \"%s\".\n", shader_path);
      return false;
   }

   return true;
}

static bool gl3_init_filter_chain(gl3_t *gl)
{
   const char *shader_path     = video_shader_get_current_shader_preset();
   enum rarch_shader_type type = video_shader_parse_type(shader_path);

   if (string_is_empty(shader_path))
   {
      RARCH_LOG("[GLCore]: Loading stock shader.\n");
      return gl3_init_default_filter_chain(gl);
   }

   if (type != RARCH_SHADER_SLANG)
   {
      RARCH_WARN("[GLCore]: Only Slang shaders are supported, falling back to stock.\n");
      return gl3_init_default_filter_chain(gl);
   }

   if (!gl3_init_filter_chain_preset(gl, shader_path))
      gl3_init_default_filter_chain(gl);

   return true;
}

#ifdef GL_DEBUG
#ifdef HAVE_OPENGLES3
#define DEBUG_CALLBACK_TYPE GL_APIENTRY
#define GL_DEBUG_SOURCE_API GL_DEBUG_SOURCE_API_KHR
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM GL_DEBUG_SOURCE_WINDOW_SYSTEM_KHR
#define GL_DEBUG_SOURCE_SHADER_COMPILER GL_DEBUG_SOURCE_SHADER_COMPILER_KHR
#define GL_DEBUG_SOURCE_THIRD_PARTY GL_DEBUG_SOURCE_THIRD_PARTY_KHR
#define GL_DEBUG_SOURCE_APPLICATION GL_DEBUG_SOURCE_APPLICATION_KHR
#define GL_DEBUG_SOURCE_OTHER GL_DEBUG_SOURCE_OTHER_KHR
#define GL_DEBUG_TYPE_ERROR GL_DEBUG_TYPE_ERROR_KHR
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_KHR
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_KHR
#define GL_DEBUG_TYPE_PORTABILITY GL_DEBUG_TYPE_PORTABILITY_KHR
#define GL_DEBUG_TYPE_PERFORMANCE GL_DEBUG_TYPE_PERFORMANCE_KHR
#define GL_DEBUG_TYPE_MARKER GL_DEBUG_TYPE_MARKER_KHR
#define GL_DEBUG_TYPE_PUSH_GROUP GL_DEBUG_TYPE_PUSH_GROUP_KHR
#define GL_DEBUG_TYPE_POP_GROUP GL_DEBUG_TYPE_POP_GROUP_KHR
#define GL_DEBUG_TYPE_OTHER GL_DEBUG_TYPE_OTHER_KHR
#define GL_DEBUG_SEVERITY_HIGH GL_DEBUG_SEVERITY_HIGH_KHR
#define GL_DEBUG_SEVERITY_MEDIUM GL_DEBUG_SEVERITY_MEDIUM_KHR
#define GL_DEBUG_SEVERITY_LOW GL_DEBUG_SEVERITY_LOW_KHR
#else
#define DEBUG_CALLBACK_TYPE APIENTRY
#endif
static void DEBUG_CALLBACK_TYPE gl3_debug_cb(GLenum source, GLenum type,
      GLuint id, GLenum severity, GLsizei length,
      const GLchar *message, void *userParam)
{
   const char      *src = NULL;
   const char *typestr  = NULL;
   gl3_t *gl = (gl3_t*)userParam; /* Useful for debugger. */

   (void)gl;
   (void)id;
   (void)length;

   switch (source)
   {
      case GL_DEBUG_SOURCE_API:
         src = "API";
         break;
      case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
         src = "Window system";
         break;
      case GL_DEBUG_SOURCE_SHADER_COMPILER:
         src = "Shader compiler";
         break;
      case GL_DEBUG_SOURCE_THIRD_PARTY:
         src = "3rd party";
         break;
      case GL_DEBUG_SOURCE_APPLICATION:
         src = "Application";
         break;
      case GL_DEBUG_SOURCE_OTHER:
         src = "Other";
         break;
      default:
         src = "Unknown";
         break;
   }

   switch (type)
   {
      case GL_DEBUG_TYPE_ERROR:
         typestr = "Error";
         break;
      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
         typestr = "Deprecated behavior";
         break;
      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
         typestr = "Undefined behavior";
         break;
      case GL_DEBUG_TYPE_PORTABILITY:
         typestr = "Portability";
         break;
      case GL_DEBUG_TYPE_PERFORMANCE:
         typestr = "Performance";
         break;
      case GL_DEBUG_TYPE_MARKER:
         typestr = "Marker";
         break;
      case GL_DEBUG_TYPE_PUSH_GROUP:
         typestr = "Push group";
         break;
      case GL_DEBUG_TYPE_POP_GROUP:
        typestr = "Pop group";
        break;
      case GL_DEBUG_TYPE_OTHER:
        typestr = "Other";
        break;
      default:
        typestr = "Unknown";
        break;
   }

   switch (severity)
   {
      case GL_DEBUG_SEVERITY_HIGH:
         RARCH_ERR("[GL debug (High, %s, %s)]: %s\n", src, typestr, message);
         break;
      case GL_DEBUG_SEVERITY_MEDIUM:
         RARCH_WARN("[GL debug (Medium, %s, %s)]: %s\n", src, typestr, message);
         break;
      case GL_DEBUG_SEVERITY_LOW:
         RARCH_LOG("[GL debug (Low, %s, %s)]: %s\n", src, typestr, message);
         break;
   }
}

static void gl3_begin_debug(gl3_t *gl)
{
   if (gl_check_capability(GL_CAPS_DEBUG))
   {
#ifdef HAVE_OPENGLES3
      glDebugMessageCallbackKHR(gl3_debug_cb, gl);
      glDebugMessageControlKHR(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);
      glEnable(GL_DEBUG_OUTPUT_KHR);
#else
      glDebugMessageCallback(gl3_debug_cb, gl);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glEnable(GL_DEBUG_OUTPUT);
#endif
   }
   else
      RARCH_ERR("[GLCore]: Neither GL_KHR_debug nor GL_ARB_debug_output are implemented. Cannot start GL debugging.\n");
}
#endif

static void gl3_set_viewport_wrapper(void *data,
      unsigned viewport_width,
      unsigned viewport_height, bool force_full, bool allow_rotate)
{
   gl3_t *gl = (gl3_t*)data;
   gl3_set_viewport(gl,
         viewport_width, viewport_height, force_full, allow_rotate);
}


static void *gl3_init(const video_info_t *video,
      input_driver_t **input, void **input_data)
{
   unsigned full_x, full_y;
   settings_t *settings                 = config_get_ptr();
   bool video_gpu_record                = settings->bools.video_gpu_record;
   int interval                         = 0;
   unsigned mode_width                  = 0;
   unsigned mode_height                 = 0;
   unsigned win_width                   = 0;
   unsigned win_height                  = 0;
   unsigned temp_width                  = 0;
   unsigned temp_height                 = 0;
   const char *vendor                   = NULL;
   const char *renderer                 = NULL;
   const char *version                  = NULL;
   char *error_string                   = NULL;
   gl3_t *gl                        = (gl3_t*)calloc(1, sizeof(gl3_t));
   const gfx_ctx_driver_t *ctx_driver   = gl3_get_context(gl);
   struct retro_hw_render_callback *hwr = video_driver_get_hw_context();

   if (!gl || !ctx_driver)
      goto error;

   video_context_driver_set(ctx_driver);

   gl->ctx_driver = ctx_driver;
   gl->video_info = *video;

   RARCH_LOG("[GLCore]: Found GL context: \"%s\".\n", ctx_driver->ident);

   if (gl->ctx_driver->get_video_size)
      gl->ctx_driver->get_video_size(gl->ctx_data,
               &mode_width, &mode_height);

   full_x      = mode_width;
   full_y      = mode_height;
   mode_width  = 0;
   mode_height = 0;
   interval    = 0;

   RARCH_LOG("[GLCore]: Detecting screen resolution: %ux%u.\n", full_x, full_y);

   if (video->vsync)
      interval = video->swap_interval;

   if (gl->ctx_driver->swap_interval)
   {
      bool adaptive_vsync_enabled            = video_driver_test_all_flags(
            GFX_CTX_FLAGS_ADAPTIVE_VSYNC) && video->adaptive_vsync;
      if (adaptive_vsync_enabled && interval == 1)
         interval = -1;
      gl->ctx_driver->swap_interval(gl->ctx_data, interval);
   }

   win_width   = video->width;
   win_height  = video->height;

   if (video->fullscreen && (win_width == 0) && (win_height == 0))
   {
      win_width  = full_x;
      win_height = full_y;
   }

   if (     !gl->ctx_driver->set_video_mode
         || !gl->ctx_driver->set_video_mode(gl->ctx_data,
            win_width, win_height, video->fullscreen))
      goto error;

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, false);

   rglgen_resolve_symbols(ctx_driver->get_proc_address);

   if (hwr && hwr->context_type != RETRO_HW_CONTEXT_NONE)
      gl3_init_hw_render(gl, RARCH_SCALE_BASE * video->input_scale, RARCH_SCALE_BASE * video->input_scale);

#ifdef GL_DEBUG
   gl3_begin_debug(gl);
   if (gl->use_shared_context)
   {
      if (gl->hw_render_enable)
      {
         gl->ctx_driver->bind_hw_render(gl->ctx_data, true);
         gl3_begin_debug(gl);
         gl->ctx_driver->bind_hw_render(gl->ctx_data, false);
      }
   }
   else
   {
      if (gl->hw_render_enable)
         gl3_begin_debug(gl);
   }
#endif

   /* Clear out potential error flags in case we use cached context. */
   glGetError();

   vendor   = (const char*)glGetString(GL_VENDOR);
   renderer = (const char*)glGetString(GL_RENDERER);
   version  = (const char*)glGetString(GL_VERSION);

   RARCH_LOG("[GLCore]: Vendor: %s, Renderer: %s.\n", vendor, renderer);
   RARCH_LOG("[GLCore]: Version: %s.\n", version);

   if (string_is_equal(ctx_driver->ident, "null"))
      goto error;

   if (!gl3_init_pipelines(gl))
   {
      RARCH_ERR("[GLCore]: Failed to cross-compile menu pipelines.\n");
      goto error;
   }

   if (!string_is_empty(version))
      sscanf(version, "%u.%u", &gl->version_major, &gl->version_minor);

   {
      char device_str[128];
      size_t len        = strlcpy(device_str, vendor, sizeof(device_str));
      device_str[len  ] = ' ';
      device_str[len+1] = '\0';
      strlcat(device_str, renderer, sizeof(device_str));

      video_driver_set_gpu_device_string(device_str);
      video_driver_set_gpu_api_version_string(version);
   }

#ifdef _WIN32
   if (   string_is_equal(vendor,   "Microsoft Corporation"))
      if (string_is_equal(renderer, "GDI Generic"))
#ifdef HAVE_OPENGL1
         video_driver_force_fallback("gl1");
#else
         video_driver_force_fallback("gdi");
#endif
#endif

   gl->vsync       = video->vsync;
   gl->fullscreen  = video->fullscreen;
   gl->keep_aspect = video->force_aspect;

   mode_width     = 0;
   mode_height    = 0;

   if (gl->ctx_driver->get_video_size)
      gl->ctx_driver->get_video_size(gl->ctx_data,
               &mode_width, &mode_height);

   temp_width     = mode_width;
   temp_height    = mode_height;

   /* Get real known video size, which might have been altered by context. */

   if (temp_width != 0 && temp_height != 0)
      video_driver_set_size(temp_width, temp_height);
   video_driver_get_size(&temp_width, &temp_height);
   gl->video_width  = temp_width;
   gl->video_height = temp_height;

   RARCH_LOG("[GLCore]: Using resolution %ux%u.\n", temp_width, temp_height);

   /* Set the viewport to fix recording, since it needs to know
    * the viewport sizes before we start running. */
   gl3_set_viewport_wrapper(gl, temp_width, temp_height, false, true);

   if (gl->ctx_driver->input_driver)
   {
      const char *joypad_name = settings->arrays.input_joypad_driver;
      gl->ctx_driver->input_driver(
            gl->ctx_data, joypad_name,
            input, input_data);
   }

   if (!gl3_init_filter_chain(gl))
   {
      RARCH_ERR("[GLCore]: Failed to init filter chain.\n");
      goto error;
   }

   if (video->font_enable)
   {
      font_driver_init_osd(gl,
            video,
            false,
            video->is_threaded,
            FONT_DRIVER_RENDER_OPENGL_CORE_API);
   }

   gl->pbo_readback_enable = video_gpu_record 
      && recording_state_get_ptr()->enable;

   if (gl->pbo_readback_enable && gl3_init_pbo_readback(gl))
   {
      RARCH_LOG("[GLCore]: Async PBO readback enabled.\n");
   }

   if (!gl_check_error(&error_string))
   {
      RARCH_ERR("%s\n", error_string);
      free(error_string);
      goto error;
   }

   glGenVertexArrays(1, &gl->vao);
   glBindVertexArray(gl->vao);
   glBindVertexArray(0);

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, true);
   return gl;

error:
   video_context_driver_free();
   gl3_destroy_resources(gl);
   if (gl)
      free(gl);
   return NULL;
}

static unsigned gl3_num_miplevels(unsigned width, unsigned height)
{
   unsigned levels = 1;
   if (width < height)
      width = height;
   while (width > 1)
   {
      levels++;
      width >>= 1;
   }
   return levels;
}

static void video_texture_load_gl3(
      const struct texture_image *ti,
      enum texture_filter_type filter_type,
      GLuint *idptr)
{
   /* Generate the OpenGL texture object */
   GLuint id;
   unsigned levels;
   GLenum mag_filter, min_filter;

   glGenTextures(1, &id);
   *idptr = id;
   glBindTexture(GL_TEXTURE_2D, id);

   levels = 1;
   if (filter_type == TEXTURE_FILTER_MIPMAP_LINEAR || filter_type == TEXTURE_FILTER_MIPMAP_NEAREST)
      levels = gl3_num_miplevels(ti->width, ti->height);

   glTexStorage2D(GL_TEXTURE_2D, levels, GL_RGBA8, ti->width, ti->height);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   switch (filter_type)
   {
      case TEXTURE_FILTER_LINEAR:
         mag_filter = GL_LINEAR;
         min_filter = GL_LINEAR;
         break;

      case TEXTURE_FILTER_NEAREST:
         mag_filter = GL_NEAREST;
         min_filter = GL_NEAREST;
         break;

      case TEXTURE_FILTER_MIPMAP_NEAREST:
         mag_filter = GL_LINEAR;
         min_filter = GL_LINEAR_MIPMAP_NEAREST;
         break;

      case TEXTURE_FILTER_MIPMAP_LINEAR:
	  default:
         mag_filter = GL_LINEAR;
         min_filter = GL_LINEAR_MIPMAP_LINEAR;
         break;
   }
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
   glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
   glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                   ti->width, ti->height, GL_RGBA, GL_UNSIGNED_BYTE, ti->pixels);

   if (levels > 1)
      glGenerateMipmap(GL_TEXTURE_2D);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_BLUE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
   glBindTexture(GL_TEXTURE_2D, 0);
}

#ifdef HAVE_OVERLAY
static bool gl3_overlay_load(void *data,
      const void *image_data, unsigned num_images)
{
   unsigned i, j;
   GLuint id;
   gl3_t *gl = (gl3_t*)data;
   const struct texture_image *images =
      (const struct texture_image*)image_data;

   if (!gl)
      return false;

   gl3_free_overlay(gl);
   gl->overlay_tex = (GLuint*)
      calloc(num_images, sizeof(*gl->overlay_tex));

   if (!gl->overlay_tex)
      return false;

   gl->overlay_vertex_coord = (GLfloat*)
      calloc(2 * 4 * num_images, sizeof(GLfloat));
   gl->overlay_tex_coord    = (GLfloat*)
      calloc(2 * 4 * num_images, sizeof(GLfloat));
   gl->overlay_color_coord  = (GLfloat*)
      calloc(4 * 4 * num_images, sizeof(GLfloat));

   if (     !gl->overlay_vertex_coord
         || !gl->overlay_tex_coord
         || !gl->overlay_color_coord)
      return false;

   gl->overlays = num_images;
   glGenTextures(num_images, gl->overlay_tex);

   for (i = 0; i < num_images; i++)
   {
      video_texture_load_gl3(&images[i], TEXTURE_FILTER_LINEAR, &id);
      gl->overlay_tex[i] = id;

      /* Default. Stretch to whole screen. */
      gl3_overlay_tex_geom(gl, i, 0, 0, 1, 1);
      gl3_overlay_vertex_geom(gl, i, 0, 0, 1, 1);

      for (j = 0; j < 16; j++)
         gl->overlay_color_coord[16 * i + j] = 1.0f;
   }

   return true;
}

static void gl3_overlay_enable(void *data, bool state)
{
   gl3_t *gl = (gl3_t*)data;

   if (!gl)
      return;

   gl->overlay_enable = state;

   if (gl->fullscreen && gl->ctx_driver->show_mouse)
      gl->ctx_driver->show_mouse(gl->ctx_data, state);
}

static void gl3_overlay_full_screen(void *data, bool enable)
{
   gl3_t *gl = (gl3_t*)data;
   if (gl)
      gl->overlay_full_screen = enable;
}

static void gl3_overlay_set_alpha(void *data, unsigned image, float mod)
{
   GLfloat *color = NULL;
   gl3_t *gl = (gl3_t*)data;
   if (!gl)
      return;

   color          = (GLfloat*)&gl->overlay_color_coord[image * 16];

   color[ 0 + 3]  = mod;
   color[ 4 + 3]  = mod;
   color[ 8 + 3]  = mod;
   color[12 + 3]  = mod;
}

static const video_overlay_interface_t gl3_overlay_interface = {
   gl3_overlay_enable,
   gl3_overlay_load,
   gl3_overlay_tex_geom,
   gl3_overlay_vertex_geom,
   gl3_overlay_full_screen,
   gl3_overlay_set_alpha,
};

static void gl3_get_overlay_interface(void *data,
      const video_overlay_interface_t **iface)
{
   (void)data;
   *iface = &gl3_overlay_interface;
}
#endif

static void gl3_free(void *data)
{
   gl3_t *gl = (gl3_t*)data;
   if (!gl)
      return;

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, false);
   font_driver_free_osd();
   gl3_destroy_resources(gl);
   if (gl->ctx_driver && gl->ctx_driver->destroy)
      gl->ctx_driver->destroy(gl->ctx_data);
   video_context_driver_free();
   free(gl);
}

static bool gl3_alive(void *data)
{
   bool ret             = false;
   bool quit            = false;
   bool resize          = false;
   gl3_t *gl        = (gl3_t*)data;
   unsigned temp_width  = gl->video_width;
   unsigned temp_height = gl->video_height;

   gl->ctx_driver->check_window(gl->ctx_data,
         &quit, &resize, &temp_width, &temp_height);

#ifdef __WINRT__
   if (is_running_on_xbox())
   {
      //we can set it to 1920x1080 as xbox uwp windowsize is guaranteed to be 1920x1080 and currently there is now way to set angle to use a variable resolution swapchain so regardless of the size the window is always 1080p
      temp_width = 1920;
      temp_height = 1080;
   }
#endif

   if (quit)
      gl->quitting = true;
   else if (resize)
      gl->should_resize = true;

   ret = !gl->quitting;

   if (temp_width != 0 && temp_height != 0)
   {
      video_driver_set_size(temp_width, temp_height);
      gl->video_width  = temp_width;
      gl->video_height = temp_height;
   }

   return ret;
}

static void gl3_set_nonblock_state(void *data, bool state,
      bool adaptive_vsync_enabled,
      unsigned swap_interval)
{
   int interval            = 0;
   gl3_t         *gl       = (gl3_t*)data;

   if (!gl)
      return;

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, false);
   if (!state)
      interval = swap_interval;

   if (gl->ctx_driver->swap_interval)
   {
      if (adaptive_vsync_enabled && interval == 1)
         interval = -1;
      gl->ctx_driver->swap_interval(gl->ctx_data, interval);
   }

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, true);
}

static bool gl3_suppress_screensaver(void *data, bool enable)
{
   bool enabled                = enable;
   gl3_t         *gl       = (gl3_t*)data;
   if (gl->ctx_data && gl->ctx_driver->suppress_screensaver)
      return gl->ctx_driver->suppress_screensaver(gl->ctx_data, enabled);
   return false;
}

static bool gl3_set_shader(void *data,
      enum rarch_shader_type type, const char *path)
{
   gl3_t *gl = (gl3_t *)data;
   if (!gl)
      return false;

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, false);

   if (gl->filter_chain)
      gl3_filter_chain_free(gl->filter_chain);
   gl->filter_chain = NULL;

   if (!string_is_empty(path) && type != RARCH_SHADER_SLANG)
   {
      RARCH_WARN("[GLCore]: Only Slang shaders are supported. Falling back to stock.\n");
      path = NULL;
   }

   if (string_is_empty(path))
   {
      gl3_init_default_filter_chain(gl);
      goto end;
   }

   if (!gl3_init_filter_chain_preset(gl, path))
   {
      RARCH_ERR("[GLCore]: Failed to create filter chain: \"%s\". Falling back to stock.\n", path);
      gl3_init_default_filter_chain(gl);
      if (gl->use_shared_context)
         gl->ctx_driver->bind_hw_render(gl->ctx_data, true);
      return false;
   }

end:
   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, true);
   return true;
}

static void gl3_set_rotation(void *data, unsigned rotation)
{
   gl3_t *gl = (gl3_t*)data;

   if (!gl)
      return;

   gl->rotation = video_driver_is_hw_context() && gl->hw_render_bottom_left ? 90 * rotation : 270 * rotation;
   gl3_set_projection(gl, &gl3_default_ortho, true);
}

static void gl3_viewport_info(void *data, struct video_viewport *vp)
{
   unsigned top_y, top_dist;
   gl3_t *gl   = (gl3_t*)data;
   unsigned width  = gl->video_width;
   unsigned height = gl->video_height;


   *vp             = gl->vp;
   vp->full_width  = width;
   vp->full_height = height;

   /* Adjust as GL viewport is bottom-up. */
   top_y           = vp->y + vp->height;
   top_dist        = height - top_y;
   vp->y           = top_dist;
}

static bool gl3_read_viewport(void *data, uint8_t *buffer, bool is_idle)
{
   gl3_t *gl = (gl3_t*)data;
   unsigned num_pixels = 0;

   if (!gl)
      return false;

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, false);
   num_pixels = gl->vp.width * gl->vp.height;

   if (gl->pbo_readback_enable)
   {
      const void *ptr = NULL;
      struct scaler_ctx *ctx = &gl->pbo_readback_scaler;

      /* Don't readback if we're in menu mode.
       * We haven't buffered up enough frames yet, come back later. */
      if (!gl->pbo_readback_valid[gl->pbo_readback_index])
         goto error;

      gl->pbo_readback_valid[gl->pbo_readback_index] = false;
      glBindBuffer(GL_PIXEL_PACK_BUFFER, gl->pbo_readback[gl->pbo_readback_index]);

      ptr = glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, num_pixels * sizeof(uint32_t), GL_MAP_READ_BIT);
      scaler_ctx_scale_direct(ctx, buffer, ptr);
      glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
      glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
   }
   else
   {
      /* Use slow synchronous readbacks. Use this with plain screenshots
         as we don't really care about performance in this case. */

      /* GLES only guarantees GL_RGBA/GL_UNSIGNED_BYTE
       * readbacks so do just that.
       * GLES also doesn't support reading back data
       * from front buffer, so render a cached frame
       * and have gl_frame() do the readback while it's
       * in the back buffer.
       *
       * Keep codepath similar for GLES and desktop GL.
       */
      gl->readback_buffer_screenshot = malloc(num_pixels * sizeof(uint32_t));

      if (!gl->readback_buffer_screenshot)
         goto error;

      if (!is_idle)
         video_driver_cached_frame();

      video_frame_convert_rgba_to_bgr(
            (const void*)gl->readback_buffer_screenshot,
            buffer,
            num_pixels);

      free(gl->readback_buffer_screenshot);
      gl->readback_buffer_screenshot = NULL;
   }

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, true);
   return true;

error:
   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, true);
   return false;
}

static void gl3_update_cpu_texture(gl3_t *gl,
      struct gl3_streamed_texture *streamed,
      const void *frame, unsigned width, unsigned height, unsigned pitch)
{
   if (width != streamed->width || height != streamed->height)
   {
      if (streamed->tex != 0)
         glDeleteTextures(1, &streamed->tex);
      glGenTextures(1, &streamed->tex);
      glBindTexture(GL_TEXTURE_2D, streamed->tex);
      glTexStorage2D(GL_TEXTURE_2D, 1,
            gl->video_info.rgb32 
            ? GL_RGBA8 
            : GL_RGB565,
            width, height);
      streamed->width = width;
      streamed->height = height;

      if (gl->video_info.rgb32)
      {
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_BLUE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
      }
   }
   else
      glBindTexture(GL_TEXTURE_2D, streamed->tex);

   glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
   if (gl->video_info.rgb32)
   {
      glPixelStorei(GL_UNPACK_ROW_LENGTH, pitch >> 2);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                      width, height, GL_RGBA, GL_UNSIGNED_BYTE, frame);
   }
   else
   {
      glPixelStorei(GL_UNPACK_ROW_LENGTH, pitch >> 1);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                      width, height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, frame);
   }
}

#if defined(HAVE_MENU)
static void gl3_draw_menu_texture(gl3_t *gl,
      unsigned width, unsigned height)
{
   const float vbo_data[32] = {
      0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, gl->menu_texture_alpha,
      1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, gl->menu_texture_alpha,
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, gl->menu_texture_alpha,
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, gl->menu_texture_alpha,
   };

   glEnable(GL_BLEND);
   glDisable(GL_CULL_FACE);
   glDisable(GL_DEPTH_TEST);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBlendEquation(GL_FUNC_ADD);

   if (gl->menu_texture_full_screen)
      glViewport(0, 0, width, height);
   else
      glViewport(gl->vp.x, gl->vp.y, gl->vp.width, gl->vp.height);

   glActiveTexture(GL_TEXTURE0 + 1);
   glBindTexture(GL_TEXTURE_2D, gl->menu_texture);

   glUseProgram(gl->pipelines.alpha_blend);
   if (gl->pipelines.alpha_blend_loc.flat_ubo_vertex >= 0)
      glUniform4fv(gl->pipelines.alpha_blend_loc.flat_ubo_vertex, 4, gl->mvp_no_rot_yflip.data);

   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);
   gl3_bind_scratch_vbo(gl, vbo_data, sizeof(vbo_data));
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
         8 * sizeof(float), (void *)(uintptr_t)0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
         8 * sizeof(float), (void *)(uintptr_t)(2 * sizeof(float)));
   glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
         8 * sizeof(float), (void *)(uintptr_t)(4 * sizeof(float)));
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);
   glDisableVertexAttribArray(2);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glDisable(GL_BLEND);
}
#endif

static bool gl3_frame(void *data, const void *frame,
      unsigned frame_width, unsigned frame_height,
      uint64_t frame_count,
      unsigned pitch, const char *msg,
      video_frame_info_t *video_info)
{
   struct gl3_filter_chain_texture texture;
   struct gl3_streamed_texture *streamed   = NULL;
   gl3_t *gl                               = (gl3_t*)data;
   unsigned width                          = video_info->width;
   unsigned height                         = video_info->height;
   struct font_params *osd_params          = (struct font_params*)
      &video_info->osd_stat_params;
   const char *stat_text                   = video_info->stat_text;
   bool statistics_show                    = video_info->statistics_show;
#if 0
   bool msg_bgcolor_enable                 = video_info->msg_bgcolor_enable;
#endif
   unsigned black_frame_insertion          = video_info->black_frame_insertion;

   unsigned hard_sync_frames               = video_info->hard_sync_frames;
   bool runloop_is_paused                  = video_info->runloop_is_paused;
   bool runloop_is_slowmotion              = video_info->runloop_is_slowmotion;
   bool input_driver_nonblock_state        = video_info->input_driver_nonblock_state;
#ifdef HAVE_MENU
   bool menu_is_alive                      = video_info->menu_is_alive;
#endif
#ifdef HAVE_GFX_WIDGETS
   bool widgets_active                     = video_info->widgets_active;
#endif
   bool hard_sync                          = video_info->hard_sync;
   bool overlay_behind_menu                = video_info->overlay_behind_menu;

   if (!gl)
      return false;

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, false);
   glBindVertexArray(gl->vao);

   if (frame)
      gl->textures_index = (gl->textures_index + 1) 
         & (GL_CORE_NUM_TEXTURES - 1);

   streamed = &gl->textures[gl->textures_index];
   if (frame)
   {
      if (!gl->hw_render_enable)
         gl3_update_cpu_texture(gl, streamed, frame,
               frame_width, frame_height, pitch);
      else
      {
         streamed->width    = frame_width;
         streamed->height   = frame_height;
      }
   }

   if (gl->should_resize)
   {
      if (gl->ctx_driver->set_resize)
         gl->ctx_driver->set_resize(gl->ctx_data,
               width, height);
      gl->should_resize     = false;
   }

   gl3_set_viewport(gl, width, height, false, true);

   texture.image            = 0;
   texture.width            = streamed->width;
   texture.height           = streamed->height;
   texture.padded_width     = 0;
   texture.padded_height    = 0;
   texture.format           = 0;
   if (gl->hw_render_enable)
   {
      texture.image         = gl->hw_render_texture;
      texture.format        = GL_RGBA8;
      texture.padded_width  = gl->hw_render_max_width;
      texture.padded_height = gl->hw_render_max_height;

	  if (texture.width == 0)
		  texture.width = 1;
	  if (texture.height == 0)
		  texture.height = 1;
   }
   else
   {
      texture.image         = streamed->tex;
      texture.format        = gl->video_info.rgb32 ? GL_RGBA8 : GL_RGB565;
      texture.padded_width  = streamed->width;
      texture.padded_height = streamed->height;
   }
   gl3_filter_chain_set_frame_count(gl->filter_chain, frame_count);
#ifdef HAVE_REWIND
   gl3_filter_chain_set_frame_direction(gl->filter_chain, state_manager_frame_is_reversed() ? -1 : 1);
#else
   gl3_filter_chain_set_frame_direction(gl->filter_chain, 1);
#endif
   gl3_filter_chain_set_input_texture(gl->filter_chain, &texture);
   gl3_filter_chain_build_offscreen_passes(gl->filter_chain,
         &gl->filter_chain_vp);

   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT);
   gl3_filter_chain_build_viewport_pass(gl->filter_chain,
         &gl->filter_chain_vp,
         gl->hw_render_bottom_left 
         ? gl->mvp.data 
         : gl->mvp_yflip.data);
   gl3_filter_chain_end_frame(gl->filter_chain);

#ifdef HAVE_OVERLAY
   if (gl->overlay_enable && overlay_behind_menu)
      gl3_render_overlay(gl, width, height);
#endif

#if defined(HAVE_MENU)
   if (gl->menu_texture_enable)
   {
      menu_driver_frame(menu_is_alive, video_info);
      if (gl->menu_texture_enable && gl->menu_texture)
         gl3_draw_menu_texture(gl, width, height);
   }
   else if (statistics_show)
   {
      if (osd_params)
         font_driver_render_msg(gl, stat_text,
               (const struct font_params*)osd_params, NULL);
   }
#endif

#ifdef HAVE_OVERLAY
   if (gl->overlay_enable && !overlay_behind_menu)
      gl3_render_overlay(gl, width, height);
#endif

#ifdef HAVE_GFX_WIDGETS
   if (widgets_active)
      gfx_widgets_frame(video_info);
#endif

   if (!string_is_empty(msg))
   {
#if 0
      if (msg_bgcolor_enable)
         gl3_render_osd_background(gl, video_info, msg);
#endif
      font_driver_render_msg(gl, msg, NULL, NULL);
   }

   if (gl->ctx_driver->update_window_title)
      gl->ctx_driver->update_window_title(gl->ctx_data);

   if (gl->readback_buffer_screenshot)
   {
      /* For screenshots, just do the regular slow readback. */
      gl3_slow_readback(gl, gl->readback_buffer_screenshot);
   }
   else if (gl->pbo_readback_enable)
   {
#ifdef HAVE_MENU
      /* Don't readback if we're in menu mode. */
      if (!gl->menu_texture_enable)
#endif
         gl3_pbo_async_readback(gl);
   }


   if (gl->ctx_driver->swap_buffers)
      gl->ctx_driver->swap_buffers(gl->ctx_data);

 /* Emscripten has to do black frame insertion in its main loop */
#ifndef EMSCRIPTEN
   /* Disable BFI during fast forward, slow-motion,
    * and pause to prevent flicker. */
    if (
         black_frame_insertion
         && !input_driver_nonblock_state
         && !runloop_is_slowmotion
         && !runloop_is_paused 
         && !gl->menu_texture_enable)
    {
        int n;
        for (n = 0; n < black_frame_insertion; ++n)
        {
          glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT);			

          if (gl->ctx_driver->swap_buffers)
            gl->ctx_driver->swap_buffers(gl->ctx_data);
        }  
    }   
#endif 

   if (    hard_sync
       && !input_driver_nonblock_state
       )
      gl3_fence_iterate(gl, hard_sync_frames);

   glBindVertexArray(0);
   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, true);
   return true;
}

static uint32_t gl3_get_flags(void *data)
{
   uint32_t flags = 0;

   BIT32_SET(flags, GFX_CTX_FLAGS_HARD_SYNC);
   BIT32_SET(flags, GFX_CTX_FLAGS_BLACK_FRAME_INSERTION);
   BIT32_SET(flags, GFX_CTX_FLAGS_MENU_FRAME_FILTERING);
   BIT32_SET(flags, GFX_CTX_FLAGS_SCREENSHOTS_SUPPORTED);
   BIT32_SET(flags, GFX_CTX_FLAGS_OVERLAY_BEHIND_MENU_SUPPORTED);

   return flags;
}

static float gl3_get_refresh_rate(void *data)
{
   float refresh_rate;
   if (video_context_driver_get_refresh_rate(&refresh_rate))
       return refresh_rate;
   return 0.0f;
}

static void gl3_set_aspect_ratio(void *data, unsigned aspect_ratio_idx)
{
   gl3_t *gl = (gl3_t*)data;

   if (!gl)
      return;

   gl->keep_aspect   = true;
   gl->should_resize = true;
}

static void gl3_apply_state_changes(void *data)
{
   gl3_t *gl = (gl3_t*)data;
   if (gl)
      gl->should_resize = true;
}

static struct video_shader *gl3_get_current_shader(void *data)
{
   gl3_t *gl = (gl3_t*)data;
   if (!gl || !gl->filter_chain)
      return NULL;

   return gl3_filter_chain_get_preset(gl->filter_chain);
}

#ifdef HAVE_THREADS
static int video_texture_load_wrap_gl3_mipmap(void *data)
{
   GLuint id = 0;

   if (!data)
      return 0;
   video_texture_load_gl3((struct texture_image*)data,
         TEXTURE_FILTER_MIPMAP_LINEAR, &id);
   return (int)id;
}

static int video_texture_load_wrap_gl3(void *data)
{
   GLuint id = 0;

   if (!data)
      return 0;
   video_texture_load_gl3((struct texture_image*)data,
         TEXTURE_FILTER_LINEAR, &id);
   return (int)id;
}
#endif

static uintptr_t gl3_load_texture(void *video_data, void *data,
      bool threaded, enum texture_filter_type filter_type)
{
   GLuint id = 0;

#ifdef HAVE_THREADS
   if (threaded)
   {
      gl3_t                    *gl = (gl3_t*)video_data;
      custom_command_method_t func = video_texture_load_wrap_gl3;

      if (gl->ctx_driver->make_current)
         gl->ctx_driver->make_current(false);

      switch (filter_type)
      {
         case TEXTURE_FILTER_MIPMAP_LINEAR:
         case TEXTURE_FILTER_MIPMAP_NEAREST:
            func = video_texture_load_wrap_gl3_mipmap;
            break;
         default:
            break;
      }
      return video_thread_texture_load(data, func);
   }
#endif

   video_texture_load_gl3((struct texture_image*)data, filter_type, &id);
   return id;
}

static void gl3_unload_texture(void *data, bool threaded,
      uintptr_t id)
{
   GLuint glid;
   gl3_t                *gl = (gl3_t*)data;
   if (!id)
      return;

#ifdef HAVE_THREADS
   if (threaded)
   {
      if (gl->ctx_driver->make_current)
         gl->ctx_driver->make_current(false);
   }
#endif

   glid = (GLuint)id;
   glDeleteTextures(1, &glid);
}

static void gl3_set_video_mode(void *data, unsigned width, unsigned height,
      bool fullscreen)
{
   gl3_t *gl = (gl3_t*)data;
   if (gl->ctx_driver->set_video_mode)
      gl->ctx_driver->set_video_mode(gl->ctx_data,
            width, height, fullscreen);
}

static void gl3_show_mouse(void *data, bool state)
{
   gl3_t *gl = (gl3_t*)data;
   if (gl && gl->ctx_driver->show_mouse)
      gl->ctx_driver->show_mouse(gl->ctx_data, state);
}

static void gl3_set_texture_frame(void *data,
      const void *frame, bool rgb32, unsigned width, unsigned height,
      float alpha)
{
   settings_t *settings = config_get_ptr();
   GLenum menu_filter   = settings->bools.menu_linear_filter 
      ? GL_LINEAR : GL_NEAREST;
   unsigned base_size   = rgb32 ? sizeof(uint32_t) : sizeof(uint16_t);
   gl3_t *gl            = (gl3_t*)data;
   if (!gl)
      return;

   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, false);

   if (gl->menu_texture)
      glDeleteTextures(1, &gl->menu_texture);
   glGenTextures(1, &gl->menu_texture);
   glBindTexture(GL_TEXTURE_2D, gl->menu_texture);
   glTexStorage2D(GL_TEXTURE_2D, 1, rgb32 
         ? GL_RGBA8 : GL_RGBA4, width, height);

   glPixelStorei(GL_UNPACK_ALIGNMENT, base_size);
   glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
   glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
   glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                   width, height, GL_RGBA, rgb32 
                   ? GL_UNSIGNED_BYTE 
                   : GL_UNSIGNED_SHORT_4_4_4_4, frame);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, menu_filter);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, menu_filter);

   if (rgb32)
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_BLUE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
   }

   glBindTexture(GL_TEXTURE_2D, 0);
   gl->menu_texture_alpha = alpha;
   if (gl->use_shared_context)
      gl->ctx_driver->bind_hw_render(gl->ctx_data, true);
}

static void gl3_set_texture_enable(void *data, bool state, bool full_screen)
{
   gl3_t *gl = (gl3_t*)data;

   if (!gl)
      return;

   gl->menu_texture_enable      = state;
   gl->menu_texture_full_screen = full_screen;
}

static void gl3_get_video_output_size(void *data,
      unsigned *width, unsigned *height, char *desc, size_t desc_len)
{
   gl3_t   *gl = (gl3_t*)data;
   if (gl && gl->ctx_driver && gl->ctx_driver->get_video_output_size)
      gl->ctx_driver->get_video_output_size(
            gl->ctx_data,
            width, height, desc, desc_len);
}

static void gl3_get_video_output_prev(void *data)
{
   gl3_t   *gl = (gl3_t*)data;
   if (gl && gl->ctx_driver && gl->ctx_driver->get_video_output_prev)
      gl->ctx_driver->get_video_output_prev(gl->ctx_data);
}

static void gl3_get_video_output_next(void *data)
{
   gl3_t   *gl = (gl3_t*)data;
   if (gl && gl->ctx_driver && gl->ctx_driver->get_video_output_next)
      gl->ctx_driver->get_video_output_next(gl->ctx_data);
}

static uintptr_t gl3_get_current_framebuffer(void *data)
{
   gl3_t *gl = (gl3_t*)data;
   if (!gl || !gl->hw_render_enable)
      return 0;
   return gl->hw_render_fbo;
}

static retro_proc_address_t gl3_get_proc_address(
      void *data, const char *sym)
{
   gl3_t *gl = (gl3_t*)data;
   if (gl && gl->ctx_driver->get_proc_address)
      return gl->ctx_driver->get_proc_address(sym);

   return NULL;
}

static const video_poke_interface_t gl3_poke_interface = {
   gl3_get_flags,
   gl3_load_texture,
   gl3_unload_texture,
   gl3_set_video_mode,
   gl3_get_refresh_rate, /* get_refresh_rate */
   NULL,
   gl3_get_video_output_size,
   gl3_get_video_output_prev,
   gl3_get_video_output_next,
   gl3_get_current_framebuffer,
   gl3_get_proc_address,
   gl3_set_aspect_ratio,
   gl3_apply_state_changes,
   gl3_set_texture_frame,
   gl3_set_texture_enable,
   font_driver_render_msg,
   gl3_show_mouse,
   NULL,                               /* grab_mouse_toggle */
   gl3_get_current_shader,
   NULL,
   NULL,
   NULL, /* set_hdr_max_nits */
   NULL, /* set_hdr_paper_white_nits */
   NULL, /* set_hdr_contrast */
   NULL  /* set_hdr_expand_gamut */
};

static void gl3_get_poke_interface(void *data,
      const video_poke_interface_t **iface)
{
   (void)data;
   *iface = &gl3_poke_interface;
}

#ifdef HAVE_GFX_WIDGETS
static bool gl3_gfx_widgets_enabled(void *data) { return true; }
#endif

static unsigned gl3_wrap_type_to_enum(enum gfx_wrap_type type)
{
   switch (type)
   {
      case RARCH_WRAP_BORDER:
#ifdef HAVE_OPENGLES3
         /* GLES does not support CLAMP_TO_BORDER until GLES 3.2.
          * It is a deprecated feature in general. */
         return GL_CLAMP_TO_EDGE;
#else
         return GL_CLAMP_TO_BORDER;
#endif
      case RARCH_WRAP_EDGE:
         return GL_CLAMP_TO_EDGE;
      case RARCH_WRAP_REPEAT:
         return GL_REPEAT;
      case RARCH_WRAP_MIRRORED_REPEAT:
         return GL_MIRRORED_REPEAT;
      default:
         break;
   }

   return 0;
}

static bool gl3_has_windowed(void *data)
{
   gl3_t *gl        = (gl3_t*)data;
   if (gl && gl->ctx_driver)
      return gl->ctx_driver->has_windowed;
   return false;
}

static bool gl3_focus(void *data)
{
   gl3_t *gl        = (gl3_t*)data;
   if (gl && gl->ctx_driver && gl->ctx_driver->has_focus)
      return gl->ctx_driver->has_focus(gl->ctx_data);
   return true;
}

video_driver_t video_gl3 = {
   gl3_init,
   gl3_frame,
   gl3_set_nonblock_state,
   gl3_alive,
   gl3_focus,
   gl3_suppress_screensaver,
   gl3_has_windowed,

   gl3_set_shader,

   gl3_free,
   "glcore",

   gl3_set_viewport_wrapper,
   gl3_set_rotation,

   gl3_viewport_info,

   gl3_read_viewport,
#if defined(READ_RAW_GL_FRAME_TEST)
   gl3_read_frame_raw,
#else
   NULL,
#endif

#ifdef HAVE_OVERLAY
   gl3_get_overlay_interface,
#endif
#ifdef HAVE_VIDEO_LAYOUT
   NULL,
#endif
   gl3_get_poke_interface,
   gl3_wrap_type_to_enum,
#ifdef HAVE_GFX_WIDGETS
   gl3_gfx_widgets_enabled
#endif
};
