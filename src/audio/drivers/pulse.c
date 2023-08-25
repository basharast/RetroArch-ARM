/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
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

#include <stdint.h>
#include <string.h>

#include <pulse/pulseaudio.h>

#include <boolean.h>
#include <retro_miscellaneous.h>
#include <retro_endianness.h>

#include "../audio_driver.h"
#include "../../verbosity.h"

typedef struct
{
   pa_threaded_mainloop *mainloop;
   pa_context *context;
   pa_stream *stream;
   size_t buffer_size;
   bool nonblock;
   bool success;
   bool is_paused;
} pa_t;

static void pulse_free(void *data)
{
   pa_t *pa = (pa_t*)data;

   if (!pa)
      return;

   if (pa->mainloop)
      pa_threaded_mainloop_stop(pa->mainloop);

   if (pa->stream)
   {
      pa_stream_disconnect(pa->stream);
      pa_stream_unref(pa->stream);
   }

   if (pa->context)
   {
      pa_context_disconnect(pa->context);
      pa_context_unref(pa->context);
   }

   if (pa->mainloop)
      pa_threaded_mainloop_free(pa->mainloop);

   free(pa);
}

static void stream_success_cb(pa_stream *s, int success, void *data)
{
   pa_t *pa = (pa_t*)data;
   (void)s;
   pa->success = success;
   pa_threaded_mainloop_signal(pa->mainloop, 0);
}

static void context_state_cb(pa_context *c, void *data)
{
   pa_t *pa = (pa_t*)data;

   switch (pa_context_get_state(c))
   {
      case PA_CONTEXT_READY:
      case PA_CONTEXT_TERMINATED:
      case PA_CONTEXT_FAILED:
         pa_threaded_mainloop_signal(pa->mainloop, 0);
         break;
      default:
         break;
   }
}

static void stream_state_cb(pa_stream *s, void *data)
{
   pa_t *pa = (pa_t*)data;

   switch (pa_stream_get_state(s))
   {
      case PA_STREAM_READY:
      case PA_STREAM_FAILED:
      case PA_STREAM_TERMINATED:
         pa_threaded_mainloop_signal(pa->mainloop, 0);
         break;
      default:
         break;
   }
}

static void stream_request_cb(pa_stream *s, size_t length, void *data)
{
   pa_t *pa = (pa_t*)data;

   (void)length;
   (void)s;

   pa_threaded_mainloop_signal(pa->mainloop, 0);
}

static void stream_latency_update_cb(pa_stream *s, void *data)
{
   pa_t *pa = (pa_t*)data;

   (void)s;

   pa_threaded_mainloop_signal(pa->mainloop, 0);
}

static void underrun_update_cb(pa_stream *s, void *data)
{
#if 0
   pa_t *pa = (pa_t*)data;

   (void)s;

   RARCH_LOG("[PulseAudio]: Underrun (Buffer: %u, Writable size: %u).\n",
         (unsigned)pa->buffer_size,
         (unsigned)pa_stream_writable_size(pa->stream));
#endif
}

static void buffer_attr_cb(pa_stream *s, void *data)
{
   pa_t *pa = (pa_t*)data;
   const pa_buffer_attr *server_attr = pa_stream_get_buffer_attr(s);
   if (server_attr)
      pa->buffer_size = server_attr->tlength;

#if 0
   RARCH_LOG("[PulseAudio]: Got new buffer size %u.\n", (unsigned)pa->buffer_size);
#endif
}

static void *pulse_init(const char *device, unsigned rate,
      unsigned latency,
      unsigned block_frames,
      unsigned *new_rate)
{
   pa_sample_spec               spec;
   pa_buffer_attr        buffer_attr = {0};
   const pa_buffer_attr *server_attr = NULL;
   pa_t                          *pa = (pa_t*)calloc(1, sizeof(*pa));

   memset(&spec, 0, sizeof(spec));

   if (!pa)
      goto error;

   pa->mainloop = pa_threaded_mainloop_new();
   if (!pa->mainloop)
      goto error;

   pa->context = pa_context_new(pa_threaded_mainloop_get_api(pa->mainloop), "RetroArch");
   if (!pa->context)
      goto error;

   pa_context_set_state_callback(pa->context, context_state_cb, pa);

   if (pa_context_connect(pa->context, device, PA_CONTEXT_NOFLAGS, NULL) < 0)
      goto error;

   pa_threaded_mainloop_lock(pa->mainloop);
   if (pa_threaded_mainloop_start(pa->mainloop) < 0)
      goto error;

   pa_threaded_mainloop_wait(pa->mainloop);

   if (pa_context_get_state(pa->context) != PA_CONTEXT_READY)
      goto unlock_error;

   spec.format   = is_little_endian() ? PA_SAMPLE_FLOAT32LE : PA_SAMPLE_FLOAT32BE;
   spec.channels = 2;
   spec.rate     = rate;

   pa->stream    = pa_stream_new(pa->context, "audio", &spec, NULL);
   if (!pa->stream)
      goto unlock_error;

   pa_stream_set_state_callback(pa->stream, stream_state_cb, pa);
   pa_stream_set_write_callback(pa->stream, stream_request_cb, pa);
   pa_stream_set_latency_update_callback(pa->stream, stream_latency_update_cb, pa);
   pa_stream_set_underflow_callback(pa->stream, underrun_update_cb, pa);
   pa_stream_set_buffer_attr_callback(pa->stream, buffer_attr_cb, pa);

   buffer_attr.maxlength = -1;
   buffer_attr.tlength   = pa_usec_to_bytes(latency * PA_USEC_PER_MSEC, &spec);
   buffer_attr.prebuf    = -1;
   buffer_attr.minreq    = -1;
   buffer_attr.fragsize  = -1;

   if (pa_stream_connect_playback(pa->stream, NULL,
            &buffer_attr, PA_STREAM_ADJUST_LATENCY, NULL, NULL) < 0)
      goto error;

   pa_threaded_mainloop_wait(pa->mainloop);

   if (pa_stream_get_state(pa->stream) != PA_STREAM_READY)
      goto unlock_error;

   server_attr = pa_stream_get_buffer_attr(pa->stream);
   if (server_attr)
   {
      pa->buffer_size = server_attr->tlength;
      RARCH_LOG("[PulseAudio]: Requested %u bytes buffer, got %u.\n",
            (unsigned)buffer_attr.tlength,
            (unsigned)pa->buffer_size);
   }
   else
      pa->buffer_size = buffer_attr.tlength;

   pa_threaded_mainloop_unlock(pa->mainloop);

   return pa;

unlock_error:
   pa_threaded_mainloop_unlock(pa->mainloop);
error:
   pulse_free(pa);
   return NULL;
}

static bool pulse_start(void *data, bool is_shutdown);
static ssize_t pulse_write(void *data, const void *buf_, size_t size)
{
   pa_t           *pa = (pa_t*)data;
   const uint8_t *buf = (const uint8_t*)buf_;
   size_t     written = 0;

   /* Workaround buggy menu code.
    * If a write happens while we're paused, we might never progress. */
   if (pa->is_paused)
      if (!pulse_start(pa, false))
         return -1;

   pa_threaded_mainloop_lock(pa->mainloop);
   while (size)
   {
      size_t writable = MIN(size, pa_stream_writable_size(pa->stream));

      if (writable)
      {
         pa_stream_write(pa->stream, buf, writable, NULL, 0, PA_SEEK_RELATIVE);
         buf += writable;
         size -= writable;
         written += writable;
      }
      else if (!pa->nonblock)
         pa_threaded_mainloop_wait(pa->mainloop);
      else
         break;
   }

   pa_threaded_mainloop_unlock(pa->mainloop);

   return written;
}

static bool pulse_stop(void *data)
{
   bool ret;
   pa_t *pa = (pa_t*)data;
   if (pa->is_paused)
      return true;

   RARCH_LOG("[PulseAudio]: Pausing.\n");

   pa->success = true; /* In case of spurious wakeup. Not critical. */
   pa_threaded_mainloop_lock(pa->mainloop);
   pa_stream_cork(pa->stream, true, stream_success_cb, pa);
   pa_threaded_mainloop_wait(pa->mainloop);
   ret = pa->success;
   pa_threaded_mainloop_unlock(pa->mainloop);
   pa->is_paused = true;
   return ret;
}

static bool pulse_alive(void *data)
{
   pa_t *pa = (pa_t*)data;

   if (!pa)
      return false;
   return !pa->is_paused;
}

static bool pulse_start(void *data, bool is_shutdown)
{
   bool ret;
   pa_t *pa = (pa_t*)data;
   if (!pa->is_paused)
      return true;

   RARCH_LOG("[PulseAudio]: Unpausing.\n");

   pa->success = true; /* In case of spurious wakeup. Not critical. */
   pa_threaded_mainloop_lock(pa->mainloop);
   pa_stream_cork(pa->stream, false, stream_success_cb, pa);
   pa_threaded_mainloop_wait(pa->mainloop);
   ret = pa->success;
   pa_threaded_mainloop_unlock(pa->mainloop);
   pa->is_paused = false;
   return ret;
}

static void pulse_set_nonblock_state(void *data, bool state)
{
   pa_t *pa = (pa_t*)data;
   if (pa)
      pa->nonblock = state;
}

static bool pulse_use_float(void *data)
{
   (void)data;
   return true;
}

static size_t pulse_write_avail(void *data)
{
   size_t length;
   pa_t *pa = (pa_t*)data;

   pa_threaded_mainloop_lock(pa->mainloop);
   length = pa_stream_writable_size(pa->stream);

   audio_driver_set_buffer_size(pa->buffer_size); /* Can change spuriously. */
   pa_threaded_mainloop_unlock(pa->mainloop);
   return length;
}

static size_t pulse_buffer_size(void *data)
{
   pa_t *pa = (pa_t*)data;
   return pa->buffer_size;
}

audio_driver_t audio_pulse = {
   pulse_init,
   pulse_write,
   pulse_stop,
   pulse_start,
   pulse_alive,
   pulse_set_nonblock_state,
   pulse_free,
   pulse_use_float,
   "pulse",
   NULL,
   NULL,
   pulse_write_avail,
   pulse_buffer_size,
};
