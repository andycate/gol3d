load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = ["main.cpp", "LoadShader.hpp", "gol.hpp", "gol.cpp", "MollerTrumbore.hpp", "MollerTrumbore.cpp", "cube.hpp", "cube.cpp"],
    copts=["-Wno-deprecated"],
    linkopts = ["-lglfw", "-framework CoreVideo", "-framework OpenGL", "-framework IOKit", "-framework Cocoa", "-framework Carbon"],
)
