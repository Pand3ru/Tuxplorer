# Tuxplorer

Tuxplorer is a terminal-based file explorer, aimed at bringing the power and simplicity of file navigation and management to the terminal interface. Designed as my first real C project, Tuxplorer is currently in its early stages, featuring basic functionality.

![Tuxplorer Screenshot](screenshots/tuxplorer_screenshotV0.1.png)

## Current Features

- **Basic Navigation**: Seamlessly move through your filesystem within the terminal.
- **File Viewing**: List files and directories in the current directory.
- **Text Editor Integration**: Open and edit text files directly with your favorite text editor (default is vim) by pressing enter on a selected file.
- **File Operations**: Perform common file operations such as rename, delete, and create directly within Tuxplorer.
- **Shortcuts**: Quick access shortcuts to frequently used directories such as Desktop, Documents, etc.
- **Pin Folders**: Ability to pin favorite folders for easy access.
- **Config Files**: Customize Tuxplorer's behavior through user-defined configuration files.
- **Image Preview**: Preview images. (Note: currently only works on kitty terminals without tmux!)

## Getting Started

To try out Tuxplorer, you'll need to have `gcc`, `ncurses`, `libsixel` and `libconfig` installed on your system.
It is also using [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) so the feature may not work without it.
I am currently working on a package build but for now this'll have to do.

### Installation

1. Clone the repository:
`git clone https://github.com/Pand3ru/Tuxplorer`
2. Change to the Tuxplorer directory:
`cd Tuxplorer`
3. Compile the project:
`make`
4. Install:
`sudo make install`

## Contributing

Contributions are welcome :)

## License

Distributed under the MIT License. See `LICENSE` for more information.





