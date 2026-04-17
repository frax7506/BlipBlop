workspace("BlipBlop")
	configurations({"Debug", "Release"})
	platforms("x64")
	location("..")
	
group("Utilities")
	project("HD_CommonUtilities")
	kind("StaticLib")
	language("C++")
	cppdialect("C++17")
	
	targetname("HD_CommonUtilities_$(Configuration)")
	targetdir("../Output/HD_CommonUtilities")
	objdir("../Intermediate/HD_CommonUtilities")
	location("../HD_CommonUtilities")
	files({ "../HD_CommonUtilities/*.h", "../HD_CommonUtilities/*.cpp", "../HD_CommonUtilities/HD_CommonUtilities.natvis" })
	pchheader("stdafx.h")
	pchsource("../HD_CommonUtilities/stdafx.cpp")
		
	warnings("Extra")
	fatalwarnings({"All"})
	defines("_CRT_SECURE_NO_WARNINGS")
	
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
	
targetname("BlipBlop_$(Configuration)")
targetdir("../Lib/BlipBlop")
objdir("../Intermediate/BlipBlop")
location("../Source/BlipBlop")
files({ "../Source/BlipBlop/*.h", "../Source/BlipBlop/*.cpp" })
pchheader("stdafx.h")
pchsource("../Source/BlipBlop/stdafx.cpp")
	
libdirs("../Lib/HD_CommonUtilities")
links("HD_CommonUtilities_$(Configuration)")
includedirs("../Source")
		
warnings("Extra")
fatalwarnings({"All"})
defines("_CRT_SECURE_NO_WARNINGS")
	
project("ModelViewer")
dependson({ "HD_CommonUtilities", "BlipBlop" })
kind("ConsoleApp")
language("C++")
cppdialect("C++17")
	
targetname("ModelViewer_$(Configuration)")
targetdir("../Output")
objdir("../Intermediate/ModelViewer")
location("../Source/ModelViewer")
files({ "../Source/ModelViewer/*.h", "../Source/ModelViewer/*.cpp" })
pchheader("stdafx.h")
pchsource("../Source/ModelViewer/stdafx.cpp")
	
libdirs({ "../Lib/HD_CommonUtilities", "../Lib/BlipBlop" })
links({ "HD_CommonUtilities_$(Configuration)", "BlipBlop_$(Configuration)" })
includedirs({ "../Source/HD_CommonUtilities", "../Source/BlipBlop" })
	
warnings("Extra")
fatalwarnings({"All"})
defines("_CRT_SECURE_NO_WARNINGS")