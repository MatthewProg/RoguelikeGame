<h1 align="center">Roguelike Game (C++)</h1>

<p align="center">
    <a href="/./LICENSE"><img src="https://img.shields.io/github/license/MatthewProg/RoguelikeGame"></a>
</p>

<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about">About</a></li>
    <li><a href="#set-up">Set up</a></li>
    <li><a href="#screenshots">Screenshots</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

## About

Roguelike Game (temporary name) is my side project to work on in free time.
The project is using SFML library to create and manage resources.
<br>
The goal of the project is to create a game that fully supports a community of modders.
All maps, graphics, weapons and enemies could be altered.
<br><br>
Graphics made by <a href="https://0x72.itch.io/">0x72</a>

## Set up

1. Download SFML from <a href="https://www.sfml-dev.org/download.php">link</a> (v2.5.1 is fully supported)
2. Clone repo using `git clone https://github.com/MatthewProg/RoguelikeGame.git`
3. Run `setup.ps1` in PowerShell and follow instructions
4. Open `RoguelikeGame.sln`

`NOTE: If SFML doesn't work, check out config tutorials on https://www.sfml-dev.org/tutorials/2.5/`<br>
`NOTE 2: Script create hooks which commit project files without direct SFML path, so please ignore *.vcxproj being modified after commit`

## Screenshots

<table>
    <tr>
        <th style="text-align:center"><a href="/./docs/img/menu.png"><img alt="menu" src="/./docs/img/menu.png"></a></th>
        <th style="text-align:center"><a href="/./docs/img/settings.png"><img alt="settings" src="/./docs/img/settings.png"></a></th>
        <th style="text-align:center"><a href="/./docs/img/game.png"><img alt="game" src="/./docs/img/game.png"></a></th>
        <th style="text-align:center"><a href="/./docs/img/debugging.png"><img alt="debugging" src="/./docs/img/debugging.png"></a></th>
    </tr>
    <tr>
        <td align="center">Main menu</td>
        <td align="center">Settings</td>
        <td align="center">Game</td>
        <td align="center">Debugging tools</td>
    </tr>
</table>

## TODO checklist

Main features
- [x] Loading maps from files
- [x] Enemies pathfinding AI
- [x] Logger class
- [ ] Loading mods
- [ ] Shaders
- [ ] Particles

Weapons
- [x] Hitbox (touch)
- [x] Melee
- [ ] Projectile
- [ ] Hitscan

UI
- [x] Container
- [x] Label
- [x] Button
- [x] Check box
- [x] Progess bar
- [x] Scroll bar
- [x] Scroll view
- [x] List selector
- [x] Animation box
- [x] Popup
- [ ] Edit box

Other
- [ ] Map editor
- [ ] Maps

## [License](/./LICENSE)

```
MIT License

Copyright (c) 2021 Mateusz Ciaglo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```