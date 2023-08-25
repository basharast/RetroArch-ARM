#ifndef VIDEO_LAYOUT_ELEMENT_H
#define VIDEO_LAYOUT_ELEMENT_H

#include "internal.h"
#include "component.h"

typedef struct element
{
   char            *name;
   component_t     *components;
   int              components_count;
   int              state;
   int              o_bind;
   int              i_bind;
   int              i_mask;
   video_layout_bounds_t  bounds;         /* float alignment */
   video_layout_bounds_t  render_bounds;  /* float alignment */
   bool             i_raw;
} element_t;

void element_init              (element_t *elem, const char *name, int components_count);
void element_copy              (element_t *elem, const element_t *src);
void element_deinit            (element_t *elem);
void element_apply_orientation (element_t *elem, video_layout_orientation_t orientation);

#endif
