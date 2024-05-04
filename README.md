``Reupload project from 2017 : This project was recovered so don't take commits into accounts``

<h1 align="center"> DoodleRetro :video_game: :space_invader: <h1>

<img src="https://img.shields.io/badge/platform-gameboy-yellowgreen" />
<img src="https://img.shields.io/badge/language-c-lightgrey" />


## Preview
<p float="left" align="center">
  <img src="https://img.itch.zone/aW1hZ2UvODMwOTc4LzQ2NTY2NDgucG5n/original/kt3AjI.png" width="300" />
  <img src="https://img.itch.zone/aW1hZ2UvODMwOTc4LzQ2NTY2NTAucG5n/original/uFRQym.png" width="300" /> 
</p>

## Overview
This project is a tribute of the mobile Doodle Jump game, like the mobile game you climb a place by jumping without any rest on platforms, if you fall you loose !


The game was made in C using the [GBDK devkit](https://github.com/gbdk-2020/gbdk-2020) for Nintendo Gameboy and more


## Run it
The github file already provie the ``.gb``, so just drag n drop it into a Gameboy Emulator [Bgb for windows user](http://bgb.bircd.org/) or [mGBA for linux and mac user](https://mgba.io/)


## Controls
Beware to set your emulator control befeore !

A -> Skip Score Screen

Start -> Start the game

Left/Right -> Moving the player


## Build the project

If you want to build the project you can follow the step below :
- Ensure you have GCC installed
- Download the [GBDK devkit](https://github.com/gbdk-2020/gbdk-2020)
- Put the GBDK folder where you want but copy the path to it
- Then open the Makefile and set the ``GBDK_FOLDER`` var at the path you got (keep the / at the end of the GBDK path)
- Finally just run ``make`` command this will replace the .gb by a freshly builded one
- Then you can just drag n drop it into a GB emulator
- (Optionnal) if you got lot of compilation file you can run ``make clean``
