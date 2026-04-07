workspace("BlipBlop")
	configurations({"Debug", "Release"})
	platforms({"x64"})
	location("..")
	
project("BlipBlop")
	kind("StaticLib")
	language("C++")
	location("../BlipBlop")
	files("../BlipBlop/*.h")
	files("../BlipBlop/*.cpp")
	pchheader("stdafx.h")
	pchsource("../BlipBlop/stdafx.cpp")
	objdir("../Intermediate")
	
	filter({"configurations:Debug"})
		targetname("BlipBlop_Debug")
		targetdir("../Output")
	
	filter({"configurations:Release"})
		targetname("BlipBlop_Release")
		targetdir("../Output")
		
	filter({"configurations:*"})
		
	warnings("Extra")
	fatalwarnings({"All"})
	
	defines { "_CRT_SECURE_NO_WARNINGS" }