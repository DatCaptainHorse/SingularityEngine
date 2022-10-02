add_rules("mode.releasedbg", "mode.debug")

if is_mode("releasedbg") or is_mode("release") then
    set_optimize("faster")
end

set_rules("c++")
set_languages("c++20")
set_warnings("all", "error")

add_requires("doctest 2.4.*")

includes("Source", "Tests")
