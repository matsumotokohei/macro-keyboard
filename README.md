# macro-keyboard

DIY Macro Keyboard with Raspberry Pi Pico

## PC environment for development

Ubuntu 24.04 LTS on WSL2

```bash
uname -a
Linux 6.6.87.2-microsoft-standard-WSL2 #1 SMP PREEMPT_DYNAMIC Thu Jun  5 18:30:46 UTC 2025 x86_64 x86_64 x86_64 GNU/Linux
```

```bash
lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 24.04.4 LTS
Release:        24.04
Codename:       noble
```

## Target board

Raspberry pi pico (RP2040)

## Install toolchain with Bazelisk

```bash
sudo apt update
sudo apt install nodejs
sudo apt install npm
sudo npm install -g @bazel/bazelisk
```

## Install toolchain with build-tool

```bash
sudo apt install cmake python3 build-essential gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
```

## Download pico-sdk from github.com

```bash
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init --recursive
```

## Build firmware

```bash
export USE_BAZEL_VERSION=8.0.0
export PICO_SDK_PATH=$(pwd)/pico-sdk

bazel build \
  --platforms=@pico-sdk//bazel/platform:rp2040 \
  --@pico-sdk//bazel/config:PICO_TINYUSB_CONFIG=//:my_tusb_config \
  --aspects=@pico-sdk//tools:uf2_aspect.bzl%pico_uf2_aspect \
  --output_groups=+pico_uf2_files \
  //:macro-keyboard
```

## Get firmware

Bazel is created macro-keyboard.uf2 in **bazel-bin** directory.

```bash
cp bazel-bin/macro-keyboard.uf2 ./
```

## Write firmware

*   Press and hold the **BOOTSEL** button while resetting the board (by pulling the **RUN** pin / Pin 30 Low).
  * Refer to https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html#layouts
*   Drag and drop the `.uf2` file into the **RPI-RP2** drive.
  * Refer to https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html#your-first-binaries

## Note

* pico-sdk v2.2.0 does not support Bazel 9.0.
  * Workaround: Forced the version to 8.0.0 using export USE_BAZEL_VERSION=8.0.0.
* tinyusb does not support Bazel
  * Workaround: Use pico-sdk/src/rp2_common/tinyusb/tinyusb.BUILD
