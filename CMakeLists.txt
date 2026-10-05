cmake_minimum_required(VERSION 3.23)

set(SPOOLES_MSVC_RUNTIME "dynamic" CACHE STRING
  "MSVC-compatible runtime linkage: dynamic (/MD) or static (/MT)")
set_property(CACHE SPOOLES_MSVC_RUNTIME PROPERTY STRINGS dynamic static)

if(SPOOLES_MSVC_RUNTIME STREQUAL "dynamic")
  set(CMAKE_MSVC_RUNTIME_LIBRARY
    "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
elseif(SPOOLES_MSVC_RUNTIME STREQUAL "static")
  set(CMAKE_MSVC_RUNTIME_LIBRARY
    "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
  message(FATAL_ERROR
    "SPOOLES_MSVC_RUNTIME must be either 'dynamic' or 'static'")
endif()

project(SPOOLES VERSION 2.2 LANGUAGES C CXX)

option(SPOOLES_ENABLE_TESTING "Build SPOOLES tests" ON)

list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

include(GNUInstallDirs)
find_package(Threads REQUIRED)

add_subdirectory(src)

if(SPOOLES_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(src/A2/tests)
  add_subdirectory(src/Lock/tests)
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake/SPOOLESConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/SPOOLESConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/SPOOLES")

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/SPOOLESConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/SPOOLESConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/SPOOLESConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/SPOOLES")

install(EXPORT SPOOLESTargets
  FILE SPOOLESTargets.cmake
  NAMESPACE SPOOLES::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/SPOOLES")
