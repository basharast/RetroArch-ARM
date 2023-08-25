#include "shaders_common.h"

static const char *stock_fragment_xmb_simple_snow = GLSL(
   uniform float time;
   uniform vec2 OutputSize;

   float baseScale = 1.25; /* [1.0  .. 10.0] */
   float density   = 0.5;  /* [0.01 ..  1.0] */
   float speed     = 0.15; /* [0.1  ..  1.0] */

   float rand(vec2 co)
   {
      return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
   }

   float dist_func(vec2 distv)
   {
      float dist = sqrt((distv.x * distv.x) + (distv.y * distv.y)) * (40.0 / baseScale);
      dist = clamp(dist, 0.0, 1.0);
      return cos(dist * (3.14159265358 * 0.5)) * 0.5;
   }

   float random_dots(vec2 co)
   {
      float part = 1.0 / 20.0;
      vec2 cd = floor(co / part);
      float p = rand(cd);

      if (p > 0.005 * (density * 40.0))
         return 0.0;

      vec2 dpos = (vec2(fract(p * 2.0) , p) + vec2(2.0, 2.0)) * 0.25;

      vec2 cellpos = fract(co / part);
      vec2 distv = (cellpos - dpos);

      return dist_func(distv);
   }

   float snow(vec2 pos, float time, float scale)
   {
      /* add wobble */
      pos.x += cos(pos.y * 1.2 + time * 3.14159 * 2.0 + 1.0 / scale) / (8.0 / scale) * 4.0;
      /* add gravity */
      pos += time * scale * vec2(-0.5, 1.0) * 4.0;
      return random_dots(pos / scale) * (scale * 0.5 + 0.5);
   }

   void main(void)
   {
      float tim = time * 0.4 * speed;
      vec2 pos = gl_FragCoord.xy / OutputSize.xx;
      float a = 0.0;
      /**
       * Each of these is a layer of snow
       * Remove some for better performance
       * Changing the scale (3rd value) will mess with the looping
       **/
      a += snow(pos, tim, 1.0);
      a += snow(pos, tim, 0.7);
      a += snow(pos, tim, 0.6);
      a += snow(pos, tim, 0.5);
      a += snow(pos, tim, 0.4);
      a += snow(pos, tim, 0.3);
      a += snow(pos, tim, 0.25);
      a += snow(pos, tim, 0.125);
      a = a * min(pos.y * 4.0, 1.0);
      gl_FragColor = vec4(1.0, 1.0, 1.0, a);
   }

);
