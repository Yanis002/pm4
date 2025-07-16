# Contribution guide
- [Project structure](#project-structure)
- [Decompiling](#decompiling)
- [Code style](#code-style)
- [Creating new `.c`/`.cpp` files](#creating-new-ccpp-files)

## Project structure
- `build/`: Build output
    - `eur|usa/`: Target version
        - `build/`: Linked ROM objects
        - `delinks/`: Objects delinked from the base ROM
        - `libs|src/`: Built C/C++ code
        - `arm9.o`: Linked ELF object
        - `arm9.o.xMAP`: Map file listing memory addresses for all symbols
- `config/`: [`dsd`](https://github.com/AetiasHax/ds-decomp) configuration files
- `docs/`: Documentation about the game
- `extract/`: Game assets, extracted from your own supplied ROM
    - `eur|usa/`: [`ds-rom`](https://github.com/AetiasHax/ds-rom) extract directories
- `include/`: Include files
- `libs/`: Source C/C++ files for libraries used by the game
- `src/`: Source C/C++ files for the game
- `tools/`: Tools for this project
    - `mwccarm/`: Compiler toolchain
    - `configure.py`: Generates `build.ninja`
    - `mangle.py`: Shows mangled symbol names in a given C/C++ file
    - `requirements.txt`: Python libraries
- `*.sha1`: SHA-1 digests of different versions of the game

## Decompiling
See [/docs/decompiling.md](/docs/decompiling.md).

## Code style
This project has a `.clang-format` file and all C/C++ files in this project should follow it. We recommend using an editor
compatible with `clang-format` to format the code as you save.
