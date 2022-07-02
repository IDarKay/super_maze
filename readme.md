<div aligne="center">
<h1>
Super Maze
</h1>
<h5>
by IDarKay
</h5>
</div>

___

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Release](https://img.shields.io/badge/version-1.0.0-blue)]()
[![Language](https://img.shields.io/badge/Language-C-red)]()

## 📝 Table of Contents
- [About](#about)
- [Control](#Control)
- [Build](#build)
- [Dependence](#dependence)


## 🧐 About <a name = "about"></a>

Super Maze is à small C game run with SDL<br>
Made for school project by Alice.B

#### Functionalities

+ Random Maze generation
+ Create custome maze
+ Try solve maz
+ 2 player maze
+ Maze generator and solver visual

## 🎮 How play <a name = "Control"></a>

|    Mode    | Key              | Description         |
|:----------:|:-----------------|:--------------------|
|    All     | ESCAPE           | go main menu        |
| Main menu  | arrow UP/DOWN    | change select menu  |
| Main menu  | arrow LEFT/RIGHT | change option value |
| One player | arrow direction  | Move player         |
| Two player | arrow direction  | Move player 1       |
| Two player | W-A-S-D          | Move player 2       |
|    Edit    | W-A-S-D          | Move player         |
|    Edit    | arrow direction  | Break wall          |
|    Edit    | F8               | Save maze           |
| Automatic  | nothing          | enjoy it            |

## 🛠️ Build <a name = "build"></a>

### Linux
run <code>build.sh</code> script or <code>gcc -o prog src/*.c src/*/*.c -lSDL2 -lSDL2_ttf</code><br>
then <code>./prog</code> for start the game

### Other

good luck I don't know


## 🔒 Soft Dependance <a name = "dependance"></a>

+ LibSDL2: https://packages.debian.org/fr/sid/libsdl2-dev
+ LibSDL-ttf2: https://packages.debian.org/stretch/libsdl-ttf2.0-0