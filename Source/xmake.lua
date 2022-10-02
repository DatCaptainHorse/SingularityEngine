includes("Utilities", "System")

target("SingularityEngine")
	set_kind("static")
	add_deps("SE_System", "SE_Utilities")
	add_files("SE.mpp")
	set_policy("build.merge_archive", true)
