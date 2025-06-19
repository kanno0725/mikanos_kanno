# mikanos
MikanOS: An Educational Operating System

# Files

- MikanLoaderPkg
  - The MikanOS loader as a UEFI Application

# dev env
https://github.com/uchan-nos/mikanos-build

# boot loader 
```sh
# setup
cd $HOME/edk2
ln -s $HOME/workspace/mikanos_kanno/MikanLoderPkg ./
source edksetup.sh

# build
build

# qemu
$HOME/osbook/devenv/run_qemu.sh Build/MikanLoaderX64/DEBUG_CLANG38/X64/Loader.efi $HOME/workspace/mikanos_kanno/kernel/kernel.elf
```

# kernel
```sh
# setup
source $HOME/osbook/devenv/buildenv.sh

# build
cd $HOME/workspace/mikanos_kanno/kernel
make

# build day18b onwards
./build.sh run
```
