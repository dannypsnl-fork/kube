cc_test(
    name = "kube-test",
    srcs = glob(["**/*.cpp"]),
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@googletest//:gtest_main",
        "//lib:kube",
    ],
)
