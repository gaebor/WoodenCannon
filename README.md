# WoodenCannon
C++ serialization library (wannabe)

## Purpose
https://en.wikipedia.org/wiki/Marshalling_(computer_science)

## Naming
Tale about the wooden cannon of [Tihany](https://en.wikipedia.org/wiki/Tihany)
 * [Myth Busters video](https://www.youtube.com/watch?v=O6abBCyT69A)
 * [Hungarian poem](http://www.tihanyinfo.com/Tihany/Tihanyi_mondak_regek_versek/A_tihanyi_faagyu_igaz_tortenete.html)

## Compile
 * `make` with gnu make
 * `nmake /F Makefile.mak` with Visual C++ Command Prompt
 * There is a Visual Studio solution under `windows/windows.sln`

## Usage
 * include `inc/wc.h` (and other headers in `inc/`)
 * link against the static library `wc`
 * see the `test` executable for example usage

## Test
This application constructs and serializes various objects. Then reconstructs and compares whether the reconstruction was successful.
If the reconstruction was successful, the serialized objects are dumped into binary files.
If any of the objects fail to reconstruct, then the fail is reported.

If you run the `test` for the second time, it reads the serialized dump files and does the reconstruction from them.
Again, if any of the objects fail to reconstruct, then the fail is reported.

### `make all`
* clears the dump files
* clears all the binary code
* compiles
* runs the `test` twice
