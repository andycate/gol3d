load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = ["main.cpp", "loadShader.hpp", "gol.hpp", "gol.cpp"],
    copts=["-Wno-deprecated"],
    linkopts = ["-lglfw", "-framework CoreVideo", "-framework OpenGL", "-framework IOKit", "-framework Cocoa", "-framework Carbon"],
)
