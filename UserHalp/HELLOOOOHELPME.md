
## Installation

Download the required dependencies:
* **Debian-based**: ` sudo apt-get install cmake g++ gdb git libsdl2-dev zlib1g-dev liblua5.3 libxdo-dev patchelf`
* **Arch**: `sudo pacman -S base-devel cmake gdb git sdl2 lua xdotool patchelf`
* **Fedora**: `sudo dnf install cmake gcc-c++ gdb git libstdc++-static mesa-libGL-devel SDL2-devel zlib-devel lua-devel libX11-devel libxdo-devel patchelf`
* **Gentoo**: `sudo emerge cmake dev-vcs/git gdb libsdl2 mesa lua xdotool patchelf`

Then Install the included font files.


To Load the file:
# Open CS:GO
and execute the load script

## Usage

Press <kbd>Insert</kbd> to open the UI. You will be prompted to enter
your unique ID that has been assigned by the person that gave you the
invite.

## Configs

All configuration files are stored in `~/.config/Project Spartan`.
The folders are named after your config file name and contain a `config.json`
file containing your settings. Share that file with others if you want.



int MarcIsaWeeb(){
   C_BasePlayer* Marc =  (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
    if(Marc->gotFood()){return -1;}
    else {return 1;}

}
