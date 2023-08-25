#if defined(_MSC_VER) && !defined(_XBOX) && (_MSC_VER >= 1500 && _MSC_VER < 1900)
#if (_MSC_VER >= 1700)
/* https://support.microsoft.com/en-us/kb/980263 */
#pragma execution_character_set("utf-8")
#endif
#pragma warning(disable:4566)
#endif

/* Top-Level Menu */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MAIN_MENU,
   "Hlavné Menu"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SETTINGS_TAB,
   "Nastavenia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_FAVORITES_TAB,
   "Obľúbené"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_HISTORY_TAB,
   "História"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_IMAGES_TAB,
   "Obrázky"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MUSIC_TAB,
   "Hudba"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_TAB,
   "Videá"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_EXPLORE_TAB,
   "Prehľadávať"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_ADD_TAB,
   "Import Obsahu"
   )

/* Main Menu */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CONTENT_SETTINGS,
   "Rýchle Menu"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CONTENT_SETTINGS,
   "Rýchly prístup ku všetkým vnútro-herným nastaveniam."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_LIST,
   "Načítať jadro"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CORE_LIST,
   "Výber jadra (core) k použití."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_LOAD_CONTENT_LIST,
   "Načítať Obsah"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_LOAD_CONTENT_LIST,
   "Vyber ktorý obsah sa má spustiť."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_LOAD_DISC,
   "Načítať Disk"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_LOAD_DISC,
   "Možnosť načítať fyzický mediálny disk (CD/DVD). Najprv zvoľ jadro (Načítať Jadro) na spustenie disku."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DUMP_DISC,
   "Preniesť Disk"
   )
MSG_HASH( /* FIXME Is a specific image format used? Is it determined automatically? User choice? */
   MENU_ENUM_SUBLABEL_DUMP_DISC,
   "Prenos dat z fyzického disku. Obrazec sa v úložišti uloží ako ISO formát."
   )
#ifdef HAVE_LAKKA
#endif
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_PLAYLISTS_TAB,
   "Hracie zoznamy"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_PLAYLISTS_TAB,
   "Vyhľadávaný obsah ktorý sa bude zhodovať s databázou sa objavý tu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_ADD_CONTENT_LIST,
   "Import Obsahu"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_ADD_CONTENT_LIST,
   "Vytvoriť a aktualizovať herný zoznam vyhľadávaním obsahu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SHOW_WIMP,
   "Zobraziť Menu ako Plochu"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_SHOW_WIMP,
   "Zobrazí tradičné menu plochy."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MENU_DISABLE_KIOSK_MODE,
   "Zakázať režim kiosku (vyžadovaný reštart)"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_MENU_DISABLE_KIOSK_MODE,
   "Zobraziť všetky nastavenia súvisiace s konfiguráciou."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_ONLINE_UPDATER,
   "Online Aktualizácie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_ONLINE_UPDATER,
   "Stiahnutie prídavkov, komponentov, a obsahu pre RetroArch."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_NETPLAY,
   "Pridať sa alebo hostiť zasedanie netplay."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SETTINGS,
   "Nastavenia"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_SETTINGS,
   "Konfigurácia programu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INFORMATION_LIST,
   "Informácie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INFORMATION_LIST_LIST,
   "Zobraziť systémové informácie."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CONFIGURATIONS_LIST,
   "Konfiguračný Súbor"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CONFIGURATIONS_LIST,
   "Spravovanie a vytvárenie konfiguračných súborov."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_HELP_LIST,
   "Nápoveda"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_HELP_LIST,
   "Zisti viac o tom ako program funguje."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RESTART_RETROARCH,
   "Reštartovať RetroArch"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_RESTART_RETROARCH,
   "Reštartuje sa program."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QUIT_RETROARCH,
   "Vypnúť RetroArch"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_QUIT_RETROARCH,
   "Ukončiť program."
   )

/* Main Menu > Load Core */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DOWNLOAD_CORE,
   "Stiahnuť Jadro (Core)"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_DOWNLOAD_CORE,
   "Stiahnutie a inštalácia jadra z online aktualizácii."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SIDELOAD_CORE_LIST,
   "Nainštalovať alebo obnoviť jadro"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_SIDELOAD_CORE_LIST,
   "Inštalácia alebo obnova jadra zo \"Stiahnuté\" zložky."
   )
MSG_HASH( /* FIXME Maybe add a description? */
   MENU_ENUM_LABEL_VALUE_START_VIDEO_PROCESSOR,
   "Spustiť Video Procesor"
   )
MSG_HASH( /* FIXME Maybe add a description? */
   MENU_ENUM_LABEL_VALUE_START_NET_RETROPAD,
   "Spustiť Diaľkový RetroPad"
   )

/* Main Menu > Load Content */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_FAVORITES,
   "Počiatočný priečinok"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DOWNLOADED_FILE_DETECT_CORE_LIST,
   "Prevzaté"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_OPEN_ARCHIVE,
   "Prehľadávať Archív"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_LOAD_ARCHIVE,
   "Načítať Archív"
   )

/* Main Menu > Load Content > Playlists */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_GOTO_FAVORITES,
   "Obľúbené"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_GOTO_FAVORITES,
   "Obsah pridaný do \"Obľúbené\" sa zobrazí tu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_GOTO_MUSIC,
   "Hudba"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_GOTO_MUSIC,
   "Hudba ktorá bola naposledy prehrávaná sa zobrazí tu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_GOTO_IMAGES,
   "Obrázky"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_GOTO_IMAGES,
   "Obrázky ktoré boly naposledy zobrazené sa zobrazí tu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_GOTO_VIDEO,
   "Videa"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_GOTO_VIDEO,
   "Videa ktoré boli naposledy prehrávané sa zobrazí tu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_GOTO_EXPLORE,
   "Preskúmať"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_GOTO_EXPLORE,
   "Prehľadať obsah ktorý sa zhoduje s databázou prostredníctvom kategorizovaného vyhľadavácieho rozhrania."
   )

/* Main Menu > Online Updater */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_UPDATER_LIST,
   "Stiahnutie Jadier"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_UPDATE_INSTALLED_CORES,
   "Aktualizácia Stiahnutých Jadier"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_UPDATE_INSTALLED_CORES,
   "Aktualizácia všetkých nainštalovaných jadier na najnovšiu dostupnú verziu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SWITCH_INSTALLED_CORES_PFD,
   "Prepnúť na jadrá z Play Store"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_SWITCH_INSTALLED_CORES_PFD,
   "Nahradí všetky staršie a ručne inštalované jadrá najnovšími verziami z Play Store, ak sú dostupné."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_THUMBNAILS_UPDATER_LIST,
   "Aktualizácia Miniatúr"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_THUMBNAILS_UPDATER_LIST,
   "Stiahnutie kompletného balíčku miniatúr pre vybraný systém."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_PL_THUMBNAILS_UPDATER_LIST,
   "Aktualizácia Zoznamu Miniatúr"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_PL_THUMBNAILS_UPDATER_LIST,
   "Stiahnutie miniatúr pre položky vo vybranom zozname."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DOWNLOAD_CORE_CONTENT,
   "Stiahnutie Obsahu"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_UPDATE_CORE_INFO_FILES,
   "Aktualizácia Info Sůborov Jadra"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_UPDATE_ASSETS,
   "Aktualizácia Assets"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_UPDATE_AUTOCONFIG_PROFILES,
   "Aktualizácia Profilov Ovládačov"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_UPDATE_CHEATS,
   "Aktualizácia Cheatov"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_UPDATE_DATABASES,
   "Aktualizácia Databáz"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_UPDATE_OVERLAYS,
   "Aktualizácia Prekrívacich Prvkov"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_UPDATE_GLSL_SHADERS,
   "Aktualizácia GLSL Shaders"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_UPDATE_CG_SHADERS,
   "Aktualizácia Cg Shaders"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_UPDATE_SLANG_SHADERS,
   "Aktualizácia Slang Shaders"
   )

/* Main Menu > Information */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFORMATION,
   "Informácie Jadra"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CORE_INFORMATION,
   "Zobraziť informácie týkajúce sa aplikácie/jadra."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DISC_INFORMATION,
   "Informácie disku"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_DISC_INFORMATION,
   "Zobraziť informácie vloženého CD/DVD disku."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_NETWORK_INFORMATION,
   "Sieťové informácie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_NETWORK_INFORMATION,
   "Zobraziť sieťové zariadenia a súvisiace IP adresy."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFORMATION,
   "Systémové informácie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_SYSTEM_INFORMATION,
   "Zobraziť informácie špecifické pre zariadenie."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DATABASE_MANAGER,
   "Správca databázy"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_DATABASE_MANAGER,
   "Zobraziť databázy."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CURSOR_MANAGER,
   "Správca kurzoru"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CURSOR_MANAGER,
   "Zobraziť predchádzajúce vyhľadávania."
   )

/* Main Menu > Information > Core Information */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFO_CORE_NAME,
   "Názov Jadra"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFO_CORE_LABEL,
   "Štítok Jadra"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFO_SYSTEM_NAME,
   "Názov systému"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFO_SYSTEM_MANUFACTURER,
   "Výrobca systému"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFO_CATEGORIES,
   "Kategórie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFO_AUTHORS,
   "Autori"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFO_PERMISSIONS,
   "Povolenia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFO_LICENSES,
   "Licencia(ie)"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFO_SUPPORTED_EXTENSIONS,
   "Podporované rozšírenia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFO_REQUIRED_HW_API,
   "Požadované grafické API"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INFO_FIRMWARE,
   "Firmvér(y)"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_LOCK,
   "Uzamknúť Nainštalované Jadro"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CORE_LOCK,
   "Zabráňte úprave aktuálne nainštalovaného jadra. Môže sa použiť na zabránenie nechceným aktualizáciám, keď obsah vyžaduje špecifickú verziu jadra (napr. Sady Arcade ROM)."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_DELETE,
   "Vymazať Jadro"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CORE_DELETE,
   "Vymaže zvolené jadro z disku."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_CREATE_BACKUP,
   "Zálohovať Jadro"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CORE_CREATE_BACKUP,
   "Vytvorí sa archivovaná záloha aktuálne nainštalovaného jadra."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_RESTORE_BACKUP_LIST,
   "Obnova Zálohy"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CORE_RESTORE_BACKUP_LIST,
   "Nainštalujte predchádzajúca verzia jadra, zo zoznamu archivovaných záloh."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_DELETE_BACKUP_LIST,
   "Vymazať Zálohu"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CORE_DELETE_BACKUP_LIST,
   "Odstráni sa súbor zo zoznamu archivovaných záloh."
   )

/* Main Menu > Information > System Information */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_BUILD_DATE,
   "Dátum zostavenia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_GIT_VERSION,
   "Verzia Git"
   )
MSG_HASH( /* FIXME Should be MENU_LABEL_VALUE */
   MSG_COMPILER,
   "Kompiler"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_CPU_MODEL,
   "Model CPU"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_CPU_FEATURES,
   "Vlastnosti CPU"
   )
MSG_HASH( /* FIXME Colon should be handled in menu_display.c like the rest */
   MENU_ENUM_LABEL_VALUE_CPU_ARCHITECTURE,
   "Architektúra CPU:"
   )
MSG_HASH( /* FIXME Colon should be handled in menu_display.c like the rest */
   MENU_ENUM_LABEL_VALUE_CPU_CORES,
   "CPU Jadrá:"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CPU_CORES,
   "Množstvo jadier CPU."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FRONTEND_IDENTIFIER,
   "Identifikátor klientskeho rozhrania"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FRONTEND_OS,
   "Operačný Systém"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE,
   "Zdroj Energie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_MM_WIDTH,
   "Šírka Obrazovky (mm)"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_MM_HEIGHT,
   "Výška Obrazovky (mm)"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_DPI,
   "DPI Obrazovky"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_LIBRETRODB_SUPPORT,
   "Podpora LibretroDB"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OVERLAY_SUPPORT,
   "Podpora Prekrývania"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_COMMAND_IFACE_SUPPORT,
   "Podpora Príkazového Rozhrania"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_NETWORK_COMMAND_IFACE_SUPPORT,
   "Podpora Sieťového Príkazového Rozhrania"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_NETWORK_REMOTE_SUPPORT,
   "Podpora Sieťového Ovládača"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_COCOA_SUPPORT,
   "Cocoa Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RPNG_SUPPORT,
   "PNG (RPNG) Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RJPEG_SUPPORT,
   "JPEG (RJPEG) Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RBMP_SUPPORT,
   "BMP (RBMP) Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RTGA_SUPPORT,
   "TGA (RTGA) Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_SDL_SUPPORT,
   "SDL 1.2 Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_SDL2_SUPPORT,
   "SDL 2 Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_VULKAN_SUPPORT,
   "Vulkan Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_METAL_SUPPORT,
   "Metal Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENGL_SUPPORT,
   "OpenGL Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENGLES_SUPPORT,
   "OpenGL ES Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_THREADING_SUPPORT,
   "Threading Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_KMS_SUPPORT,
   "KMS/EGL Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_UDEV_SUPPORT,
   "udev Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENVG_SUPPORT,
   "OpenVG Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_EGL_SUPPORT,
   "EGL Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_X11_SUPPORT,
   "X11 Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_WAYLAND_SUPPORT,
   "Wayland Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_XVIDEO_SUPPORT,
   "XVideo Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_ALSA_SUPPORT,
   "ALSA Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OSS_SUPPORT,
   "OSS Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENAL_SUPPORT,
   "OpenAL Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENSL_SUPPORT,
   "OpenSL Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RSOUND_SUPPORT,
   "RSound Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_ROARAUDIO_SUPPORT,
   "RoarAudio Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_JACK_SUPPORT,
   "JACK Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_PULSEAUDIO_SUPPORT,
   "PulseAudio Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_COREAUDIO_SUPPORT,
   "CoreAudio Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_COREAUDIO3_SUPPORT,
   "CoreAudio V3 Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DSOUND_SUPPORT,
   "DirectSound Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_WASAPI_SUPPORT,
   "WASAPI Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_XAUDIO2_SUPPORT,
   "XAudio2 Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_ZLIB_SUPPORT,
   "zlib Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_7ZIP_SUPPORT,
   "7zip Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DYLIB_SUPPORT,
   "Podpora Dynamickej Knižnice"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DYNAMIC_SUPPORT,
   "Dynamický beh programu načítavania Libetro Knižnice"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_CG_SUPPORT,
   "Cg Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_GLSL_SUPPORT,
   "GLSL Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_HLSL_SUPPORT,
   "HLSL Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_SDL_IMAGE_SUPPORT,
   "Podpora SDL Obrazca"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FFMPEG_SUPPORT,
   "FFmpeg Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_MPV_SUPPORT,
   "mpv Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_CORETEXT_SUPPORT,
   "CoreText Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FREETYPE_SUPPORT,
   "FreeType Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_STB_TRUETYPE_SUPPORT,
   "STB TrueType Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_NETPLAY_SUPPORT,
   "Netplay (Peer-to-Peer) Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_PYTHON_SUPPORT,
   "Python (Podpora Skriptov v Shaders) Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_V4L2_SUPPORT,
   "Video4Linux2 Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_LIBUSB_SUPPORT,
   "libusb Podpora"
   )

/* Main Menu > Information > Database Manager */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DATABASE_SELECTION,
   "Výber databázy"
   )

/* Main Menu > Information > Database Manager > Information */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_NAME,
   "Názov"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_DESCRIPTION,
   "Popis"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_GENRE,
   "Žáner"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_PUBLISHER,
   "Vydavateľ"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_DEVELOPER,
   "Vývojár"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ORIGIN,
   "Pôvod"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_FRANCHISE,
   "Franšíza"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_TGDB_RATING,
   "TGDB Hodnotenie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_FAMITSU_MAGAZINE_RATING,
   "Famitsu Magazine Hodnotenie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_EDGE_MAGAZINE_REVIEW,
   "Edge Magazine Recenzia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_EDGE_MAGAZINE_RATING,
   "Edge Magazine Hodnotenie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_EDGE_MAGAZINE_ISSUE,
   "Edge Magazine Problém"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_RELEASE_MONTH,
   "Mesiac dátumu vydania"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_RELEASE_YEAR,
   "Rok dátumu vydania"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_BBFC_RATING,
   "BBFC Hodnotenie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ESRB_RATING,
   "ESRB Hodnotenie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ELSPA_RATING,
   "ELSPA Hodnotenie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_PEGI_RATING,
   "PEGI Hodnotenie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ENHANCEMENT_HW,
   "Vylepšovací Hardvér"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_CERO_RATING,
   "CERO Hodnotenie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_SERIAL,
   "Sériové Č."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ANALOG,
   "Analógová Podpora"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_RUMBLE,
   "Podpora Vibrácíi"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RDB_ENTRY_COOP,
   "Co-op Podpora"
   )

/* Main Menu > Configuration File */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CONFIGURATIONS,
   "Načítať konfiguráciu"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RESET_TO_DEFAULT_CONFIG,
   "Resetovať do Predvolených Nastavení"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_RESET_TO_DEFAULT_CONFIG,
   "Nastavenia sa resetujú do pôvodných hodnôt."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SAVE_CURRENT_CONFIG,
   "Uložiť Aktuálne Nastavenia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SAVE_NEW_CONFIG,
   "Uložiť Nové Nastavenia"
   )

/* Main Menu > Help */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_HELP_CONTROLS,
   "Menu Základných Ovládacích Prvkov"
   )

/* Main Menu > Help > Basic Menu Controls */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_SCROLL_UP,
   "Posunúť hore"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_SCROLL_DOWN,
   "Posunúť dolu"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_CONFIRM,
   "Potvrdiť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_START,
   "Štart"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_TOGGLE_MENU,
   "Otvoriť / Zatvoriť hlavné menu"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_QUIT,
   "Ukončiť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_TOGGLE_KEYBOARD,
   "Otvoriť / Zatvoriť Klávesnicu"
   )

/* Settings */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DRIVER_SETTINGS,
   "Ovládače"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_DRIVER_SETTINGS,
   "Zmeniť ovládače používané systémom."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_SETTINGS,
   "Zmena nastavení video výstupu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_SETTINGS,
   "Zvuk"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_AUDIO_SETTINGS,
   "Zmena nastavení audio výstupu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_SETTINGS,
   "Vstup"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_SETTINGS,
   "Zmeniť nastavenia ovládača, klávesnice a myši."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_LATENCY_SETTINGS,
   "Odozva"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_LATENCY_SETTINGS,
   "Zmeňte nastavenia týkajúce sa videa, zvuku a vstupného oneskorenia."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_SETTINGS,
   "Jadro"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CORE_SETTINGS,
   "Zmena nastavenia jadra."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CONFIGURATION_SETTINGS,
   "Konfigurácia"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CONFIGURATION_SETTINGS,
   "Zmena predvolených nastavení konfiguračných súborov."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SAVING_SETTINGS,
   "Ukládanie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_SAVING_SETTINGS,
   "Zmena nastavenia ukládania."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_LOGGING_SETTINGS,
   "Prihlasovanie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_LOGGING_SETTINGS,
   "Zmena nastavení prihlasovania."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MENU_FILE_BROWSER_SETTINGS,
   "Správca súborov"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_MENU_FILE_BROWSER_SETTINGS,
   "Zmeniť nastavenia správcu súborov."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_FRAME_THROTTLE_SETTINGS,
   "Zmena nastavení pretočenia, zrýchlenia a spomalenia."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RECORDING_SETTINGS,
   "Záznam"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_RECORDING_SETTINGS,
   "Zmena nastavenia záznamu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_ONSCREEN_DISPLAY_SETTINGS,
   "Upraviť zobrazenie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_ONSCREEN_DISPLAY_SETTINGS,
   "Zmena prekrytia displeja, prekrytia klávesnice a nastavenia upozornení na obrazovke."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_USER_INTERFACE_SETTINGS,
   "Užívateľské rozhranie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_USER_INTERFACE_SETTINGS,
   "Zmena nastavení užívateľského rozhrania."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AI_SERVICE_SETTINGS,
   "Služba AI"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_AI_SERVICE_SETTINGS,
   "Zmeniť nastavenia AI služby (preklad/prevod textu na reč/ostatné)."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_ACCESSIBILITY_SETTINGS,
   "Zjednodušenie prístupu"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_ACCESSIBILITY_SETTINGS,
   "Zmeniť nastavenia moderátora."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_POWER_MANAGEMENT_SETTINGS,
   "Správa napájania"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_POWER_MANAGEMENT_SETTINGS,
   "Zmeniť nastavenia správy napájania."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RETRO_ACHIEVEMENTS_SETTINGS,
   "Úspechy"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_RETRO_ACHIEVEMENTS_SETTINGS,
   "Zmeniť nastavenia úspechov."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_NETWORK_SETTINGS,
   "Sieť"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_NETWORK_SETTINGS,
   "Zmeniť nastavenia servera a siete."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_PLAYLIST_SETTINGS,
   "Hracie zoznamy"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_PLAYLIST_SETTINGS,
   "Zmeniť nastavenia zoznamov."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_USER_SETTINGS,
   "Používateľ"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_USER_SETTINGS,
   "Zmeniť účet, užívateľské meno a nastavenie jazyka."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DIRECTORY_SETTINGS,
   "Priečinok"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_DIRECTORY_SETTINGS,
   "Zmeniť východzie zložky umiestnenia súborov."
   )

/* Settings > Drivers */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_DRIVER,
   "Vstup"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_DRIVER,
   "Používaný vstupný ovládač. Niektoré ovládače videa vynucujú iný vstupný ovládač."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_JOYPAD_DRIVER,
   "Ovládač"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_JOYPAD_DRIVER,
   "Používaný ovládač herného ovládača."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_DRIVER,
   "Použiť ovládač videa."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_DRIVER,
   "Zvuk"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_AUDIO_DRIVER,
   "Použiť ovládač zvuku."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_RESAMPLER_DRIVER,
   "Vzorkovač zvuku"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_AUDIO_RESAMPLER_DRIVER,
   "Použiť ovládač vzorkovača zvuku."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CAMERA_DRIVER,
   "Kamera"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CAMERA_DRIVER,
   "Použiť ovládač kamery."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_BLUETOOTH_DRIVER,
   "Používaný ovládač Bluetooth."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_WIFI_DRIVER,
   "Používaný ovládač Wi-Fi."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_LOCATION_DRIVER,
   "Poloha"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_LOCATION_DRIVER,
   "Použiť ovládač polohy."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_MENU_DRIVER,
   "Použiť ovládač ponuky."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RECORD_DRIVER,
   "Nahrávanie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_RECORD_DRIVER,
   "Použiť ovládač nahrávania."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_MIDI_DRIVER,
   "Použiť ovládač MIDI."
   )

/* Settings > Video */

MSG_HASH(
   MENU_ENUM_SUBLABEL_CRT_SWITCHRES_SETTINGS,
   "Výstup signálu nízkeho rozlíšenia na použitie s CRT obrazovkami."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_OUTPUT_SETTINGS,
   "Výstup"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_OUTPUT_SETTINGS,
   "Zmena nastavení video výstupu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_FULLSCREEN_MODE_SETTINGS,
   "Režim na celú obrazovku"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_FULLSCREEN_MODE_SETTINGS,
   "Zmena nastavení režimu na celú obrazovku."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_WINDOWED_MODE_SETTINGS,
   "Režim v Okne"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_WINDOWED_MODE_SETTINGS,
   "Zmena nastavení režimu v okne."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_SCALING_SETTINGS,
   "Škálovanie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_SCALING_SETTINGS,
   "Zmena nastavenia škálovania."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_SYNCHRONIZATION_SETTINGS,
   "Synchronizácia"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_SYNCHRONIZATION_SETTINGS,
   "Zmena nastavenia synchronizácie videa."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SUSPEND_SCREENSAVER_ENABLE,
   "Pozastaviť šetrič obrazovky"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_SUSPEND_SCREENSAVER_ENABLE,
   "Zabrániť aktivácii systémového šetriča obrazovky."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_THREADED,
   "Vláknové Video"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_THREADED,
   "Zlepšuje výkon za cenu oneskorenia a vyššieho zasekávania videa. Používajte, iba ak nemôžete získať plnú rýchlosť."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_BLACK_FRAME_INSERTION,
   "Vloženie Čierneho Rámu"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_BLACK_FRAME_INSERTION,
   "Vloží medzi snímky čierny rám. Užitočné na niektorých obrazovkách s vysokou obnovovacou frekvenciou na odstránenie duchov."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_GPU_SCREENSHOT,
   "GPU Snímok obrazovky"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_GPU_SCREENSHOT,
   "Snímka obrazovky zachytená z tieňového materiálu GPU, ak je k dispozícii."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_SMOOTH,
   "Bilineárne Filtrovanie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_SMOOTH,
   "Pridá do obrázka mierne rozostrenie, aby sa odstránila hrana z okrajov pevných pixelov. Táto možnosť má veľmi malý vplyv na výkon."
   )
#if defined(DINGUX)
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_DINGUX_IPU_FILTER_TYPE,
   "Interpolácia obrazu"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_DINGUX_IPU_FILTER_TYPE,
   "Vybrať metódu interpolácie obrazu pri škálovaní obsahu pomocou interného IPU. Pri použití videofiltrov poháňané procesorom sa odporúča „bikubická“ alebo „bilineárna“. Táto voľba nemá žiadny vplyv na výkon."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_DINGUX_IPU_FILTER_BICUBIC,
   "Bikubická"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_DINGUX_IPU_FILTER_BILINEAR,
   "Bilineárna"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_DINGUX_IPU_FILTER_NEAREST,
   "Najbližší sused"
   )
#if defined(RS90) || defined(MIYOO)
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_DINGUX_RS90_SOFTFILTER_TYPE,
   "Interpolácia obrazu"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_DINGUX_RS90_SOFTFILTER_POINT,
   "Najbližší sused"
   )
#endif
#endif
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_DELAY,
   "Auto-Shader Oneskorenie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_SHADER_DELAY,
   "Oneskoriť automatické načítavanie shaderu (v ms). Môže obísť grafické chyby pri používaní softvéru na zachytávania obrazovky."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_FILTER,
   "Použiť videofilter poháňaný CPU. Môže mať veľký dopad na výkon. Niektoré videofiltre môžu fungovať iba s jadrami, ktoré používajú 32-bitovú alebo 16-bitovú hĺbku farby."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_FILTER_REMOVE,
   "Odstrániť videofilter"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_FILTER_REMOVE,
   "Zrušiť všetky aktívne videofiltre poháňané CPU."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_NOTCH_WRITE_OVER,
   "Povoliť plnú obrazovku naprieč výrezu pre kameru na zariadeniach Android"
)

/* Settings > Video > CRT SwitchRes */

MSG_HASH(
   MENU_ENUM_SUBLABEL_CRT_SWITCH_RESOLUTION,
   "Iba pre CRT displeje. Pokúša sa použiť presné rozlíšenie jadra/hry a obnovovaciu frekvenciu."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CRT_SWITCH_RESOLUTION_SUPER,
   "Prepínanie medzi natívnym a ultraširokým super rozlíšením."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CRT_SWITCH_X_AXIS_CENTERING,
   "Centrovanie X-Axis"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CRT_SWITCH_X_AXIS_CENTERING,
   "Ak obraz nie je správne vycentrovaný na displeji, môžete prenastaviť tieto možnosti."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CRT_SWITCH_PORCH_ADJUST,
   "Porch Nastevenie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CRT_SWITCH_PORCH_ADJUST,
   "Pomocou týchto možností môžete upravovať nastavenia verandy a meniť veľkosť obrázka."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CRT_SWITCH_HIRES_MENU,
   "Použiť ponuku vo vysokom rozlíšení"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CRT_SWITCH_RESOLUTION_USE_CUSTOM_REFRESH_RATE,
   "Vlastná obnovovacia frekvencia"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_CRT_SWITCH_RESOLUTION_USE_CUSTOM_REFRESH_RATE,
   "V prípade potreby použite vlastnú obnovovaciu frekvenciu uvedenú v konfiguračnom súbore."
   )

/* Settings > Video > Output */

MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_MONITOR_INDEX,
   "Vybrať obrazovku, ktorá sa použije."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_ROTATION,
   "Video Rotácia"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_ROTATION,
   "Vynúti určitú rotáciu videa. Rotácia sa pridá k rotáciám, ktoré nastavuje jadro."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SCREEN_ORIENTATION,
   "Orientácia Obrazovky"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_SCREEN_ORIENTATION,
   "Vynúti určitú orientáciu obrazovky z operačného systému."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_GPU_INDEX,
   "Vybrať, ktorú grafickú kartu použiť."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_REFRESH_RATE,
   "Vertikálna Obnovovacia Frekvencia"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_REFRESH_RATE,
   "Vertikálna obnovovacia frekvencia obrazovky. Použije sa na výpočet vhodného vzorkovania vstupu audia.\nIgnorované, ak je povolené 'Vláknové video'."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_REFRESH_RATE_AUTO,
   "Odhadovaná Miera Obnovenia Obrazovky"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_REFRESH_RATE_AUTO,
   "Odhadovaná obnovovacia frekvencia obrazovky v Hz."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_REFRESH_RATE_POLLED,
   "Nastaviť obnovovaciu frekvenciu hlásenú obrazovkou"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_REFRESH_RATE_POLLED,
   "Obnovovacia frekvencia hlásená ovládačom obrazu."
   )
#if defined(DINGUX) && defined(DINGUX_BETA)
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_DINGUX_REFRESH_RATE,
   "Vertikálna obnovovacia frekvencia"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_DINGUX_REFRESH_RATE,
   "Nastaviť vertikálnu obnovovaciu frekvenciu. '50 Hz' umožní plynulé video pre obsah v režime PAL."
   )
#endif
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_FORCE_SRGB_DISABLE,
   "Natvrdo zakázať sRGB FBO"
   )

/* Settings > Video > Fullscreen Mode */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_FULLSCREEN,
   "Spustiť v režime celej obrazovky"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_FULLSCREEN,
   "Spustiť v režime na celú obrazovku. Môže byť zmenené počas behu. Môže byť prepísané prepínačom príkazového riadku."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_WINDOWED_FULLSCREEN,
   "Maximalizované okno"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_WINDOWED_FULLSCREEN,
   "Ak v režime na celú obrazovku, uprednostniť okno na celú obrazovku a zabrániť prepínaniu zobrazovacích režimov."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_FULLSCREEN_X,
   "Šírka na celú obrazovku"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_FULLSCREEN_X,
   "Nastaviť vlastnú šírku pre celoobrazovkový režim bez okna. Prázdne použije rozlíšenie pracovnej plochy."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_FULLSCREEN_Y,
   "Výška na celú obrazovku"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_FULLSCREEN_Y,
   "Nastaviť vlastnú výšku pre celoobrazovkový režim bez okna. Prázdne použije rozlíšenie pracovnej plochy."
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_FORCE_RESOLUTION,
   "Vynútiť rozlíšenie na celú obrazovku, ak nastavené na 0, použije sa pevná hodnota 3840 x 2160."
   )

/* Settings > Video > Windowed Mode */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_SCALE,
   "Škálovanie okna"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_WINDOW_OPACITY,
   "Priehľadnosť okna"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_WINDOW_SHOW_DECORATIONS,
   "Zobraziť dekorácie okien"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_WINDOW_SAVE_POSITION,
   "Zapamätať pozíciu a veľkosť okna"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_WINDOW_WIDTH,
   "Šírka okna"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_WINDOW_WIDTH,
   "Nastaviť vlastnú šírku okna."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_WINDOW_HEIGHT,
   "Výška okna"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_WINDOW_HEIGHT,
   "Nastaviť vlastnú výšku okna."
   )

/* Settings > Video > Scaling */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_SCALE_INTEGER,
   "Celočíselné škálovanie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_ASPECT_RATIO_INDEX,
   "Pomer strán"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_ASPECT_RATIO,
   "Vlastný pomer strán"
   )
#if defined(DINGUX)
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_DINGUX_IPU_KEEP_ASPECT,
   "Zachovať pomer strán"
   )
#endif
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_X,
   "Vlastný pomer strán (pozícia X)"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_Y,
   "Vlastný pomer strán (pozícia Y)"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_WIDTH,
   "Vlastný pomer strán (šírka)"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_VIEWPORT_CUSTOM_WIDTH,
   "Šírka priezoru ak je vybraná možnosť 'Vlastný pomer strán'."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_HEIGHT,
   "Vlastný pomer strán (výška)"
   )

/* Settings > Video > HDR */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_HDR_ENABLE,
   "Povoliť HDR"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_HDR_ENABLE,
   "Povoliť HDR, ak to displej podporuje."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_HDR_MAX_NITS,
   "Najvyšší jas"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_HDR_MAX_NITS,
   "Nastaviť najvyšší jas (v cd/m2), ktorú vie displej zobraziť. Pozri stránku RTINGS.com na zistenie najvyššieho jasu displeja."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_HDR_CONTRAST,
   "Kontrast"
   )

/* Settings > Video > Synchronization */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_VSYNC,
   "Vertikálna synchronizácia (VSync)"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_VSYNC,
   "Synchronizovať video výstup grafickej karty s obnovovacou frekvenciou obrazovky. Odporúčané."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_FRAME_DELAY,
   "Oneskorenie snímky"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_HARD_SYNC,
   "Pevná synchronizácia GPU"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_VIDEO_HARD_SYNC,
   "Napevno synchronizovať CPU a GPU. Znižuje latenciu na úkor výkonu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_HARD_SYNC_FRAMES,
   "Snímky pre pevnú synchronizáciu GPU"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VRR_RUNLOOP_ENABLE,
   "Synchronizovať na presnú rýchlosť sníkov obsahu (G-Sync, FreeSync)"
   )

/* Settings > Audio */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_OUTPUT_SETTINGS,
   "Výstup"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_AUDIO_OUTPUT_SETTINGS,
   "Zmena nastavení audio výstupu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_SYNCHRONIZATION_SETTINGS,
   "Synchronizácia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_MIXER_SETTINGS,
   "Zmiešavač"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_MUTE,
   "Stlmiť"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_AUDIO_MUTE,
   "Stlmiť zvuk."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_DSP_PLUGIN,
   "DPS zásuvný modul"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_DSP_PLUGIN_REMOVE,
   "Odstrániť DSP rozšírenie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_AUDIO_DSP_PLUGIN_REMOVE,
   "Odobrať aktívne zvukové DSP rozšírenie."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_WASAPI_EXCLUSIVE_MODE,
   "WASAPI exkluzívny mód"
   )

/* Settings > Audio > Output */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_ENABLE,
   "Zvuk"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_AUDIO_ENABLE,
   "Povoliť výstup zvuku."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_DEVICE,
   "Zariadenie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_LATENCY,
   "Oneskorenie zvuku (ms)"
   )

/* Settings > Audio > Resampler */


/* Settings > Audio > Synchronization */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_SYNC,
   "Synchronizácia"
   )

/* Settings > Audio > MIDI */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MIDI_INPUT,
   "Vstup"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_MIDI_INPUT,
   "Zvoliť vstupné zariadenie."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MIDI_OUTPUT,
   "Výstup"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_MIDI_OUTPUT,
   "Zvoliť výstupné zariadenie."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MIDI_VOLUME,
   "Hlasitosť"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_MIDI_VOLUME,
   "Nastaviť hlasitosť výstupu (%)."
   )

/* Settings > Audio > Mixer Settings > Mixer Stream */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MIXER_ACTION_PLAY,
   "Spustiť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MIXER_ACTION_PLAY_LOOPED,
   "Prehrávať (v slučke)"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MIXER_ACTION_PLAY_SEQUENTIAL,
   "Prehrávať (v poradí)"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MIXER_ACTION_STOP,
   "Zastaviť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MIXER_ACTION_REMOVE,
   "Odstrániť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MIXER_ACTION_VOLUME,
   "Hlasitosť"
   )

/* Settings > Audio > Menu Sounds */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_AUDIO_ENABLE_MENU,
   "Zmiešavač"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_AUDIO_ENABLE_MENU,
   "Prehrávať súčasne zvukové toky aj v ponuke."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MENU_SOUND_OK,
   "Povoliť zvukový efekt potvrdenia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MENU_SOUND_CANCEL,
   "Povoliť zvukový efekt zrušenia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MENU_SOUND_NOTICE,
   "Povoliť zvukový efekt oznámenia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MENU_SOUND_BGM,
   "Povoliť zvuk hudby na pozadí"
   )

/* Settings > Input */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_MAX_USERS,
   "Maximum užívateľov"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_MAX_USERS,
   "Maximálny počet užívateľov podporovaný v RetroArch."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR,
   "Správanie dopytu"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_POLL_TYPE_BEHAVIOR,
   "Ovplyvniť správanie dopytu vstupu v RetroArch. Nastavenie 'Skoré' alebo 'Neskoré' môže spôsobiť nižšiu latenciu v závislosti na konfigurácii."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_REMAP_BINDS_ENABLE,
   "Zmeniť ovládanie pre toto jadro"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_AUTODETECT_ENABLE,
   "Automatická konfigurácia"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_AUTODETECT_ENABLE,
   "Automaticky konfigurovať herné ovládače, pre ktoré existuje profil, štýl Plug-and-Play."
   )
#if defined(HAVE_DINPUT) || defined(HAVE_WINRAWINPUT)
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_NOWINKEY_ENABLE,
   "Zakázať klávesové skratky systému Windows (vyžaduje reštart)"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_NOWINKEY_ENABLE,
   "Zachytiť tieto klávesové kombinácie v aplikácii."
   )
#endif
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_SENSORS_ENABLE,
   "Vstup prídavného senzoru"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_SENSORS_ENABLE,
   "Povoliť vstup zo senzorov zrýchlenia, naklonenia a osvetlenia, ak ich podporuje súčasný hardvér. Môže mať dopad na výkon a/alebo zvýšiť spotrebu na niektorých platformách."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_AUTO_GAME_FOCUS,
   "Automaticky povoliť režim 'Zameranie na hru'"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_AUTO_GAME_FOCUS_OFF,
   "VYP"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_AUTO_GAME_FOCUS_ON,
   "ZAP"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_AUTO_GAME_FOCUS_DETECT,
   "Zistiť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_TURBO_MODE,
   "Turbo mód"
   )

/* Settings > Input > Haptic Feedback/Vibration */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIBRATE_ON_KEYPRESS,
   "Vibrovať pri stlačení"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_ENABLE_DEVICE_VIBRATION,
   "Povoliť vibrovanie zariadenia (pre podporované jadrá)"
   )

/* Settings > Input > Menu Controls */


/* Settings > Input > Hotkeys */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_FULLSCREEN_TOGGLE_KEY,
   "Celá obrazovka (prepínanie)"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_META_FULLSCREEN_TOGGLE_KEY,
   "Prepína medzi režimom na celú obrazovku a zobrazení v okne."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_CLOSE_CONTENT_KEY,
   "Zavrieť obsah"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_META_CLOSE_CONTENT_KEY,
   "Zavrie aktuálny obsah. Akékoľvek neuložené zmeny sa môžu stratiť."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_QUIT_KEY,
   "Vypnúť RetroArch"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_META_QUIT_KEY,
   "Ukončí RetroArch, zaistí zapísanie všetky úložných dát a nastavení na disk."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_STATE_SLOT_PLUS,
   "Slot na uloženie stavu +"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_META_STATE_SLOT_PLUS,
   "Zvýši index aktuálne zvoleného slotu na ukladanie stavu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_STATE_SLOT_MINUS,
   "Slot na uloženie stavu -"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_META_STATE_SLOT_MINUS,
   "Zníži index aktuálne zvoleného slotu na ukladanie stavu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_REWIND,
   "Previnúť"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_META_REWIND_HOTKEY,
   "Previnie aktuálny obsah počas držania klávesy.\n'Podpora prevíjania' musí byť povolená."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_BSV_RECORD_TOGGLE,
   "Zaznamenanie vstupov (prepínanie)"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_META_BSV_RECORD_TOGGLE,
   "Zapína/vypína zaznamenávanie vstupov hrania v .bsv formáte."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_PAUSE_TOGGLE,
   "Prestávka (prepínanie)"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_META_PAUSE_TOGGLE,
   "Zapína/vypína pozastavenie bežiaceho obsahu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_FRAMEADVANCE,
   "Postupovanie po snímkach"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_META_FRAMEADVANCE,
   "Postúpiť o jednu snímku počas pozastaveného obsahu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_SEND_DEBUG_INFO,
   "Odoslať ladiace informácie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_META_SEND_DEBUG_INFO,
   "Odoslať diagnostické informácie o zariadení a nastaveniach RetroArch na naše serveri na analýzu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_AI_SERVICE,
   "Služba AI"
   )

/* Settings > Input > Port # Controls */


/* Settings > Latency */


/* Settings > Core */

#ifndef HAVE_DYNAMIC
#endif

/* Settings > Configuration */


/* Settings > Saving */


/* Settings > Logging */


/* Settings > File Browser */


/* Settings > Frame Throttle */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_REWIND_SETTINGS,
   "Previnúť"
   )

/* Settings > Frame Throttle > Rewind */


/* Settings > Frame Throttle > Frame Time Counter */


/* Settings > Recording */


/* Settings > On-Screen Display */


/* Settings > On-Screen Display > On-Screen Overlay */


#if defined(ANDROID)
#endif
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_OVERLAY_SHOW_INPUTS,
   "Zobraziť vstupy na prekrytí"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_OVERLAY_SHOW_INPUTS,
   "Zobraziť zachytené vstupy na prekrytí obrazovky. 'Dotykové' zvýrazni prvky prekrytie, ktoré boli stlačené/kliknuté. 'Fyzické (herný ovládač)' zvýrazní skutočný vstup poslaný jadru, zvyčajne z pripojeného herného ovládača/klávesnice."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_OVERLAY_SHOW_INPUTS_TOUCHED,
   "Dotykové"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_OVERLAY_SHOW_INPUTS_PHYSICAL,
   "Fyzické (herný ovládač)"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_OVERLAY_SHOW_INPUTS_PORT,
   "Zobraziť vstupy z portu"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_INPUT_OVERLAY_SHOW_INPUTS_PORT,
   "Zvoliť port vstupného zariadenia na sledovanie, keď je 'Zobraziť vstupy na prekrytí' nastavené na 'Fyzické (herný ovládať)'."
   )

/* Settings > On-Screen Display > Video Layout */


/* Settings > On-Screen Display > On-Screen Notifications */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_NOTIFICATION_SHOW_SCREENSHOT_DURATION_NORMAL,
   "Normálne"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_NOTIFICATION_SHOW_SCREENSHOT_DURATION_FAST,
   "Rýchlo"
   )

/* Settings > User Interface */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MENU_SCREENSAVER_ANIMATION_SNOW,
   "Sneh"
   )

/* Settings > User Interface > Menu Item Visibility */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QUICK_MENU_VIEWS_SETTINGS,
   "Rýchle Menu"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SETTINGS_VIEWS_SETTINGS,
   "Nastavenia"
   )
#ifdef HAVE_LAKKA
#endif
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MENU_ADD_CONTENT_ENTRY_DISPLAY_MAIN_TAB,
   "Hlavné Menu"
   )

/* Settings > User Interface > Menu Item Visibility > Quick Menu */


/* Settings > User Interface > Views > Settings */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SETTINGS_SHOW_DIRECTORY,
   "Zobraziť 'Adresár'"
   )

/* Settings > User Interface > Appearance */


/* Settings > AI Service */


/* Settings > Accessibility */


/* Settings > Power Management */

/* Settings > Achievements */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CHEEVOS_ENABLE,
   "Úspechy"
   )

/* Settings > Network */


/* Settings > Network > Updater */


/* Settings > Playlists */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_HISTORY_LIST_ENABLE,
   "História"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_PLAYLIST_SUBLABEL_CORE,
   "Jadro:"
   )

/* Settings > Playlists > Playlist Management */


/* Settings > User */


/* Settings > User > Privacy */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CAMERA_ALLOW,
   "Povoliť kameru"
   )

/* Settings > User > Accounts */


/* Settings > User > Accounts > RetroAchievements */


/* Settings > User > Accounts > YouTube */


/* Settings > User > Accounts > Twitch */


/* Settings > User > Accounts > Facebook Gaming */


/* Settings > Directory */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_ASSETS_DIRECTORY,
   "Prevzaté"
   )
MSG_HASH( /* FIXME Not RGUI specific */
   MENU_ENUM_LABEL_VALUE_RGUI_BROWSER_DIRECTORY,
   "Správca súborov"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_LIBRETRO_INFO_PATH,
   "Informácie o jadre"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CURSOR_DIRECTORY,
   "Kurzor"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_JOYPAD_AUTOCONFIG_DIR,
   "Profily herného ovládača"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_PLAYLIST_DIRECTORY,
   "Hracie zoznamy"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CACHE_DIRECTORY,
   "Vyrovnávacia pamäť"
   )

/* Music */

/* Music > Quick Menu */


/* Netplay */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_NETWORK_HOSTING_SETTINGS,
   "Hostiteľ"
   )

/* Netplay > Host */


/* Import Content */


/* Import Content > Scan File */


/* Import Content > Manual Scan */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MANUAL_CONTENT_SCAN_SYSTEM_NAME,
   "Názov systému"
   )

/* Explore tab */
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_EXPLORE_ALL,
   "Všetko"
   )

/* Playlist > Playlist Item */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RUN,
   "Spustiť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DELETE_ENTRY,
   "Odstrániť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INFORMATION,
   "Informácie"
   )

/* Playlist Item > Set Core Association */


/* Playlist Item > Information */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CONTENT_INFO_LABEL,
   "Názov"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CONTENT_INFO_CORE_NAME,
   "Jadro"
   )

/* Quick Menu */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CLOSE_CONTENT,
   "Zavrieť obsah"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_OPTIONS,
   "Nastavenia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_INPUT_REMAPPING_OPTIONS,
   "Ovládanie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CORE_CHEAT_OPTIONS,
   "Cheaty"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_ACHIEVEMENT_LIST,
   "Úspechy"
   )

/* Quick Menu > Options */


/* Quick Menu > Options > Manage Core Options */


/* - Legacy (unused) */

/* Quick Menu > Controls */


/* Quick Menu > Controls > Load Remap File */


/* Quick Menu > Cheats */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CHEAT_APPLY_CHANGES,
   "Použiť zmeny"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CHEAT,
   "Cheaty"
   )

/* Quick Menu > Cheats > Start or Continue Cheat Search */


/* Quick Menu > Cheats > Load Cheat File (Replace) */


/* Quick Menu > Cheats > Load Cheat File (Append) */


/* Quick Menu > Cheats > Cheat Details */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CHEAT_DESC,
   "Popis"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CHEAT_TYPE,
   "Typ"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CHEAT_VALUE,
   "Hodnota"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CHEAT_CODE,
   "Kód"
   )

/* Quick Menu > Disc Control */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DISK_TRAY_INSERT,
   "Vložte disk"
   )

/* Quick Menu > Shaders */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET,
   "Načítať"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET_SAVE,
   "Uložiť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET_REMOVE,
   "Odstrániť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SHADER_APPLY_CHANGES,
   "Použiť zmeny"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SCALE,
   "Škálovať"
   )

/* Quick Menu > Shaders > Save */




/* Quick Menu > Shaders > Remove */


/* Quick Menu > Shaders > Shader Parameters */


/* Quick Menu > Overrides */


/* Quick Menu > Achievements */


/* Quick Menu > Information */


/* Miscellaneous UI Items */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_BACK,
   "Späť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_DIRECTORY_NOT_FOUND,
   "Priečinok nebol nájdený."
   )

/* Settings Options */

MSG_HASH( /* FIXME Should be MENU_LABEL_VALUE */
   MSG_UNKNOWN_COMPILER,
   "Neznámy kompiler"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_NETPLAY_SHARE_DIGITAL_OR,
   "Zdielať"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_NETPLAY_SHARE_DIGITAL_VOTE,
   "Hlasovať"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_NETPLAY_SHARE_NONE,
   "Žiadne"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_PLAYLIST_ENTRY_REMOVE_ENABLE_NONE,
   "VYP"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_PLAYLIST_INLINE_CORE_DISPLAY_NEVER,
   "Nikdy"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_NONE,
   "Žiadne"
   )
MSG_HASH(
   MENU_ENUM_LABEL_RUMBLE_PORT_16,
   "Všetko"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_PLAYLIST_MANAGER_SORT_MODE_OFF,
   "Žiadne"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SCROLL_NORMAL,
   "Normálne"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SCROLL_FAST,
   "Rýchlo"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_ON,
   "ZAP"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_OFF,
   "VYP"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_YES,
   "Áno"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_NO,
   "Nie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_TRUE,
   "Pravda"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_FALSE,
   "Nepravda"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CHEEVOS_LOCKED_ENTRY,
   "Zamknuté"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_LINEAR,
   "Lineárny"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_LEFT_ANALOG,
   "Ľavý analóg"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_MOUSE_LEFT,
   "Myš 1"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_MOUSE_RIGHT,
   "Myš 2"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_MOUSE_MIDDLE,
   "Myš 3"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_MOUSE_BUTTON4,
   "Myš 4"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_MOUSE_BUTTON5,
   "Myš 5"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR_EARLY,
   "Skoro"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR_NORMAL,
   "Normálne"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR_LATE,
   "Neskoro"
   )

/* RGUI: Settings > User Interface > Appearance */


/* RGUI: Settings Options */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RGUI_THUMB_SCALE_BILINEAR,
   "Bilineárna"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RGUI_UPSCALE_NONE,
   "Žiadny"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RGUI_ASPECT_RATIO_LOCK_NONE,
   "VYP"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RGUI_ASPECT_RATIO_LOCK_INTEGER,
   "Celočíselné škálovanie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RGUI_PARTICLE_EFFECT_NONE,
   "VYP"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RGUI_PARTICLE_EFFECT_RAIN,
   "Dážď"
   )

/* XMB: Settings > User Interface > Appearance */


/* XMB: Settings Options */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SHADER_PIPELINE_SNOW,
   "Sneh"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_DARK,
   "Tmavý"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_LIGHT,
   "Svetlý"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_PLAIN,
   "Obyčajný"
   )

/* Ozone: Settings > User Interface > Appearance */


/* MaterialUI: Settings > User Interface > Appearance */


/* MaterialUI: Settings Options */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_BLUE,
   "Modrá"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_GREEN,
   "Zelená"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_RED,
   "Červená"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_TRANSITION_ANIM_FADE,
   "Zoslabiť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_TRANSITION_ANIM_NONE,
   "VYP"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MATERIALUI_THUMBNAIL_VIEW_PORTRAIT_DISABLED,
   "VYP"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MATERIALUI_THUMBNAIL_VIEW_LANDSCAPE_DISABLED,
   "VYP"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MATERIALUI_LANDSCAPE_LAYOUT_OPTIMIZATION_DISABLED,
   "VYP"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_MATERIALUI_LANDSCAPE_LAYOUT_OPTIMIZATION_ALWAYS,
   "ZAP"
   )

/* Qt (Desktop Menu) */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_MENU_FILE_LOAD_CORE,
   "&Načítať jadro..."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_MENU_EDIT,
   "&Upraviť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_MENU_VIEW,
   "&Zobrazenie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_MENU_VIEW_OPTIONS_THEME_DARK,
   "Tmavý"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_MENU_VIEW_OPTIONS_TITLE,
   "Nastavenia"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_MENU_HELP,
   "&Nápoveda"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_LOAD_CORE,
   "Načítať jadro"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_NAME,
   "Názov"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_TAB_PLAYLISTS,
   "Hracie zoznamy"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_TAB_FILE_BROWSER,
   "Správca súborov"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_TAB_FILE_BROWSER_UP,
   "Hore"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_CORE,
   "Jadro"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_CORE_INFO,
   "Informácie o jadre"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_INFORMATION,
   "Informácie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_ERROR,
   "Chyba"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_HIDE,
   "Skryť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_ZOOM,
   "Priblíženie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_VIEW,
   "Zobrazenie"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_VIEW_TYPE_ICONS,
   "Ikony"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_VIEW_TYPE_LIST,
   "Zoznam"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_MENU_SEARCH_CLEAR,
   "Vyčistiť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_PLAYLIST_ENTRY_NAME,
   "Názov:"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_PLAYLIST_ENTRY_PATH,
   "Cesta:"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_PLAYLIST_ENTRY_CORE,
   "Jadro:"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_EDIT,
   "Upraviť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_LOAD,
   "Načítať"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_SAVE,
   "Uložiť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_REMOVE,
   "Odstrániť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_QT_APPLY,
   "Použiť"
   )

/* Unsorted */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_LOAD_CONTENT_HISTORY,
   "História"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_RUN_MUSIC,
   "Spustiť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_USER,
   "Používateľ"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BROWSE_START,
   "Štart"
   )

/* Unused (Only Exist in Translation Files) */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CUSTOM_RATIO,
   "Vlastný pomer"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_HELP,
   "Nápoveda"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_HELP_WHAT_IS_A_CORE,
   "Čo je jadro?"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_HELP_SEND_DEBUG_INFO,
   "Odoslať ladiace informácie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_HELP_SEND_DEBUG_INFO,
   "Odoslať diagnostické informácie o zariadení a nastaveniach RetroArch na naše serveri na analýzu."
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CHEEVOS_DESCRIPTION,
   "Popis"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_CHEAT_START_SEARCH,
   "Začať vyhľadávanie pre nový cheat kód"
   )

/* Unused (Needs Confirmation) */

MSG_HASH( /* FIXME Seems related to MENU_ENUM_LABEL_VALUE_CORE_ASSETS_DIRECTORY, possible duplicate */
   MENU_ENUM_LABEL_VALUE_CORE_ASSETS_DIR,
   "Prevzaté"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_ASK_ARCHIVE,
   "Opýtať sa"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_CONFIRM,
   "Potvrdiť/OK"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_QUIT,
   "Ukončiť"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_SCROLL_UP,
   "Posunúť hore"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_START,
   "Predvolené"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_TOGGLE_KEYBOARD,
   "Otvoriť / Zatvoriť Klávesnicu"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_TOGGLE_MENU,
   "Otvoriť / Zatvoriť hlavné menu"
   )

/* Discord Status */


/* Notifications */

MSG_HASH( /* FIXME Should be MSG_ */
   MENU_ENUM_LABEL_VALUE_SIDELOAD_CORE_ERROR,
   "Inštalácia jadra zlyhala"
   )
MSG_HASH(
   MSG_NATIVE,
   "Natívne"
   )
MSG_HASH(
   MSG_DEVICE_DISCONNECTED_FROM_PORT,
   "Zariadenie odpojené z portu"
   )
MSG_HASH(
   MSG_UNKNOWN_NETPLAY_COMMAND_RECEIVED,
   "Prijatý neznámy netplay príkaz"
   )
MSG_HASH(
   MSG_FILE_ALREADY_EXISTS_SAVING_TO_BACKUP_BUFFER,
   "Súbor už existuje. Ukladám do záložného bufferu"
   )
MSG_HASH(
   MSG_WAITING_FOR_CLIENT,
   "Čakám na klienta..."
   )
MSG_HASH(
   MSG_NETPLAY_YOU_HAVE_LEFT_THE_GAME,
   "Opustili ste hru"
   )
MSG_HASH(
   MSG_NETPLAY_YOU_HAVE_JOINED_AS_PLAYER_N,
   "Pripojili ste as ako hráč %u"
   )
MSG_HASH(
   MSG_NETPLAY_YOU_HAVE_JOINED_WITH_INPUT_DEVICES_S,
   "Pripojili ste sa so vstupnými zariadeniami %.*s"
   )
MSG_HASH(
   MSG_NETPLAY_PLAYER_S_LEFT,
   "Hráč %.*s opustil hru"
   )
MSG_HASH(
   MSG_NETPLAY_S_HAS_JOINED_AS_PLAYER_N,
   "%.*s sa pripojil ako hráč %u"
   )
MSG_HASH(
   MSG_NETPLAY_S_HAS_JOINED_WITH_INPUT_DEVICES_S,
   "%.*s sa pripojil so vstupnými zariadeniami %.*s"
   )
MSG_HASH(
   MSG_AUDIO_VOLUME,
   "Hlasitosť zvuku"
   )
MSG_HASH(
   MSG_AUTOCONFIG_FILE_ERROR_SAVING,
   "Chyba pri ukladaní profilu herného ovládača."
   )
MSG_HASH(
   MSG_AUTOCONFIG_FILE_SAVED_SUCCESSFULLY,
   "Profil herného ovládača úspešne uložený."
   )
MSG_HASH(
   MSG_ERROR,
   "Chyba"
   )
MSG_HASH(
   MSG_TO,
   "do"
   )
MSG_HASH(
   MSG_RESAMPLER_QUALITY_LOWER,
   "Nižšie"
   )
MSG_HASH(
   MSG_RESAMPLER_QUALITY_NORMAL,
   "Normálne"
   )
MSG_HASH(
   MSG_CORE_INSTALLATION_FAILED,
   "Inštalácia jadra zlyhala: "
   )

/* Lakka */


/* Environment Specific Settings */

MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_META_RESTART_KEY,
   "Reštartovať RetroArch"
   )
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_INPUT_TOUCH_ENABLE,
   "Dotyk"
   )

#ifdef HAVE_LAKKA_SWITCH
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SWITCH_GPU_PROFILE,
   "GPU pretaktovanie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_SWITCH_GPU_PROFILE,
   "Pretaktovanie alebo podtaktovanie GPU Switcha."
   )
#endif
#if defined(HAVE_LAKKA_SWITCH) || defined(HAVE_LIBNX)
MSG_HASH(
   MENU_ENUM_LABEL_VALUE_SWITCH_CPU_PROFILE,
   "CPU pretaktovanie"
   )
MSG_HASH(
   MENU_ENUM_SUBLABEL_SWITCH_CPU_PROFILE,
   "Pretaktovanie CPU Switcha."
   )
#endif
#ifdef HAVE_LAKKA
#endif
#ifdef GEKKO
#endif
#ifdef HAVE_ODROIDGO2
#else
#endif
#if defined(_3DS)
#endif
#ifdef HAVE_QT
#endif
