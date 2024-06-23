<p align="center">
  <img src="assets/logo.png" width="176"><br>
  <b>RetroArch ARM UWP release</b><br/>
  <a href="./cores">Cores</a> |
  <a href="https://github.com/libretro/RetroArch">Project</a> 
  <br/><br/>
  <img src="assets/screen.png"><br/><br/>
</p>


# About

After long time of work I present to you this UWP release of RetroArch for Windows (ARM) devices

With more than 70+ core


# Target

- It should work with Build: 15035+ 
- (14393 with few cores)

# Support

Please note that I'm not maintaining WOA (.exe) releases any more, only UWP
if you faced installation difficulties please checkout [this page](https://github.com/basharast/RetroArch-ARM/wiki/Installation)

## Surface (ARM64)
- Please note Qualcomm and Windows are no longer support ARM32 (which what I do)
- Keep check the official download page for any update from libretro side [Click here](https://retroarch.com/?page=platforms)
- My work is focus on legacy support for older devices and ARM devices that still support ARM32

# UWP improvements
This app supported by [UWP2Win32](https://github.com/basharast/UWP2Win32)

# Cores updater
The current cores downloader/updater linked with my repo at `docs/cores`

if you want to help to get/fix more cores for ARM please submit pull request with the target core or contact me


# Recently added
- EasyRPG (supported only with `1.15.5` or higher)


# Future

- Help me add Dynarec for Flycast: [GitHub Issue here](https://github.com/flyinghead/flycast/issues/545)

- ~~Help me to solve N64 issue: [GitHub Issue here](https://github.com/mupen64plus/mupen64plus-video-rice/issues/102)~~ [Solved]


# Credits

- <a href="https://www.retroarch.com/">RetroArch</a> Developed by RetroArch Team 


- Bashar Astifan (UWP Improvements and ARM support)

- [Gustave Monce](https://github.com/gus33000) (DLL dynamic loading solution)


# Support

RetroArch Team done great job to make this project if you would like to support them <a href="https://www.retroarch.com/index.php?page=donate">Click Here</a>

# Building

- You need SDKs: 18362 and 14393
- Use Visual Studio 2022
- Goto pkg\msvc-uwp
- Use only `RetroArch-msvc2019-UWP.sln` 
- You can build it as `ReleaseANGLE` or `ReleaseANGLE GL`
- `ReleaseANGLE GL` for devices with no DirectX 11.x support
- There is also Release 14393 (pointless, no much cores)
- To ensure cores loaded correctly, you can make them built-in
- Built-in cores must be at `pkg\msvc-uwp\RetroArch-msvc2019-UWP\cores`


## Important

This project maintained for legacy hardware, when something don't work well on modern hardware

please don't open issue for it, I have no interests to support other than ARM32


# Note

I'm not affiliate with RetroArch or Libretro
