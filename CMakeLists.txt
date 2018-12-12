cmake_minimum_required (VERSION 3.12)

# VCPKG manager
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

# Project setup
project (gl-utils LANGUAGES CXX VERSION 0.0.1)
set (CMAKE_CXX_STANDARD 20)

##############################################
# Dependencies
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Catch2 REQUIRED)
find_path(GSL_INCLUDE_DIR gsl/gsl_util)
include_directories(${GSL_INCLUDE_DIR})
##############################################

add_library(gl-utils 
	src/window.cc
	src/input-manager.cc
)
# Alias for testing
add_library(gl-utils::gl-utils ALIAS gl-utils)

target_compile_options (gl-utils PRIVATE "-Wall" "-Werror" )
# Target Properties
target_include_directories(gl-utils
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(gl-utils
	PRIVATE 
		OpenGL::OpenGL
		GLEW::GLEW
		glfw
)

# Testing
enable_testing()
add_subdirectory(test)