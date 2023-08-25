/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *  Copyright (C) 2012-2015 - Michael Lelli
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

#include <libretro.h>
#include <glsym/glsym.h>

#include "../camera_driver.h"
#include "../../retroarch.h"

typedef struct android_camera
{
   jmethodID onCameraInit;
   jmethodID onCameraFree;
   jmethodID onCameraPoll;
   jmethodID onCameraStart;
   jmethodID onCameraStop;
   jmethodID onCameraSetTexture;
   GLuint tex;
} androidcamera_t;

static void *android_camera_init(const char *device, uint64_t caps,
      unsigned width, unsigned height)
{
   jclass class;
   androidcamera_t *androidcamera  = NULL;
   JNIEnv                    *env  = NULL;
   struct android_app *android_app = (struct android_app*)g_android;

   (void)device;
   (void)width;
   (void)height;

   if ((caps & (UINT64_C(1) << RETRO_CAMERA_BUFFER_OPENGL_TEXTURE)) == 0)
   {
      RARCH_ERR("androidcamera returns OpenGL texture.\n");
      return NULL;
   }

   androidcamera = (androidcamera_t*)calloc(1, sizeof(androidcamera_t));
   if (!androidcamera)
      return NULL;

   env = jni_thread_getenv();
   if (!env)
      goto dealloc;

   GET_OBJECT_CLASS(env, class, android_app->activity->clazz);
   if (!class)
      goto dealloc;

   GET_METHOD_ID(env, androidcamera->onCameraInit, class,
         "onCameraInit", "()V");
   if (!androidcamera->onCameraInit)
      goto dealloc;

   GET_METHOD_ID(env, androidcamera->onCameraFree, class,
         "onCameraFree", "()V");
   if (!androidcamera->onCameraFree)
      goto dealloc;

   GET_METHOD_ID(env, androidcamera->onCameraSetTexture, class,
         "onCameraSetTexture", "(I)V");
   if (!androidcamera->onCameraSetTexture)
      goto dealloc;

   GET_METHOD_ID(env, androidcamera->onCameraStart, class,
         "onCameraStart", "()V");
   if (!androidcamera->onCameraStart)
      goto dealloc;

   GET_METHOD_ID(env, androidcamera->onCameraStop, class,
         "onCameraStop", "()V");
   if (!androidcamera->onCameraStop)
      goto dealloc;

   GET_METHOD_ID(env, androidcamera->onCameraPoll, class,
         "onCameraPoll", "()Z");
   if (!androidcamera->onCameraPoll)
      goto dealloc;

   CALL_VOID_METHOD(env, android_app->activity->clazz,
         androidcamera->onCameraInit);

   return androidcamera;
dealloc:
   if (androidcamera)
      free(androidcamera);
   return NULL;
}

static void android_camera_free(void *data)
{
   struct android_app *android_app = (struct android_app*)g_android;
   androidcamera_t *androidcamera  = (androidcamera_t*)data;
   JNIEnv                     *env = jni_thread_getenv();

   if (!env)
      return;

   CALL_VOID_METHOD(env, android_app->activity->clazz,
         androidcamera->onCameraFree);

   free(androidcamera);
}

static bool android_camera_start(void *data)
{
   struct android_app *android_app = (struct android_app*)g_android;
   androidcamera_t *androidcamera  = (androidcamera_t*)data;
   JNIEnv                     *env = jni_thread_getenv();

   if (!env)
      return NULL;

   glGenTextures(1, &androidcamera->tex);
   glBindTexture(GL_TEXTURE_EXTERNAL_OES, androidcamera->tex);
   glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S,
         GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T,
         GL_CLAMP_TO_EDGE);

   CALL_VOID_METHOD_PARAM(env, android_app->activity->clazz,
         androidcamera->onCameraSetTexture, (int) androidcamera->tex);
   CALL_VOID_METHOD(env, android_app->activity->clazz,
         androidcamera->onCameraStart);

   return true;
}

static void android_camera_stop(void *data)
{
   struct android_app *android_app = (struct android_app*)g_android;
   androidcamera_t  *androidcamera = (androidcamera_t*)data;
   JNIEnv                     *env = jni_thread_getenv();

   if (!env)
      return;

   CALL_VOID_METHOD(env, android_app->activity->clazz,
         androidcamera->onCameraStop);

   if (androidcamera->tex)
      video_driver_texture_unload((uintptr_t*)&androidcamera->tex);
}

static bool android_camera_poll(void *data,
      retro_camera_frame_raw_framebuffer_t frame_raw_cb,
      retro_camera_frame_opengl_texture_t frame_gl_cb)
{
   jboolean newFrame;
   struct android_app *android_app = (struct android_app*)g_android;
   androidcamera_t  *androidcamera = (androidcamera_t*)data;
   JNIEnv                     *env = jni_thread_getenv();

   if (!env)
      return NULL;

   (void)frame_raw_cb;

   CALL_BOOLEAN_METHOD(env, newFrame, android_app->activity->clazz,
         androidcamera->onCameraPoll);

   if (newFrame)
   {
      /* FIXME: Identity for now. Use proper texture matrix as
       * returned by Android Camera. */
      static const float affine[] = {
         1.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 1.0f
      };

      if (frame_gl_cb)
        frame_gl_cb(androidcamera->tex,
              GL_TEXTURE_EXTERNAL_OES,
              affine);
      return true;
   }

   return false;
}

camera_driver_t camera_android = {
   android_camera_init,
   android_camera_free,
   android_camera_start,
   android_camera_stop,
   android_camera_poll,
   "android",
};
