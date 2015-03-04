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

## FAQ
### Hey, I get that libglfw.so is not found on the provided binaries?
This library is linked by cmake and it is dependent of the environment where you compiled it though (username, path).
You can check this by running `ldd` on the compiled library and see which one(s) are in your `/home/username/` folder.
There aren't any solutions, just compile from nothing...

### Hey, my code works on Windows but not on Linux. WTF!
Linus has a case sensitive file system, make sure the uppercase/lowercase files are all correct.

### What? You added binaries into the git? Are you drunk?
Yeah I made it, yeah it's dumb, yeah I agree with you.
The rules are the rules.
If they want a broken binary, they'll have broken library.
