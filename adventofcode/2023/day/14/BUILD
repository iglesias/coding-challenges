cc_binary(
    name = "parabolic_reflector_dish",
    copts = [
        "-std=c++2a",
        "-Iexternal/google-benchmark/include",
        "-Ofast",
    ],
    srcs = ["parabolic_reflector_dish.cpp", "splay_tree_map.hpp"],
    deps = ["@google-benchmark//:benchmark"],
    data = ["@parabolic_reflector_dish_input//file"],
)
