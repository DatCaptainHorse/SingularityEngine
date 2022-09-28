add_rules("mode.debug", "mode.release")

set_rules("c++")
set_languages("c++20")
set_warnings("all", "error")

add_requires("doctest 2.4.*")

target("SE-Utilities")
	set_kind("static")
	add_files("Source/Utilities/*.mpp")

target("SingularityEngine")
	set_kind("static")
	add_deps("SE-Utilities")
	add_files("Source/SE.mpp")

target("Tests")
	set_kind("binary")
	add_deps("SingularityEngine")
	add_files("Tests/test.cpp")
	add_packages("doctest")
