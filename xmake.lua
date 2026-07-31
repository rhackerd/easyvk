add_rules("mode.debug", "mode.release")

add_requires("vulkan-headers", "vulkan-loader")
add_requires("vulkan-memory-allocator")
add_requires("vk-bootstrap")
add_requires("eigen")
add_requires("sdl3")

target("easyvk")
    set_kind("static")
    set_languages("c++23")

    add_headerfiles("easyvk/inc/easyvk.hpp")
    add_includedirs("easyvk/inc/")

    add_files("easyvk/src/*.cpp")

    add_packages("vulkan-headers", "vulkan-loader", "vulkan-memory-allocator", "vk-bootstrap", "eigen", "sdl3")

target("bin")
    set_kind("binary")
    add_files("src/*.cpp")
    set_languages("c++23")

    add_deps("easyvk")