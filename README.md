<p align="center"><img width=15% src="https://pbs.twimg.com/profile_images/672198632733548544/FCDxjRu1.png"></p>
<p align="center"><img width=40% src="https://i.imgur.com/42q4drh.png"></p>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;
[![State of Life](https://img.shields.io/badge/state-beta-orange.svg)][0]
[![Circle CI](https://img.shields.io/circleci/token/d414e4537f4abe5f1f6a9e269894e04662e1c160/project/github/anass98h/Project-Spartan/master.svg)][4]
[![Contributors](https://img.shields.io/badge/collaborators-7-green.svg)][1]
[![Join the Discord](https://img.shields.io/discord/336453128033533963.svg?label=discord)][2]
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

[Project Spartan](http://spartan.whatkills.us/ "Website") `/ˈspɑrtən/` (or just **Spartan**) is an advanced Counter-Strike:
Global Offensive internal multihack. It is based around a invite-only principle
which requires you to receive a invite to receive access to Spartan.
Spartan is a HvH orientated cheat, mainly focused on Spread.

## Features

* Base features of Hitlerhook, a fork of Fuzion - which is a fork of AimTux
* LBY-Breaker
* Improved Anti-Aims
* Better Resolver
* Circle Strafer
* Fakewalk
* Backtracking
* ...and much more

## Installation

Download the required dependencies:
* **Debian-based**: `cmake g++ clang-5.0 gdb git libsdl2-dev zlib1g-dev liblua5.3 libxdo-dev patchelf`
* **Arch**: `base-devel cmake clang gdb git sdl2 lua xdotool patchelf sdl2_image`
* **Fedora**: `cmake gcc-c++ clang gdb git libstdc++-static mesa-libGL-devel SDL2-devel zlib-devel lua-devel libX11-devel libxdo-devel patchelf`
* **Gentoo**: `cmake dev-vcs/git sys-devel/clang gdb libsdl2 mesa lua xdotool patchelf sdl2-image`

Then execute the following:

```bash
$ git clone --recursive https://github.com/anass98h/Project-Spartan
$ ./build.sh
# Open CS:GO
$ ./load.sh
```

Prebuilt binaries are available on [psBuild][3] and [CircleCI][4].
Please ship prebuilt binaries only over this repository.

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

## License & Credits

Project Spartan is closed source and is licensed under the
Project Spartan Private License. Visit the `LICENSE.txt` file for more
informations.

[Spartan Dev Team](http://spartan.whatkills.us/ "Website")

**Big shoutout to [Realnigga.Club](http://realnigga.club/ "Website") and his [his Discord](http://discord.me/realnigga "Discord"). [AimTux](http://aimtux.net/ "Website"), [Nyctum](http://nyctum.cc/ "Website") and all Linux cheats**.
GIGA Shoutout to Kisak.

## Screenshots

![In-Game Menu](http://spartan.whatkills.us/i/spartanmenu.png)

[0]: https://github.com/anass98h/Project-Spartan
[1]: https://github.com/anass98h/Project-Spartan
[2]: https://discord.gg/exTBFWz
[3]: https://github.com/ikfe/psbuilds
[4]: https://circleci.com/gh/anass98h/Project-Spartan/
