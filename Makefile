BAZEL = bazel
BAZEL_VERSION = 8.0.0
BAZEL_ENV = --action_env=USE_BAZEL_VERSOIN=$(BAZEL_VERION) --action_env=PICO_SDK_PATH=./pico-sdk

PICO_OPT = --platforms=@pico-sdk//bazel/platform:rp2040 --aspects=@pico-sdk//tools:uf2_aspect.bzl%pico_uf2_aspect --output_groups=+pico_uf2_files
TARGET = //:main
UF2_FILE = main.uf2

all:
	$(BAZEL) build $(BAZEL_ENV) $(PICO_OPT) $(TARGET) 
	rm -rf ./$(UF2_FILE)
	cp bazel-bin/$(UF2_FILE) ./$(UF2_FILE)

clean:
	$(BAZEL) clean --expunge
	rm -rf ./$(UF2_FILE)

