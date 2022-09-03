# xbrz-cli

A repackaged version of [T1T4N/xBRZ-cli](https://github.com/T1T4N/xBRZ-cli) with minor differences:

- Adds `cimg_use_jpeg` flag for jpeg support (and links `libjpeg`)
- Adds `-quality 0-100` flag (only for JPEG output)
- Builds with [CMake](./CMakeLists.txt) instead of XCode.

### Running
```
$ xbrz [input] [scale] [output] [-quality 0-100]
```

The argument parsing isn't fancy – so make sure to use that format exactly.

### Building from Source

#### Linux
```sh
$ sudo apt-get install libpng-dev libjpeg-dev
$ cmake CMakeLists.txt
$ cmake --build . --config Release
```

#### OSX
```sh
$ brew install jpeg libpng
$ cmake CMakeLists.txt
$ cmake --build . --config Release
```

If you get libjpeg linking errors on OSX, try running `xcode-select --install` and `brew unlink jpeg && brew link jpeg` first.
