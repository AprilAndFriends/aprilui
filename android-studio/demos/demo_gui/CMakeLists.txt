cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_VERBOSE_MAKEFILE on)

project(demo_gui)

include("../../../../hltypes/android-studio/generic/CMakeLists.txt")

file(
	GLOB_RECURSE CppSrc
	"../../../demos/demo_gui/*.cpp"
)

add_library(demo_gui SHARED ${CppSrc})

include_directories(AFTER "../../../../atres/include")
include_directories(AFTER "../../../../april/include")
include_directories(AFTER "../../../../hltypes/include")
include_directories(AFTER "../../../../gtypes/include")

add_subdirectory("../../lib" aprilui)
add_subdirectory("../../../../atres/android-studio/lib" atres)
add_subdirectory("../../../../april/android-studio/lib" april)
add_subdirectory("../../../../hltypes/hlxml/android-studio/lib" hlxml)
add_subdirectory("../../../../hltypes/android-studio/lib" hltypes)
add_subdirectory("../../../../gtypes/android-studio/lib" gtypes)
add_subdirectory("../../../../april/lib/jpeg/android-studio/lib" jpeg)
add_subdirectory("../../../../april/lib/png/android-studio/lib" png)

target_link_libraries(
	demo_gui
	aprilui
	atres
	april
	hlxml
	hltypes
	gtypes
	jpeg
	png
	${liblog}
	${libz}
	${libc}
	${libdl}
	${libandroid}
	${libGLESv2}
)
