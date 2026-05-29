projectRootFolder = ".."
projectRootPath = projectRootFolder .. "/"

sourceHDCommonUtilitiesFolder = projectRootPath .. "Source/HD_CommonUtilities"
sourceHDCommonUtilitiesPath = sourceHDCommonUtilitiesFolder .. "/"

sourceBlipBlopFolder = projectRootPath .. "Source/BlipBlop"
sourceBlipBlopPath = sourceBlipBlopFolder .. "/"

sourceModelViewerFolder = projectRootPath .. "Source/ModelViewer"
sourceModelViewerPath = sourceModelViewerFolder .. "/"

workspace("BlipBlop")
	configurations({ "Debug", "Release", "Retail" })
	platforms("x64")
	location(projectRootFolder)
	
group("Utilities")
	project("HD_CommonUtilities")
		kind("StaticLib")
		language("C++")
		cppdialect("C++17")
	
		targetname("HD_CommonUtilities_$(Configuration)")
		targetdir("$(SolutionDir)Lib")
		objdir("$(SolutionDir)Intermediate/HD_CommonUtilities")
		location(sourceHDCommonUtilitiesFolder)
		files({ sourceHDCommonUtilitiesPath .. "*.h", sourceHDCommonUtilitiesPath .. "*.cpp", sourceHDCommonUtilitiesPath .. "HD_CommonUtilities.natvis" })
		pchheader("stdafx.h")
		pchsource(sourceHDCommonUtilitiesPath .. "stdafx.cpp")
		
		warnings("Extra")
		fatalwarnings("All")
		defines("_CRT_SECURE_NO_WARNINGS")
	
		filter("configurations:Retail")
			defines("_RETAIL")
	
		filter({})
	
		vpaths
		{
			["Containers"] =
			{
				sourceHDCommonUtilitiesPath .. "HD_ArrayIterator.h",
				sourceHDCommonUtilitiesPath .. "HD_Bitset.h",
				sourceHDCommonUtilitiesPath .. "HD_CircularArray.h",
				sourceHDCommonUtilitiesPath .. "HD_DataBuffer.h",
				sourceHDCommonUtilitiesPath .. "HD_GrowingArray.h",
				sourceHDCommonUtilitiesPath .. "HD_HashMap.h",
				sourceHDCommonUtilitiesPath .. "HD_Map.h",
				sourceHDCommonUtilitiesPath .. "HD_Pair.h",
				sourceHDCommonUtilitiesPath .. "HD_StaticArray.h",
				sourceHDCommonUtilitiesPath .. "HD_StaticStack.h",
				sourceHDCommonUtilitiesPath .. "HD_String.h"
			},
			["Math"] =
			{
				sourceHDCommonUtilitiesPath .. "HD_AABB_2D.h",
				sourceHDCommonUtilitiesPath .. "HD_Math.h",
				sourceHDCommonUtilitiesPath .. "HD_Vector2.h",
				sourceHDCommonUtilitiesPath .. "HD_Vector3.h",
				sourceHDCommonUtilitiesPath .. "HD_Vector4.h"
			},
			["Misc"] =
			{
				sourceHDCommonUtilitiesPath .. "HD_ExeArgs.h",
				sourceHDCommonUtilitiesPath .. "HD_ExeArgs.cpp",
				sourceHDCommonUtilitiesPath .. "HD_Format.h",
				sourceHDCommonUtilitiesPath .. "HD_Hash.h",
				sourceHDCommonUtilitiesPath .. "HD_IsFundamental.h",
				sourceHDCommonUtilitiesPath .. "HD_Logger.h",
				sourceHDCommonUtilitiesPath .. "HD_Logger.cpp",
				sourceHDCommonUtilitiesPath .. "HD_Move.h",
				sourceHDCommonUtilitiesPath .. "HD_PreprocessorMacros.h",
				sourceHDCommonUtilitiesPath .. "HD_Random.h",
				sourceHDCommonUtilitiesPath .. "HD_Random.cpp",
				sourceHDCommonUtilitiesPath .. "HD_SafeDelete.h",
				sourceHDCommonUtilitiesPath .. "HD_Singleton.h",
				sourceHDCommonUtilitiesPath .. "HD_StringUtils.h",
				sourceHDCommonUtilitiesPath .. "HD_Time.h",
				sourceHDCommonUtilitiesPath .. "HD_Time.cpp",
				sourceHDCommonUtilitiesPath .. "HD_Types.h",
				sourceHDCommonUtilitiesPath .. "HD_TypeTraits.h",
				sourceHDCommonUtilitiesPath .. "HD_Unused.h",
				sourceHDCommonUtilitiesPath .. "OptimizedWindowsInclude.h"
			},
			["Natvis"] =
			{
				sourceHDCommonUtilitiesPath .. "HD_CommonUtilities.natvis"
			},
			["Profiling"] =
			{
				sourceHDCommonUtilitiesPath .. "HD_ScopedTimer.h",
				sourceHDCommonUtilitiesPath .. "HD_ScopedTimer.cpp"
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
	location(sourceBlipBlopFolder)
	files({ sourceBlipBlopPath .. "*.h", sourceBlipBlopPath .. "*.cpp", sourceBlipBlopPath .. "*.hlsli", sourceBlipBlopPath .. "*.hlsl", sourceBlipBlopPath .. "*.natvis" })
	pchheader("stdafx.h")
	pchsource(sourceBlipBlopPath .. "stdafx.cpp")
	
	libdirs("$(SolutionDir)Lib")
	links({ "HD_CommonUtilities_$(Configuration)", "d3d11", "DXGI", "dxguid" })
	includedirs("$(SolutionDir)Source/HD_CommonUtilities")
		
	warnings("Extra")
	fatalwarnings("All")
	defines("_CRT_SECURE_NO_WARNINGS")

	shadervariablename("TEMP_%%(Filename)_ByteCode")
	shaderheaderfileoutput("$(SolutionDir)Source/BlipBlop/TemporaryShaders/%%(Filename).h")
	shaderobjectfileoutput("$(SolutionDir)Output/CompiledShaders/%%(Filename).cso")
	shaderoptions("/WX")

	filter("configurations:Retail")
		defines("_RETAIL")
	
	filter("files:" .. sourceBlipBlopPath .. "VS_*.hlsl")
		shadertype("Vertex")
		
	filter("files:" .. sourceBlipBlopPath .. "PS_*.hlsl")
		shadertype("Pixel")
	
	filter({})
	
	vpaths
	{
		["Objects"] =
		{
			sourceBlipBlopPath .. "Buffer.h",
			sourceBlipBlopPath .. "Buffer.cpp",
			sourceBlipBlopPath .. "Texture.h",
			sourceBlipBlopPath .. "Texture.cpp",
			sourceBlipBlopPath .. "Vertex.h",
			sourceBlipBlopPath .. "Vertex.cpp"
		},
		["RHI"] =
		{
			sourceBlipBlopPath .. "RenderHardwareInterface.h",
			sourceBlipBlopPath .. "RenderHardwareInterface.cpp",
			sourceBlipBlopPath .. "RHIStructs.h"
		},
		["Shaders"] =
		{
			sourceBlipBlopPath .. "Common.hlsli",
			sourceBlipBlopPath .. "VS_VertexShader.hlsl",
			sourceBlipBlopPath .. "PS_PixelShader.hlsl"
		}
	}
	
project("ModelViewer")
	dependson({ "HD_CommonUtilities", "BlipBlop" })
	kind("WindowedApp")
	language("C++")
	cppdialect("C++17")
	
	targetname("ModelViewer_$(Configuration)")
	targetdir("$(SolutionDir)Output")
	objdir("$(SolutionDir)Intermediate/ModelViewer")
	location(sourceModelViewerFolder)
	files({ sourceModelViewerPath .. "ModelViewer/*.h", sourceModelViewerPath .. "ModelViewer/*.cpp" })
	pchheader("stdafx.h")
	pchsource(sourceModelViewerPath .. "stdafx.cpp")
	
	libdirs("$(SolutionDir)Lib")
	links({ "HD_CommonUtilities_$(Configuration)", "BlipBlop_$(Configuration)" })
	includedirs({ "$(SolutionDir)Source/HD_CommonUtilities", "$(SolutionDir)Source/BlipBlop" })
	
	warnings("Extra")
	fatalwarnings("All")
	defines("_CRT_SECURE_NO_WARNINGS")

	filter("configurations:Retail")
		defines("_RETAIL")
	
	filter({})