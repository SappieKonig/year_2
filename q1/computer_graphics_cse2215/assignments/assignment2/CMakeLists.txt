cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(ComputerGraphics C CXX)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/framework")
	# Create framework library and include CMake scripts (compiler warnings, sanitizers and static analyzers).
	add_subdirectory("framework")
else()
	# During development the framework lives in parent folder.
	add_subdirectory("../../../framework/" "${CMAKE_BINARY_DIR}/framework/")
endif()

add_subdirectory("robot_arm")
add_subdirectory("solar_system")
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/grading_tests")
	add_subdirectory("grading_tests")
endif()
