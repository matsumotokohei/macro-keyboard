CURDIR = $(shell pwd)

# ENVIRONMENT VARIABLES
export USE_BAZEL_VERSION=8.0.0
export PICO_SDK_PATH=$(CURDIR)/pico-sdk

# BAZEL options
BAZEL = bazel
#BAZEL_OPTS = --enable_workspace --enable_bzlmod --verbose_failures
BAZEL_OPTS = --enable_bzlmod --verbose_failures
BAZEL_TARGET = //:macro-keyboard

# Build options for the Pico SDK
PICO_OPT = --platforms=@pico-sdk//bazel/platform:rp2040
PICO_TINY_USB_CONFIG = --@pico-sdk//bazel/config:PICO_TINYUSB_CONFIG=//:my_tusb_config
PICO_UF2 = --aspects=@pico-sdk//tools:uf2_aspect.bzl%pico_uf2_aspect --output_groups=+pico_uf2_files
PICO_UF2_FILE = macro-keyboard.uf2

# Astyle
ASTYLE = astyle
ASTYLE_OPTS = --style=google -n -t

all:
	$(BAZEL) build $(BAZEL_OPTS) $(PICO_OPT) $(PICO_TINY_USB_CONFIG) $(BAZEL_TARGET) $(PICO_UF2)
	rm -rf $(CURDIR)/$(PICO_UF2_FILE)
	cp bazel-bin/$(PICO_UF2_FILE) $(CURDIR)/$(PICO_UF2_FILE)

get_pico_sdk:
	git clone https://github.com/raspberrypi/pico-sdk.git
	cd pico-sdk
	git submodule update --init --recursive

clean:
	$(BAZEL) clean
	rm -rf $(CURDIR)/$(PICO_UF2_FILE)

distclean:
	$(BAZEL) clean --expunge
	rm -rf $(CURDIR)/$(PICO_UF2_FILE)

style: src/*.c src/*.h
	$(ASTYLE) $(ASTYLE_OPTS) $^
