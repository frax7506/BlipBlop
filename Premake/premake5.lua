solutionDir = "../"

workspace("BlipBlop")
	configurations({ "Debug", "Release", "Retail" })
	platforms("x64")
	location("..")
	
group("Utilities")
	project("HD_CommonUtilities")
		kind("StaticLib")
		language("C++")
		cppdialect("C++17")
	
		targetname("HD_CommonUtilities_$(Configuration)")
		targetdir("../Lib")
		objdir("../Intermediate/HD_CommonUtilities")
		location("../Source/HD_CommonUtilities")
		files({ "../Source/HD_CommonUtilities/*.h", "../Source/HD_CommonUtilities/*.cpp", "../Source/HD_CommonUtilities.natvis" })
		pchheader("stdafx.h")
		pchsource("../Source/HD_CommonUtilities/stdafx.cpp")
		
		warnings("Extra")
		fatalwarnings({"All"})
		defines("_CRT_SECURE_NO_WARNINGS")
	
		filter("configurations:Retail")
			defines("_RETAIL")
	
		filter({})
	
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
				"../Source/HD_CommonUtilities/HD_String.h"
			},
			["Math"] =
			{
				"../Source/HD_CommonUtilities/HD_AABB_2D.h",
				"../Source/HD_CommonUtilities/HD_Math.h",
				"../Source/HD_CommonUtilities/HD_Vector2.h",
				"../Source/HD_CommonUtilities/HD_Vector3.h",
				"../Source/HD_CommonUtilities/HD_Vector4.h"
			},
			["Misc"] =
			{
				"../Source/HD_CommonUtilities/HD_Bitset.h",
				"../Source/HD_CommonUtilities/HD_ExeArgs.h",
				"../Source/HD_CommonUtilities/HD_ExeArgs.cpp",
				"../Source/HD_CommonUtilities/HD_Format.h",
				"../Source/HD_CommonUtilities/HD_Hash.h",
				"../Source/HD_CommonUtilities/HD_IsFundamental.h",
				"../Source/HD_CommonUtilities/HD_Logger.h",
				"../Source/HD_CommonUtilities/HD_Logger.cpp",
				"../Source/HD_CommonUtilities/HD_PreprocessorMacros.h",
				"../Source/HD_CommonUtilities/HD_Random.h",
				"../Source/HD_CommonUtilities/HD_Random.cpp",
				"../Source/HD_CommonUtilities/HD_Singleton.h",
				"../Source/HD_CommonUtilities/HD_StringUtils.h",
				"../Source/HD_CommonUtilities/HD_Time.h",
				"../Source/HD_CommonUtilities/HD_Time.cpp",
				"../Source/HD_CommonUtilities/HD_Types.h",
				"../Source/HD_CommonUtilities/HD_Utilities.h",
				"../Source/HD_CommonUtilities/OptimizedWindowsInclude.h"
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
	cppdialect("C++17")
	
	targetname("BlipBlop_$(Configuration)")
	targetdir("$(SolutionDir)Lib")
	objdir("$(SolutionDir)Intermediate/BlipBlop")
	location(solutionDir .. "Source/BlipBlop")
	files({ solutionDir .. "Source/BlipBlop/*.h", solutionDir .. "Source/BlipBlop/*.cpp", solutionDir .. "Source/BlipBlop/*.hlsli", solutionDir .. "Source/BlipBlop/*.hlsl", solutionDir .. "Source/BlipBlop/*.natvis" })
	pchheader("stdafx.h")
	pchsource(solutionDir .. "Source/BlipBlop/stdafx.cpp")
	
	libdirs("$(SolutionDir)Lib")
	links({ "HD_CommonUtilities_$(Configuration)", "d3d11", "DXGI", "dxguid" })
	includedirs("$(SolutionDir)Source/HD_CommonUtilities")
		
	warnings("Extra")
	fatalwarnings({"All"})
	defines("_CRT_SECURE_NO_WARNINGS")

	shadervariablename("TEMP_%%(Filename)_ByteCode")
	shaderheaderfileoutput("$(SolutionDir)Source/BlipBlop/TemporaryShaders/%%(Filename).h")
	shaderobjectfileoutput("$(SolutionDir)Output/CompiledShaders/%%(Filename).cso")
	shaderoptions("/WX")

	filter("configurations:Retail")
		defines("_RETAIL")
	
	filter("files:" .. solutionDir .. "Source/BlipBlop/VS_*.hlsl")
		shadertype("Vertex")
		
	filter("files:" .. solutionDir .. "Source/BlipBlop/PS_*.hlsl")
		shadertype("Pixel")
	
	filter({})
	
	vpaths
	{
		["Objects"] =
		{
			solutionDir .. "Source/BlipBlop/Buffer.h",
			solutionDir .. "Source/BlipBlop/Buffer.cpp",
			solutionDir .. "Source/BlipBlop/Texture.h",
			solutionDir .. "Source/BlipBlop/Texture.cpp",
			solutionDir .. "Source/BlipBlop/Vertex.h",
			solutionDir .. "Source/BlipBlop/Vertex.cpp"
		},
		["RHI"] =
		{
			solutionDir .. "Source/BlipBlop/RenderHardwareInterface.h",
			solutionDir .. "Source/BlipBlop/RenderHardwareInterface.cpp",
			solutionDir .. "Source/BlipBlop/RHIStructs.h"
		},
		["Shaders"] =
		{
			solutionDir .. "Source/BlipBlop/Common.hlsli",
			solutionDir .. "Source/BlipBlop/VS_VertexShader.hlsl",
			solutionDir .. "Source/BlipBlop/PS_PixelShader.hlsl"
		}
	}
	
project("ModelViewer")
	dependson({ "HD_CommonUtilities", "BlipBlop" })
	kind("WindowedApp")
	language("C++")
	cppdialect("C++17")
	
	targetname("ModelViewer_$(Configuration)")
	targetdir("../Output")
	objdir("../Intermediate/ModelViewer")
	location("../Source/ModelViewer")
	files({ "../Source/ModelViewer/*.h", "../Source/ModelViewer/*.cpp" })
	pchheader("stdafx.h")
	pchsource("../Source/ModelViewer/stdafx.cpp")
	
	libdirs("../Lib")
	links({ "HD_CommonUtilities_$(Configuration)", "BlipBlop_$(Configuration)" })
	includedirs({ "../Source/HD_CommonUtilities", "../Source/BlipBlop" })
	
	warnings("Extra")
	fatalwarnings({"All"})
	defines("_CRT_SECURE_NO_WARNINGS")

	filter("configurations:Retail")
		defines("_RETAIL")
	
	filter({})