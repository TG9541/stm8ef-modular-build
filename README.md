# stm8ef-modular-build

[![Travis-CI](https://travis-ci.org/TG9541/stm8ef-modular-build.svg)](https://travis-ci.org/TG9541/stm8ef-modular-build)

This demonstrator for the [STM8 eForth](https://github.com/TG9541/stm8ef) Modular Build feature implements board support for a [simple demo device](https://github.com/TG9541/stm8ef-modular-build/tree/master/DEMO). Board specific setting are entirely in the folder `DEMO` and all dependencies are taken care of in the `Makefile`.

Building the code can be done manually (on Linux make sure that Python and SDCC are installed) or automatically wuth Travis-CI.

## How to enable your fork in Travis-CI

First create a Travis-CI account, connect it to your GitHub account and enable your fork of this repository:

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
