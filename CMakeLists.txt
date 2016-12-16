# ---------------------------------------------------------------
# HLF: A Hand Learning Framework
# A Machine-Learning playground dedicated hand pose estimation
# Author: B. Halimi - 2016
# ---------------------------------------------------------------
project (HLF)
cmake_minimum_required (VERSION 2.8)
set(CMAKE_CXX_FLAGS "-O2")
# FIND ----------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/)
find_package(SFML 2 COMPONENTS system window graphics REQUIRED)
find_package(Eigen3 REQUIRED)
# INCLUDE -------------------------------------------------------
include_directories(
	${SFML_INCLUDE_DIR}
	${EIGEN3_INCLUDE_DIR}
	Framework/Include
#	Framework/HLF
)
add_subdirectory(
	Framework
)
# BUILD ---------------------------------------------------------
add_executable(HLF
	main.cpp
	${SOURCE}
)
# LINK ----------------------------------------------------------
target_link_libraries(HLF
	${SFML_LIBRARIES}
	"X11"
)
