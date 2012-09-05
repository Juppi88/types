-- Base types (Vector, linked list etc)

project "Types"
	kind "StaticLib"
	language "C"
	files { "**.h", "**.c", "premake4.lua" }
	vpaths { [""] = { "../Libraries/Types" } }
	includedirs { ".", ".." }
	location ( "../../Projects/" .. os.get() .. "/" .. _ACTION )
	
	-- Linux specific stuff
	configuration "linux"
		targetextension ".a"
		configuration "Debug" targetname "libtypesd"
		configuration "Release" targetname "libtypes"
	
	-- Windows specific stuff
	configuration "windows"
		targetextension ".lib"
		configuration "Debug" targetname "typesd"
		configuration "Release" targetname "types"
