<p align="center"><img width=15% src="https://pbs.twimg.com/profile_images/672198632733548544/FCDxjRu1.png"></p>
<p align="center"><img width=40% src="https://i.imgur.com/42q4drh.png"></p>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;
[![State of Life](https://img.shields.io/badge/state-beta-orange.svg)][0]
[![Contributors](https://img.shields.io/badge/collaborators-3-green.svg)][1]
[![Join the Discord](https://img.shields.io/discord/336453128033533963.svg?label=discord)][2]
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

Project Spartan `/ˈspɑrtən/` (or just **Spartan**) is an advanced Counter-Strike:
Global Offensive internal multihack. It is based around a invite-only principle
which requires you to receive a invite and pay to receive access to Spartan.
Spartan is a HvH orientated cheat, mainly focused on Spread.

## Features

* Base features of Hitlerhook, a fork of Fuzion - which is a fork of AimTux
* LBY-Breaker
* Improved Anti-Aims
* Better Resolver
* Circle Strafer
* Fakewalk
* ...and much more

## Installation

Download the required dependencies:
* **All** http://smc.org.in/downloads/fonts/dyuthi/Dyuthi.ttf &&
  ftp://195.220.108.108/linux/fedora-secondary/releases/26/Everything/s390x/os/Packages/s/sarai-fonts-1.0-19.fc26.noarch.rpm
* **Debian-based**: `cmake g++ gdb git libsdl2-dev zlib1g-dev liblua5.3 libxdo-dev patchelf`
* **Arch**: `base-devel cmake gdb git sdl2 lua xdotool patchelf`
* **Fedora**: `cmake gcc-c++ gdb git libstdc++-static mesa-libGL-devel SDL2-devel zlib-devel lua-devel libX11-devel libxdo-devel patchelf`
* **Gentoo**: `cmake dev-vcs/git gdb libsdl2 mesa lua xdotool patchelf`

Then execute the following:

```bash
$ git clone https://github.com/anass98h/Project-Spartan
$ ./build.sh
# Open CS:GO
$ ./load.sh
```

## Usage

Press <kbd>Insert</kbd> to open the UI. You will be prompted to enter
your unique ID that has been assigned by the person that gave you the
invite.

## Configs

All configuration files are stored in `~/.config/Project Spartan`.
The folders are named after your config file name and contain a `config.json`
file containing your settings. Share that file with others if you want.

## Contributing

The development on this repository is not standardized. The most people are commiting
their changes onto a branch named `<username>/<reason>` and are opening a
Pull Request to merge it into the `master` branch. This allows for easy
reverting of changes from the GitHub Web UI.

Tutorial for @Apostropheqq and other:

```bash
$ git checkout -b username/reason
$ git add .
$ git commit -m "I made changes that I'll push into a seperate branch"
$ git push origin username/reason:username/reason
```

## License

Project Spartan is closed source and is licensed under the
Project Spartan Private License. Visit the `LICENSE.txt` file for more
informations.

## Screenshots

![Apostrophe zwingte mich, sorry Rasp](https://i.imgur.com/KgKGe6Mr.jpg)

[0]: https://github.com/anass98h/Project-Spartan
[1]: https://github.com/anass98h/Project-Spartan
[2]: https://discord.gg/exTBFWz
