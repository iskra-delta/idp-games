# IDP Games

This repository contains a collection of games for the Iskra Delta Partner:

- Adventure
- Frogger
- Invaders
- Tetris
- Tetris G

## Compiling

The build downloads the latest model-M XCC release and uses the Partner SDK
and `cpmdisk` from the
[`wischner/xcc-z80-idp`](https://github.com/wischner/docker-toolchains)
image. Model M keeps `float` and `long`, while omitting `double`, `long long`,
and stdio floating-point conversions. Run:

```sh
make
```

The host Makefile downloads `x-m-linux.zip` from the latest XCC GitHub release.
It runs that compiler, assembler, linker, and its model-M target libraries in
the container alongside the Partner SDK. The image also supplies `cpmdisk`.
No local toolchain checkout, SDK submodules, host linker, CP/M disk definitions,
or additional libraries are required.

Build outputs are written to `bin/`:

- `advent.com`, `frogger.com`, `invaders.com`, `tetris.com`, and `tetrisg.com`
- the games' runtime `.bin` data files
- `idp-games.img`, containing all programs and data files

Use `make com` to build the programs and data files without creating the disk
image, or `make clean` to remove all generated output.
