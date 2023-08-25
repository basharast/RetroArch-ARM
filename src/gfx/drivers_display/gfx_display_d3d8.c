﻿/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2011-2017 - Daniel De Matteis
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

#define CINTERFACE

#include <retro_miscellaneous.h>

#include <gfx/math/matrix_4x4.h>

#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif

#include "../gfx_display.h"

#include "../../retroarch.h"
#include "../common/d3d_common.h"
#include "../common/d3d8_common.h"

static const float d3d8_vertexes[] = {
   0, 0,
   1, 0,
   0, 1,
   1, 1
};

static const float d3d8_tex_coords[] = {
   0, 1,
   1, 1,
   0, 0,
   1, 0
};

static const float *gfx_display_d3d8_get_default_vertices(void)
{
   return &d3d8_vertexes[0];
}

static const float *gfx_display_d3d8_get_default_tex_coords(void)
{
   return &d3d8_tex_coords[0];
}

static void *gfx_display_d3d8_get_default_mvp(void *data)
{
   static math_matrix_4x4 id;
   matrix_4x4_identity(id);

   return &id;
}

static INT32 gfx_display_prim_to_d3d8_enum(
      enum gfx_display_prim_type prim_type)
{
   switch (prim_type)
   {
      case GFX_DISPLAY_PRIM_TRIANGLES:
      case GFX_DISPLAY_PRIM_TRIANGLESTRIP:
         return D3DPT_COMM_TRIANGLESTRIP;
      case GFX_DISPLAY_PRIM_NONE:
      default:
         break;
   }

   /* TODO/FIXME - hack */
   return 0;
}

static void gfx_display_d3d8_blend_begin(void *data)
{
   d3d8_video_t *d3d             = (d3d8_video_t*)data;

   if (!d3d)
      return;

   d3d8_enable_blend_func(d3d->dev);
}

static void gfx_display_d3d8_blend_end(void *data)
{
   d3d8_video_t *d3d             = (d3d8_video_t*)data;

   if (!d3d)
      return;

   d3d8_disable_blend_func(d3d->dev);
}

static void gfx_display_d3d8_bind_texture(gfx_display_ctx_draw_t *draw,
      d3d8_video_t *d3d)
{
   LPDIRECT3DDEVICE8 dev = d3d->dev;

   d3d8_set_texture(d3d->dev, 0, (void*)draw->texture);
   d3d8_set_sampler_address_u(d3d->dev, 0, D3DTADDRESS_COMM_CLAMP);
   d3d8_set_sampler_address_v(d3d->dev, 0, D3DTADDRESS_COMM_CLAMP);
   d3d8_set_sampler_minfilter(d3d->dev, 0, D3DTEXF_COMM_LINEAR);
   d3d8_set_sampler_magfilter(d3d->dev, 0, D3DTEXF_COMM_LINEAR);
}

static void gfx_display_d3d8_draw(gfx_display_ctx_draw_t *draw,
      void *data,
      unsigned video_width, 
      unsigned video_height)
{
   unsigned i;
   math_matrix_4x4 mop, m1, m2;
   d3d8_video_t *d3d             = (d3d8_video_t*)data;
   Vertex * pv                   = NULL;
   const float *vertex           = NULL;
   const float *tex_coord        = NULL;
   const float *color            = NULL;

   if (!d3d || !draw || draw->pipeline_id)
      return;
   if ((d3d->menu_display.offset + draw->coords->vertices )
         > (unsigned)d3d->menu_display.size)
      return;

   pv           = (Vertex*)
      d3d8_vertex_buffer_lock(d3d->menu_display.buffer);

   if (!pv)
      return;

   pv          += d3d->menu_display.offset;
   vertex       = draw->coords->vertex;
   tex_coord    = draw->coords->tex_coord;
   color        = draw->coords->color;

   if (!vertex)
      vertex    = &d3d8_vertexes[0];
   if (!tex_coord)
      tex_coord = &d3d8_tex_coords[0];

   for (i = 0; i < draw->coords->vertices; i++)
   {
      int colors[4];

      colors[0]   = *color++ * 0xFF;
      colors[1]   = *color++ * 0xFF;
      colors[2]   = *color++ * 0xFF;
      colors[3]   = *color++ * 0xFF;

      pv[i].x     = *vertex++;
      pv[i].y     = *vertex++;
      pv[i].z     = 0.5f;
      pv[i].u     = *tex_coord++;
      pv[i].v     = *tex_coord++;

#if 1
	  if ((void*)draw->texture)
      {
         D3DSURFACE_DESC desc;
         if (d3d8_texture_get_level_desc((void*)draw->texture, 0, &desc))
         {
            pv[i].u *= desc.Width;
            pv[i].v *= desc.Height;
         }
      }
#endif

      pv[i].color =
         D3DCOLOR_ARGB(
               colors[3], /* A */
               colors[0], /* R */
               colors[1], /* G */
               colors[2]  /* B */
               );
   }
   d3d8_vertex_buffer_unlock(d3d->menu_display.buffer);

   if (!draw->matrix_data)
      draw->matrix_data = gfx_display_d3d8_get_default_mvp(d3d);

   /* ugh */
   matrix_4x4_scale(m1,       2.0,  2.0, 0);
   matrix_4x4_translate(mop, -1.0, -1.0, 0);
   matrix_4x4_multiply(m2, mop, m1);
   matrix_4x4_multiply(m1,
         *((math_matrix_4x4*)draw->matrix_data), m2);
   matrix_4x4_scale(mop,
         (draw->width  / 2.0) / video_width,
         (draw->height / 2.0) / video_height, 0);
   matrix_4x4_multiply(m2, mop, m1);
   matrix_4x4_translate(mop,
         (draw->x + (draw->width  / 2.0)) / video_width,
         (draw->y + (draw->height / 2.0)) / video_height,
         0);
   matrix_4x4_multiply(m1, mop, m2);
   matrix_4x4_multiply(m2, d3d->mvp_transposed, m1);
   d3d_matrix_transpose(&m1, &m2);

   d3d8_set_mvp(d3d->dev, &m1);

   if (draw && draw->texture)
      gfx_display_d3d8_bind_texture(draw, d3d);

   d3d8_draw_primitive(d3d->dev,
         gfx_display_prim_to_d3d8_enum(draw->prim_type),
         d3d->menu_display.offset,
         draw->coords->vertices -
         ((draw->prim_type == GFX_DISPLAY_PRIM_TRIANGLESTRIP)
          ? 2 : 0));

   d3d->menu_display.offset += draw->coords->vertices;
}

static bool gfx_display_d3d8_font_init_first(
      void **font_handle, void *video_data,
      const char *font_path, float font_size,
      bool is_threaded)
{
   font_data_t **handle = (font_data_t**)font_handle;
   if (!(*handle = font_driver_init_first(video_data,
         font_path, font_size, true,
         is_threaded,
         FONT_DRIVER_RENDER_D3D8_API)))
		 return false;
   return true;
}

gfx_display_ctx_driver_t gfx_display_ctx_d3d8 = {
   gfx_display_d3d8_draw,
   NULL,                                        /* draw_pipeline */
   gfx_display_d3d8_blend_begin,
   gfx_display_d3d8_blend_end,
   gfx_display_d3d8_get_default_mvp,
   gfx_display_d3d8_get_default_vertices,
   gfx_display_d3d8_get_default_tex_coords,
   gfx_display_d3d8_font_init_first,
   GFX_VIDEO_DRIVER_DIRECT3D8,
   "d3d8",
   false,
   NULL,
   NULL
};
