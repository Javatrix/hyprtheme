# Hyprtheme
Extremely basic theme swapping utility for Hyprland.

# Usage
- `list` lists saved themes
- `get` gets the currently set theme
- `set` sets the theme
- `create <themeName>` save a new theme from the current configs

Note that when creating a theme with a name that is already used, hyprtheme will ask you if you would like to overwrite the existing theme.
However, doing so will NOT delete any files that your current config does not include. E.g. if you previous theme had a README.md file,
and you current configuration does not include this file, it will not be deleted. Only the files that you have stored in your config directory
and added to the .themefiles will get overwritten.

# Installation
### Arch Linux
```sh
git clone https://github.com/Javatrix/hyprtheme && cd hyprtheme && makepkg -si
```

## Other Distros
### Dependencies
Before installing, make sure you have gcc installed.
### Build process
Execute the following to automatically clone, compile and move the file to your local bin:
```
git clone https://github.com/Javatrix/hyprtheme && cd hyprtheme && g++ -o hyprtheme src/* && mv hyprtheme /usr/bin
```

# Configuring
List the files you want to save in `~/.config/hypr/themes/.themefiles` (the paths are relative to ~/.config/
Example:
```
hypr/hyprland.conf
hypr/windowrules.conf
waybar/
wofi/
gtk-2.0/
gtk-3.0/
gtk-4.0/
```
