cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_VERBOSE_MAKEFILE on)

project(aprilui)

include("../../../hltypes/android-studio/generic/CMakeLists.txt")

file(
	GLOB_RECURSE CppSrc
	"../../src/*.c"
	"../../src/*.cpp"
)

add_library(aprilui STATIC ${CppSrc})

include_directories(AFTER "../../include/aprilui")
include_directories(AFTER "../../../atres/include")
include_directories(AFTER "../../../april/include")
include_directories(AFTER "../../../hltypes/hlxml/include")
include_directories(AFTER "../../../gtypes/include")
include_directories(AFTER "../../../hltypes/include")
include_directories(AFTER "../../lib/jpeg")
include_directories(AFTER "../../lib/png")
include_directories(AFTER "../../../hltypes/lib/zlib")

add_definitions(
	-DAPRILUI_EXPORTS
)
