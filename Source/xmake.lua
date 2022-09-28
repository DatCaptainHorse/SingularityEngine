includes("Utilities")

target("SingularityEngine")
	set_kind("static")
	add_deps("SE_Utilities")
	add_files("SE.mpp")
