
## Installation

Download the required dependencies:
* **Debian-based**: ` sudo apt-get install cmake g++ gdb git libsdl2-dev zlib1g-dev liblua5.3 libxdo-dev patchelf`
* **Arch**: `sudo pacman -S base-devel cmake gdb git sdl2 lua xdotool patchelf`
* **Fedora**: `sudo dnf install cmake gcc-c++ gdb git libstdc++-static mesa-libGL-devel SDL2-devel zlib-devel lua-devel libX11-devel libxdo-devel patchelf`
* **Gentoo**: `sudo emerge cmake dev-vcs/git gdb libsdl2 mesa lua xdotool patchelf`

Then install Curl++ (using the provided script) and the included font files.

## Injecting

If you want to use GDB, **open** CS:GO and type `./load.sh`.
If you want to use Binkload, **close CS:GO**, type `./binkload.sh` and **open CS:GO**.

## Usage

Press <kbd>Insert</kbd> to open the UI. You will be prompted to enter
your unique ID that has been assigned by the person that gave you the
invite.

## Configs

All configuration files are stored in `~/.config/Project Spartan`.
The folders are named after your config file name and contain a `config.json`
file containing your settings. Share that file with others if you want.

Usual Marc mocking down:

```cpp
int MarcIsaWeeb()
{
   C_BasePlayer* marc =  (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());

    return marc->gotFood() ? -1 : 1;
}
```
