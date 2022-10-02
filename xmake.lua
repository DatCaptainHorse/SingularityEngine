add_rules("mode.debug", "mode.release")

set_rules("c++")
set_languages("c++20")
set_warnings("all", "error")

add_requires("doctest 2.4.*")

includes("Source", "Tests")
