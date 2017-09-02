<p align="center"><img width=15% src="https://pbs.twimg.com/profile_images/672198632733548544/FCDxjRu1.png"></p>
<p align="center"><img width=40% src="https://i.imgur.com/42q4drh.png"></p>

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;
[![State of Life](https://img.shields.io/badge/state-eap-beta-orange.svg)][0]
[![Contributors](https://img.shields.io/badge/collaborators-7-green.svg)][1]
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
* Improved Resolvers
* Circle Strafer
* Fakewalk
* ...and much more

## Installation

Download the required dependencies:
* **All distributions**: Move [Font 1][3] and [Font 2][4] to `~/.fonts`
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

Redirect to the [help file][5] if customers require help.

## Usage

Press <kbd>Insert</kbd> to open the UI. You will be prompted to enter
your unique ID that has been assigned by the person that gave you the
invite.

## Configs

All configuration files are stored in `~/.config/Project Spartan`.
The folders are named after your config file name and contain a `config.json`
file containing your settings. Share that file with others if you want.

Be careful when accepting configs from others and check their contents as they
may have unwanted features (like `Allow untrusted angles`) enabled.

## Contributing

The development on this repository is standardized. All people are commiting
their changes onto branches named `<username>/<reason>` and are opening Pull
Requests to merge these branches it into the `master` branch. This allows for easy
reverting of changes from the GitHub Web UI and a cleaner workflow.

Short tutorial how to push into a separate branch:

```bash
$ git checkout -b username/featurename
$ git add .
$ git commit -m "Insert your own commit message here"
$ git push origin username/featurename:username/featurename
```

## License

Project Spartan is closed source and is licensed under the
Project Spartan Private License. Visit the `LICENSE.txt` file for more
informations.

## Screenshots

![Screenshot 0][s1]
![Screenshot 1][s2]

[0]: https://github.com/anass98h/Project-Spartan
[1]: https://github.com/anass98h/Project-Spartan
[2]: https://discord.gg/exTBFWz
[3]: https://github.com/anass98h/Project-Spartan/blob/master/UserHalp/Dyuthi.ttf
[4]: https://github.com/anass98h/Project-Spartan/blob/master/UserHalp/Sarai_07.ttf
[5]: https://github.com/anass98h/Project-Spartan/blob/master/UserHalp/HELLOOOOHELPME.md

[s1]: https://i.imgur.com/XqnBnv4.png
[s2]: https://i.imgur.com/4vkPfrC.jpg
