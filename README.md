# :video_game::video_game::video_game: NADNES :video_game::video_game::video_game:

</br>

 <img src="https://github.com/Iandmee/NADNES/blob/mem_search/UI/logo.png" alt="alt text" title="image Title" width="450"/>


</br>

* [General](#General)
    * [:space_invader: NES](#space_invader-NES)
    * [:floppy_disk: Saves](#floppy_disk-Saves)
    * [:mag_right: RAM/ROM search](#mag_right-RAM/ROM-search)
    * [:satellite: Network](#satellite-Network)
* [Settings](#Settings)
    * [:computer: Keymap binding](#computer-Keymap)
    * [:wrench: Configuration file](#wrench-Configuration-file)
* [Installation](#Installation)
    * [:rainbow: Windows](#windows)
    * [:apple: MacOS](#mac)
    * [:penguin: Linux](#linux)


</br>

## General

### :space_invader: NES


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*The Nintendo Entertainment System (NES)* is an 8-bit third-generation home video game console produced by *Nintendo*. It was first released in Japan in 1983 (Famicon) and in North America + Europe in 1985 (NES). It had several clones (Soviet Dendy, 1992).
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The *NES* featured a number of groundbreaking games, such as the platform game *Super Mario Bros*. and the action-adventure games *The Legend of Zelda* and *Metroid*, which became long-running franchises. It was succeeded in 1990 by the *Super Nintendo Entertainment System*.

</br>

### :floppy_disk: Saves

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*NADNES* provides possibility to save the game state to the file and loading from it. You can use quick save <kbd>Ctrl</kbd> + <kbd>S</kbd> to save current process to the last chosen file. If you want to specify destination you can use <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>S</kbd>.

</br>

### :mag_right: RAM/ROM search

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;There is an ability to search for certain cells in memory (for example, cells that are responsible for health, time, game-level or weapon). Cells can be exported to *cheats* and further modified to simplify/complicate the game or to feel the game in minimum time.

</br>

### :satellite: Network

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Also *NADNES* allows to play through the network: application can be swicthed to the host or to the guest mode. The host will be listening for incoming connections on every ip listed in the box after running the server. Client can connect to server by entering correct destination port and ip.

Number of connected players is unlimited, but no more than 2 **(!!!)** of them can play at the same time (host can assign roles). Others will be able to watch the game.

<!-- Server doesn't  limitations on incoming connections, but there are no more than 2 **(!!!)** clients whose can interact with the game at the same time (server has possibillity to choose current players), others can watch the server screen only. -->
</br>

## Settings


### :keyboard: Keymap binding

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Sometimes user wants to use different keys on his keyboard for specific games or when playing with a friend at the same computer. That's when keymap binding comes in handy.
You can chose one of 3 keymap profiles:
* *single player 1* (<kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>1</kbd>),
* *single player 2* (<kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>2</kbd>),
*  *cooperative* (<kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>C</kbd>)

Bindings for each of 2 gamepads can be changed with keymap editors (<kbd>Ctrl</kbd> + <kbd>1</kbd> and <kbd>Ctrl</kbd> + <kbd>2</kbd>)

</br>

### :wrench: Configuration file

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;If you want to change some special *NADNES* params you should move `nadnes.conf.sample` to the `nadnes.conf`, place it **one directory up** from executable and set desired params there.

</br>

## Installation


### Building from source (all platforms)

On Windows you either download a packed version or google how to install `qt6`, `boost`, `git`, `cmake`, `make` yourself (also don't forget to add everything to your `PATH`) :blush:.

On unix-like systems open the terminal. If you are on MacOs and don't have brew install it via
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

On Linux use your defualt package manager (`apt`, `pamac`, `pacman`, `deb`, `snap`, `flatpack`)

Then install `qt6`, `boost`: 

on `macOS`
```bash
brew install qt6 boost
```
on `Ubuntu`

```bash
sudo apt update && sudo apt upgrade
sudo apt install qt6 boost
```

on `Manjaro`


```bash
pamac update
pamac install qt6 boost
```

Locate to the directory where you want to install our project and write:

```bash
cd 'path/to/desired/project/location'
git clone 'https://github.com/Iandmee/NADNES'
```

Build the project

```bash
mkdir cmake-build-release
cd cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

Congratulations, you can run the project by writing

```bash
./NADNES
```

Or on *Windows*

```powershell
.\NADNES.exe
```
