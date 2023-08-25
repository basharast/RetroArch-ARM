/* RetroArch - A frontend for libretro.
 * Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 * Copyright (C) 2011-2017 - Daniel De Matteis
 * Copyright (C) 2012-2015 - Michael Lelli
 *
 * RetroArch is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Found-
 * ation, either version 3 of the License, or (at your option) any later version.
 *
 * RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with RetroArch.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fat.h>
#include <gctypes.h>
#include <ogc/cache.h>
#include <ogc/system.h>
#include <ogc/usbstorage.h>
#include <ogc/lwp_threads.h>
#include <sdcard/wiisd_io.h>

#include <file/file_path.h>
#include <retro_miscellaneous.h>

#ifdef HAVE_CONFIG_H
#include "../../config.h"
#endif

#include "../../verbosity.h"

#define EXECUTE_ADDR ((uint8_t *) 0x91800000)
#define BOOTER_ADDR  ((uint8_t *) 0x93000000)
#define ARGS_ADDR    ((uint8_t *) 0x93200000)

extern uint8_t _binary_wii_app_booter_app_booter_bin_start[];
extern uint8_t _binary_wii_app_booter_app_booter_bin_end[];
#define booter_start _binary_wii_app_booter_app_booter_bin_start
#define booter_end _binary_wii_app_booter_app_booter_bin_end

#ifdef IS_SALAMANDER
char gx_rom_path[PATH_MAX_LENGTH];
#endif

static void dol_copy_argv_path(const char *dolpath, const char *argpath)
{
   size_t t_len;
   char tmp[PATH_MAX_LENGTH] = {0};
   struct __argv       *argv = (struct __argv *)ARGS_ADDR;
   char             *cmdline = NULL;
   size_t                len = 0;

   memset(ARGS_ADDR, 0, sizeof(struct __argv));

   cmdline = (char *)ARGS_ADDR + sizeof(struct __argv);

   argv->argvMagic = ARGV_MAGIC;
   argv->commandLine = cmdline;

   /* a device-less fullpath */
   if (dolpath[0] == '/')
   {
      char *dev = strchr(__system_argv->argv[0], ':');
      t_len = dev - __system_argv->argv[0] + 1;
      memcpy(cmdline, __system_argv->argv[0], t_len);
      len += t_len;
   }
   /* a relative path */
   else if (
         (strstr(dolpath, "sd:/")    != dolpath) &&
         (strstr(dolpath, "usb:/")   != dolpath) &&
         (strstr(dolpath, "carda:/") != dolpath) &&
         (strstr(dolpath, "cardb:/") != dolpath)
         )
   {
      fill_pathname_parent_dir(tmp,
            __system_argv->argv[0], sizeof(tmp));
      t_len        = strlen(tmp);
      memcpy(cmdline, tmp, t_len);
      len         += t_len;
   }

   t_len           = strlen(dolpath);
   memcpy(cmdline + len, dolpath, t_len);
   len            += t_len;
   cmdline[len++]  = 0;

   /* File must be split into two parts,
    * the path and the actual filename
    * done to be compatible with loaders. */
   if (argpath && strrchr(argpath, '/') != NULL)
   {
      char *name = NULL;

      /* basedir. */
      fill_pathname_parent_dir(tmp, argpath, sizeof(tmp));
      t_len = strlen(tmp);
      memcpy(cmdline + len, tmp, t_len);
      len += t_len;
      cmdline[len++] = 0;

      /* filename */
      name = strrchr(argpath, '/') + 1;
      t_len = strlen(name);
      memcpy(cmdline + len, name, t_len);
      len += t_len;
      cmdline[len++] = 0;
   }

   cmdline[len++] = 0;
   argv->length = len;
   DCFlushRange(ARGS_ADDR, sizeof(struct __argv) + argv->length);
}

/* WARNING: after we move any data
 * into EXECUTE_ADDR, we can no longer use any
 * heap memory and are restricted to the stack only. */
void system_exec_wii(const char *_path, bool should_load_game)
{
   size_t size, booter_size;
   FILE *fp                        = NULL;
   void *dol                       = NULL;
   char path[PATH_MAX_LENGTH]      = {0};
   char game_path[PATH_MAX_LENGTH] = {0};
#ifndef IS_SALAMANDER
   bool original_verbose           = verbosity_is_enabled();
#endif

   /* copy heap info into stack so it survives
    * us moving the .dol into MEM2. */
   strlcpy(path, _path, sizeof(path));
   if (should_load_game)
   {
#ifdef IS_SALAMANDER
      strlcpy(game_path, gx_rom_path, sizeof(game_path));
#else
      strlcpy(game_path, path_get(RARCH_PATH_CONTENT), sizeof(game_path));
#endif
   }

   RARCH_LOG("Attempt to load executable: [%s]\n", path);

   fp = fopen(path, "rb");
   if (!fp)
   {
      RARCH_ERR("Could not open DOL file %s.\n", path);
      goto exit;
   }

   fseek(fp, 0, SEEK_END);
   size = ftell(fp);
   fseek(fp, 0, SEEK_SET);

   /* try to allocate a buffer for it. if we can't, fail. */
   dol = malloc(size);
   if (!dol)
   {
      RARCH_ERR("Could not execute DOL file %s.\n", path);
      fclose(fp);
      goto exit;
   }

   fread(dol, 1, size, fp);
   fclose(fp);

   fatUnmount("carda:");
   fatUnmount("cardb:");
   fatUnmount("sd:");
   fatUnmount("usb:");
   __io_wiisd.shutdown();
   __io_usbstorage.shutdown();

   /* don't use memcpy, there might be an overlap. */
   memmove(EXECUTE_ADDR, dol, size);
   DCFlushRange(EXECUTE_ADDR, size);

   dol_copy_argv_path(path, should_load_game ? game_path : NULL);

   booter_size = booter_end - booter_start;
   memcpy(BOOTER_ADDR, booter_start, booter_size);
   DCFlushRange(BOOTER_ADDR, booter_size);

   RARCH_LOG("jumping to %08x\n", (unsigned) BOOTER_ADDR);
   SYS_ResetSystem(SYS_SHUTDOWN,0,0);
   __lwp_thread_stopmultitasking((void (*)(void)) BOOTER_ADDR);

exit:
   (void)0;
#ifndef IS_SALAMANDER
   if (original_verbose)
      verbosity_enable();
   else
      verbosity_disable();
#endif
}
