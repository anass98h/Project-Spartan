

## What is Spartan?

pHake



If you're having problems compiling make sure you've got the latest version of `g++`.

[How to update g++](https://github.com/AimTuxOfficial/AimTux/wiki/Updating-your-compiler)

==================

#### required depencies!

__Ubuntu-Based / Debian:__
```bash
sudo apt-get install cmake g++ gdb git libsdl2-dev zlib1g-dev liblua5.3 libxdo-dev patchelf
```
__Arch:__
```bash
sudo pacman -S base-devel cmake gdb git sdl2 lua xdotool patchelf
```
__Fedora:__
```bash
sudo dnf install cmake gcc-c++ gdb git libstdc++-static mesa-libGL-devel SDL2-devel zlib-devel lua-devel libX11-devel libxdo-devel patchelf
```

__Gentoo:__
```bash
sudo emerge cmake dev-vcs/git gdb libsdl2 mesa lua xdotool patchelf
```
===================

#### Download Spartan:

```bash
git clone --recursive https://github.com/anass98h/Project-Spartan
```

```bash
cd  Project\ Spartan/
```

===================


#### Compile with build script

You can build easily with the included build script.
```bash
./build
```
You can later update with 
```bash
./update
```

## Injecting

First of all, make sure CSGO is open, it doesn't matter whether you're in game or not. However, it is not recommended to inject while CSGO is loading into a map. 

Navigate to the directory where spartan was built if you haven't ready.

```bash
cd  Project\ Spartan/
```

Now, you can inject the hack with the `load` script

```bash
./load
```




## Using the hack

insert for menu :)

## Unloading the hack

If you wish to unload the hack from the game, you can do so by entering the command:

```bash
./uload
```

## Configs

Configs are stored in a hidden directory in your home folder. Specifically 

```
~/.config/project spartan/
```

Each `config.json` is stored in a seperately named folder (The name you see in-game, in the config window). 

To add a config, create a folder inside of the `~/.config/Hitlerhook` folder with a name of your choice, and paste the `config.json` inside of that folder.

To see hidden folders inside your home folder, press <kbd>CTRL</kbd>+<kbd>H</kbd> when using a file manager.

On your Command line, You can also add the -a flag on ls     `ex: ls -la ~/` 

## Grenade Configs

```

```
useless shit


## Screenshots


# Project-Spartan
# Project-Spartan
