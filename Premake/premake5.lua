workspace("BlipBlop")
	configurations({"Debug", "Release"})
	platforms({"x64"})
	location("..")
	
group("Utilities")
	project("HD_CommonUtilities")
		kind("StaticLib")
		language("C++")
		location("../Source/HD_CommonUtilities")
		files("../Source/HD_CommonUtilities/*.h")
		files("../Source/HD_CommonUtilities/*.cpp")
		pchheader("stdafx.h")
		pchsource("../Source/HD_CommonUtilities/stdafx.cpp")
		objdir("../Intermediate/HD_CommonUtilities")
	
		filter({"configurations:Debug"})
			targetname("HD_CommonUtilities_Debug")
			targetdir("../Lib/HD_CommonUtilities")
	
		filter({"configurations:Release"})
			targetname("HD_CommonUtilities_Release")
			targetdir("../Lib/HD_CommonUtilities")
		
		filter({"configurations:*"})
		
		warnings("Extra")
		fatalwarnings({"All"})
	
		defines { "_CRT_SECURE_NO_WARNINGS" }
		
		vpaths
	{
		["Containers"] =
		{
			"../Source/HD_CommonUtilities/HD_ArrayIterator.h",
			"../Source/HD_CommonUtilities/HD_CircularArray.h",
			"../Source/HD_CommonUtilities/HD_DataBuffer.h",
			"../Source/HD_CommonUtilities/HD_GrowingArray.h",
			"../Source/HD_CommonUtilities/HD_HashMap.h",
			"../Source/HD_CommonUtilities/HD_Map.h",
			"../Source/HD_CommonUtilities/HD_Pair.h",
			"../Source/HD_CommonUtilities/HD_StaticArray.h",
			"../Source/HD_CommonUtilities/HD_StaticStack.h",
			"../Source/HD_CommonUtilities/HD_String.h",
			"../Source/HD_CommonUtilities/HD_String.cpp"
		},
		["Math"] =
		{
			"../Source/HD_CommonUtilities/HD_AABB_2D.h",
			"../Source/HD_CommonUtilities/HD_Math.h",
			"../Source/HD_CommonUtilities/HD_Vector2.h",
			"../Source/HD_CommonUtilities/HD_Vector3.h"
		},
		["Misc"] =
		{
			"../Source/HD_CommonUtilities/HD_ExeArgs.h",
			"../Source/HD_CommonUtilities/HD_ExeArgs.cpp",
			"../Source/HD_CommonUtilities/HD_Hash.h",
			"../Source/HD_CommonUtilities/HD_IsFundamental.h",
			"../Source/HD_CommonUtilities/HD_PreprocessorMacros.h",
			"../Source/HD_CommonUtilities/HD_Random.h",
			"../Source/HD_CommonUtilities/HD_Random.cpp",
			"../Source/HD_CommonUtilities/HD_Singleton.h",
			"../Source/HD_CommonUtilities/HD_Time.h",
			"../Source/HD_CommonUtilities/HD_Time.cpp",
			"../Source/HD_CommonUtilities/HD_Types.h",
			"../Source/HD_CommonUtilities/HD_Utilities.h"
		},
		["Natvis"] =
		{
			"../Source/HD_CommonUtilities/HD_CommonUtilities.natvis"
		},
		["Profiling"] =
		{
			"../Source/HD_CommonUtilities/HD_ScopedTimer.h",
			"../Source/HD_CommonUtilities/HD_ScopedTimer.cpp"
		}
	}
group("")
	
project("BlipBlop")
	dependson("HD_CommonUtilities")
	kind("StaticLib")
	language("C++")
	location("../Source/BlipBlop")
	files("../Source/BlipBlop/*.h")
	files("../Source/BlipBlop/*.cpp")
	libdirs("../Lib/HD_CommonUtilities")
	includedirs("../Source/HD_CommonUtilities")
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
	location("../Source/ModelViewer")
	files("../Source/ModelViewer/*.h")
	files("../Source/ModelViewer/*.cpp")
	libdirs("../Lib/HD_CommonUtilities", "../Lib/BlipBlop")
	includedirs("../Source/HD_CommonUtilities", "../Source/BlipBlop")
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