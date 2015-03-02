# ICG15

## Contributors
* Christopher Benz, administrator
* Joachim Huet, administrator
* Vincent Loup, administrator

## Building
Just do `mkdir build && cd build/` and just do `cmake -DCMAKE_BUILD_TYPE=Debug ..` for building.

Make sure if there is troubles to `make clean` before complaining about some weird effects.

## Debugging
* Either use `qtcreator` with their interface.
* You can use `cgdb` if you like more the command line.
* If you have the longest beard, feel free to use `gdb`.
* I've also heard about a tool named `ddd`, I haven't tested it though.

## Submitting
* File should be named `Hw#-LastName1-LastName2-LastName3.zip`. You can use `zip -r file.zip folder/`
* Do not submit `build` folder. (what about the `.git` folder or other any useless other folders?)
* Executable must run on lab's machine (INF3). They have Radeon GPU, Ubuntu LTS 14.04, Haswell CPU.
* A readme.txt needs to be included for any explanations.
