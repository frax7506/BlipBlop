projectRootFolder = ".."
projectRootPath = projectRootFolder .. "/"

projectFilesExternalFolder = projectRootPath .. "ProjectFiles/External"
projectFilesBlipBlopFolder = projectRootPath .. "ProjectFiles/BlipBlop"
projectFilesModelViewerFolder = projectRootPath .. "ProjectFiles/ModelViewer"

sourceExternalFolder = projectRootPath .. "Source/External"
sourceExternalPath = sourceExternalFolder .. "/"

sourceBlipBlopFolder = projectRootPath .. "Source/BlipBlop"
sourceBlipBlopPath = sourceBlipBlopFolder .. "/"

sourceModelViewerFolder = projectRootPath .. "Source/ModelViewer"
sourceModelViewerPath = sourceModelViewerFolder .. "/"

workspace("BlipBlop")
	configurations({ "Debug", "Release", "Retail" })
	platforms("x64")
	location(projectRootFolder)
	
project("External")
	kind("StaticLib")
	language("C++")
	cppdialect("C++17")
	
	targetname("External_$(Configuration)")
	targetdir("$(SolutionDir)Output/External")
	objdir("$(SolutionDir)Intermediate/External")
	location(projectFilesExternalFolder)
	files
	{
		sourceExternalPath .. "**.h",
		sourceExternalPath .. "**.cpp",
		sourceExternalPath .. "**.natvis",
	}

	vpaths
	{
		["HD_CommonUtilities/*"] = sourceExternalPath .. "HD_CommonUtilities/**"
	}
	
	includedirs
	{
		"$(SolutionDir)Source/External/HD_CommonUtilities/Containers",
		"$(SolutionDir)Source/External/HD_CommonUtilities/Math",
		"$(SolutionDir)Source/External/HD_CommonUtilities/Misc",
		"$(SolutionDir)Source/External/HD_CommonUtilities/Profiling"
	}
	
	warnings("Extra")
	fatalwarnings("All")
	defines("_CRT_SECURE_NO_WARNINGS")
	
	filter("configurations:Retail")
		defines("_RETAIL")
	
	filter({})
	
project("BlipBlop")
	dependson("External")
	kind("StaticLib")
	language("C++")
	cppdialect("C++17")
	
	targetname("BlipBlop_$(Configuration)")
	targetdir("$(SolutionDir)Output/BlipBlop")
	objdir("$(SolutionDir)Intermediate/BlipBlop")
	location(projectFilesBlipBlopFolder)
	pchheader("stdafx.h")
	pchsource(sourceBlipBlopPath .. "stdafx.cpp")
	files
	{
		sourceBlipBlopPath .. "**.h",
		sourceBlipBlopPath .. "**.cpp",
		sourceBlipBlopPath .. "**.hlsli",
		sourceBlipBlopPath .. "**.hlsl"
	}
	
	libdirs("$(SolutionDir)Output/External")
	links({ "External_$(Configuration)", "d3d11", "DXGI", "dxguid" })
	includedirs
	{
		"$(SolutionDir)Source/External/HD_CommonUtilities/Containers",
		"$(SolutionDir)Source/External/HD_CommonUtilities/Math",
		"$(SolutionDir)Source/External/HD_CommonUtilities/Misc",
		"$(SolutionDir)Source/External/HD_CommonUtilities/Profiling",
		"$(SolutionDir)Source/BlipBlop",
		"$(SolutionDir)Source/BlipBlop/GraphicsEngine",
		"$(SolutionDir)Source/BlipBlop/Objects",
		"$(SolutionDir)Source/BlipBlop/RHI"
	}
		
	warnings("Extra")
	fatalwarnings("All")
	defines("_CRT_SECURE_NO_WARNINGS")

	shadervariablename("TEMP_%%(Filename)_ByteCode")
	shaderheaderfileoutput("$(SolutionDir)Source/BlipBlop/TemporaryShaders/%%(Filename).h")
	shaderobjectfileoutput("$(SolutionDir)Output/BlipBlop/CompiledShaders/%%(Filename).cso")
	shaderoptions("/WX")

	filter("configurations:Retail")
		defines("_RETAIL")
	
	filter("files:" .. sourceBlipBlopPath .. "Shaders/" .. "VS_*.hlsl")
		shadertype("Vertex")
		
	filter("files:" .. sourceBlipBlopPath .. "Shaders/" .. "PS_*.hlsl")
		shadertype("Pixel")
		
	filter("files:" .. sourceExternalPath .. "**.natvis")
		buildaction("Natvis")
	
	filter({})
	
project("ModelViewer")
	dependson({ "External", "BlipBlop" })
	kind("WindowedApp")
	language("C++")
	cppdialect("C++17")
	
	targetname("ModelViewer_$(Configuration)")
	targetdir("$(SolutionDir)Output/ModelViewer")
	objdir("$(SolutionDir)Intermediate/ModelViewer")
	location(projectFilesModelViewerFolder)
	pchheader("stdafx.h")
	pchsource(sourceModelViewerPath .. "stdafx.cpp")
	files
	{
		sourceModelViewerPath .. "**.h",
		sourceModelViewerPath .. "**.cpp"
	}
	
	libdirs({"$(SolutionDir)Output/External", "$(SolutionDir)Output/BlipBlop"})
	links({ "External_$(Configuration)", "BlipBlop_$(Configuration)" })
	includedirs
	{
		"$(SolutionDir)Source/External/HD_CommonUtilities/Containers",
		"$(SolutionDir)Source/External/HD_CommonUtilities/Math",
		"$(SolutionDir)Source/External/HD_CommonUtilities/Misc",
		"$(SolutionDir)Source/External/HD_CommonUtilities/Profiling",
		"$(SolutionDir)Source/BlipBlop",
		"$(SolutionDir)Source/BlipBlop/GraphicsEngine",
		"$(SolutionDir)Source/BlipBlop/Objects",
		"$(SolutionDir)Source/BlipBlop/RHI"
	}
	
	warnings("Extra")
	fatalwarnings("All")
	defines("_CRT_SECURE_NO_WARNINGS")

	filter("configurations:Retail")
		defines("_RETAIL")
		
	filter("files:" .. sourceExternalPath .. "**.natvis")
		buildaction("Natvis")
	
	filter({})