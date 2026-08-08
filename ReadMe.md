# IDP Games

This repository contains a collection of games for the Iskra Delta Partner:

- Adventure
- Frogger
- Invaders
- Tetris
- Tetris G

## Compiling

The build uses the locally built XCC compiler together with the Partner SDK,
target libraries, and `cpmdisk` from the
[`wischner/xcc-z80-idp`](https://github.com/wischner/docker-toolchains)
image. By default, XCC is loaded from
`../../retro-vault/xyz/bin/x/bin/xcc`. Build that compiler first, then run:

```sh
make
```

The host Makefile mounts the local compiler over the image's XCC binary, then
performs the complete build with the Partner SDK and `cpmdisk` supplied by the
image. Override `LOCAL_XCC=/absolute/path/to/xcc` if the X tools checkout is
elsewhere. No SDK submodules, host linker, CP/M disk definitions, or additional
libraries are required.

Build outputs are written to `bin/`:

- `advent.com`, `frogger.com`, `invaders.com`, `tetris.com`, and `tetrisg.com`
- the games' runtime `.bin` data files
- `idp-games.img`, containing all programs and data files

Use `make com` to build the programs and data files without creating the disk
image, or `make clean` to remove all generated output.
