load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = ["main.cc", "loadShader.hpp"],
    linkopts = ["-lglfw", "-framework CoreVideo", "-framework OpenGL", "-framework IOKit", "-framework Cocoa", "-framework Carbon"],
)
