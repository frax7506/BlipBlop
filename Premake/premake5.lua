workspace("BlipBlop")
	configurations({"Debug", "Release"})
	platforms({"x64"})
	location("..")
	
group("Utilities")
	project("HD_CommonUtilities")
	kind("StaticLib")
	language("C++")
	cppdialect("C++17")
	location("../HD_CommonUtilities")
	files("../HD_CommonUtilities/*.h")
	files("../HD_CommonUtilities/*.cpp")
	files("../HD_CommonUtilities/HD_CommonUtilities.natvis")
	pchheader("stdafx.h")
	pchsource("../HD_CommonUtilities/stdafx.cpp")
	objdir("../Intermediate/HD_CommonUtilities")
	
	filter({"configurations:Debug"})
		targetname("HD_CommonUtilities_Debug")
		targetdir("../Output/HD_CommonUtilities")
	
	filter({"configurations:Release"})
		targetname("HD_CommonUtilities_Release")
		targetdir("../Output/HD_CommonUtilities")
		
	filter({"configurations:*"})
		
	warnings("Extra")
	fatalwarnings({"All"})
	
	defines { "_CRT_SECURE_NO_WARNINGS" }
	
	vpaths
	{
		["Containers"] =
		{
			"../HD_CommonUtilities/HD_ArrayIterator.h",
			"../HD_CommonUtilities/HD_CircularArray.h",
			"../HD_CommonUtilities/HD_DataBuffer.h",
			"../HD_CommonUtilities/HD_GrowingArray.h",
			"../HD_CommonUtilities/HD_HashMap.h",
			"../HD_CommonUtilities/HD_Map.h",
			"../HD_CommonUtilities/HD_Pair.h",
			"../HD_CommonUtilities/HD_StaticArray.h",
			"../HD_CommonUtilities/HD_StaticStack.h",
			"../HD_CommonUtilities/HD_String.h",
			"../HD_CommonUtilities/HD_String.cpp"
		},
		["Math"] =
		{
			"../HD_CommonUtilities/HD_AABB_2D.h",
			"../HD_CommonUtilities/HD_Math.h",
			"../HD_CommonUtilities/HD_Vector2.h",
			"../HD_CommonUtilities/HD_Vector3.h"
		},
		["Misc"] =
		{
			"../HD_CommonUtilities/HD_ExeArgs.h",
			"../HD_CommonUtilities/HD_ExeArgs.cpp",
			"../HD_CommonUtilities/HD_Hash.h",
			"../HD_CommonUtilities/HD_IsFundamental.h",
			"../HD_CommonUtilities/HD_Logger.h",
			"../HD_CommonUtilities/HD_Logger.cpp",
			"../HD_CommonUtilities/HD_PreprocessorMacros.h",
			"../HD_CommonUtilities/HD_Random.h",
			"../HD_CommonUtilities/HD_Random.cpp",
			"../HD_CommonUtilities/HD_Singleton.h",
			"../HD_CommonUtilities/HD_Time.h",
			"../HD_CommonUtilities/HD_Time.cpp",
			"../HD_CommonUtilities/HD_Types.h",
			"../HD_CommonUtilities/HD_Utilities.h",
			"../HD_CommonUtilities/OptimizedWindowsInclude.h"
		},
		["Natvis"] =
		{
			"../HD_CommonUtilities/HD_CommonUtilities.natvis"
		},
		["Profiling"] =
		{
			"../HD_CommonUtilities/HD_ScopedTimer.h",
			"../HD_CommonUtilities/HD_ScopedTimer.cpp"
		}
	}
group("")
	
project("BlipBlop")
	dependson("HD_CommonUtilities")
	kind("StaticLib")
	language("C++")
	cppdialect("C++17")
	location("../Source/BlipBlop")
	files("../Source/BlipBlop/*.h")
	files("../Source/BlipBlop/*.cpp")
	libdirs("../Lib/HD_CommonUtilities")
	includedirs("../Source")
	pchheader("stdafx.h")
	pchsource("../Source/BlipBlop/stdafx.cpp")
	objdir("../Intermediate/BlipBlop")
	
	filter({"configurations:Debug"})
		links("HD_CommonUtilities_Debug")
		targetname("BlipBlop_Debug")
		targetdir("../Lib/BlipBlop")
	
	filter({"configurations:Release"})
		links("HD_CommonUtilities_Release")
		targetname("BlipBlop_Release")
		targetdir("../Lib/BlipBlop")
		
	filter({"configurations:*"})
		
	warnings("Extra")
	fatalwarnings({"All"})
	
	defines { "_CRT_SECURE_NO_WARNINGS" }
	
project("ModelViewer")
	dependson("HD_CommonUtilities", "BlipBlop")
	kind("ConsoleApp")
	language("C++")
	cppdialect("C++17")
	location("../Source/ModelViewer")
	files("../Source/ModelViewer/*.h")
	files("../Source/ModelViewer/*.cpp")
	libdirs("../Lib/HD_CommonUtilities", "../Lib/BlipBlop")
	includedirs("../Source")
	pchheader("stdafx.h")
	pchsource("../Source/ModelViewer/stdafx.cpp")
	objdir("../Intermediate/ModelViewer")
	
	filter({"configurations:Debug"})
		links("HD_CommonUtilities_Debug", "BlipBlop_Debug")
		targetname("ModelViewer_Debug")
		targetdir("../Output")
	
	filter({"configurations:Release"})
		links("HD_CommonUtilities_Release", "BlipBlop_Release")
		targetname("ModelViewer_Release")
		targetdir("../Output")
		
	filter({"configurations:*"})
		
	warnings("Extra")
	fatalwarnings({"All"})
	
	defines { "_CRT_SECURE_NO_WARNINGS" }