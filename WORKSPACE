workspace(name = "macro_key")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

## rules_cc
http_archive(
    name = "rules_cc",
    sha256 = "1de5b47721fce0af0dd453b3071228fdfc44bd18199826b3f0b03b423aae9f65",
    strip_prefix = "rules_cc-0.2.18",
    url = "https://github.com/bazelbuild/rules_cc/releases/download/0.2.18/rules_cc-0.2.18.tar.gz",
)

load("@rules_cc//cc:extensions.bzl", "compatibility_proxy_repo")
compatibility_proxy_repo()

## bazel_skylib
http_archive(
    name = "bazel_skylib",
    sha256 = "fa01292859726603e3cd3a0f3f29625e68f4d2b165647c72908045027473e933",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.8.0/bazel-skylib-1.8.0.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.8.0/bazel-skylib-1.8.0.tar.gz",
    ],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")
bazel_skylib_workspace()

## platforms
http_archive(
    name = "platforms",
    sha256 = "218efe8ee736d26a3572663b374a253c012b716d8af0c07e842e82f238a0a7ee",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/platforms/releases/download/0.0.10/platforms-0.0.10.tar.gz",
        "https://github.com/bazelbuild/platforms/releases/download/0.0.10/platforms-0.0.10.tar.gz",
    ],
)

## pico-sdk
http_archive(
	name = "pico-sdk",
	sha256 = "2678fe2b176cf64a7f71cd91749fdf9134c8cf7ff84b7199dfe5ea0d6dba6fa4",
	urls = [
		"https://github.com/raspberrypi/pico-sdk/releases/download/2.2.0/pico-sdk-2.2.0.tar.gz",
	],
)

