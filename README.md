# stm8ef-modular-build

[![Travis-CI](https://travis-ci.org/TG9541/stm8ef-modular-build.svg)](https://travis-ci.org/TG9541/stm8ef-modular-build)

This repository is at the same time a demonstrator and a template for the [STM8 eForth](https://github.com/TG9541/stm8ef) Modular Build feature. implements board support for a [simple demo device](https://github.com/TG9541/stm8ef-modular-build/tree/master/DEMO): board specific setting are entirely in the board support folder (`DEMO` in the example) and STM8 eForth dependencies are taken care of by the `Makefile`.

Building the code can be done manually (on Linux make sure that Python and SDCC are installed) or automatically wuth Travis-CI.

## How to use the code manually

Make a copy or clone of the code in this repository, connect your board to the serial interface and run `make` on a Linux system with STM8FLASH, SDCC and Python 2.7 (with some chages Python3 can be used).

You can also run `make flash` to only build the binary and flash it to you board and `make term` to start an installed e4thcom.

## How to enable your fork in Travis-CI

If you want build automation, first create a Travis-CI account, link it to your GitHub account and enable your fork of this repository:

![image](https://user-images.githubusercontent.com/5466977/79549569-7041d100-8097-11ea-86a2-8a544cdea3b5.png)

Install the Travis-CI CLI tool and install credentials:

```
(base) thomas@w500:~/source/stm8s/stm8ef-modular-build$ travis setup releases --force
Username: tg9541
Password for tg9541: *******************
File to Upload: test
Deploy only from TG9541/stm8ef-modular-build? |yes| 
Encrypt API key? |yes| 
```

Change (back) `FILE to Upload` from `file: test` to the following:

```
  file:
    - "out/stm8ef-bin.zip"
    - "out/stm8ef-bin.tgz"
  skip_cleanup: true
```

Create and configure a Travis-CI webhook in your repository (my settings are [here](https://github.com/TG9541/stm8ef-modular-build/issues/1#issuecomment-615125384)).

Make the Travis-CI badge point to your accounts:
```
[![Travis-CI](https://travis-ci.org/TG9541/stm8ef-modular-build.svg)](https://travis-ci.org/TG9541/stm8ef-modular-build)
```
