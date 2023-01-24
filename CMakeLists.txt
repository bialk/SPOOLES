# Minimum required version of CMake
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)


# example:
# find_path_by_regex(VARIABLE_TO_RETURN_FOUND_PATH "boost_*[0-9]_*[0-9]_*[0-9]"
# PATHS ${LIST_OF_PATHS} )
function(find_path_by_regex var_out regex_in)
    foreach(item ${ARGN})
        if(item STREQUAL "PATHS")
            set(itemmode "PATHS")
            continue()
        endif()
        if(itemmode STREQUAL "PATHS")
           FILE(GLOB found_path "${item}/${regex_in}")
           message(STATUS found_path:${found_path})
           if(found_path)
               set(${var_out} ${found_path} PARENT_SCOPE)
               return()
           endif()
        endif()
    endforeach()
endfunction()



# temporarily variables need to be set as external parameters
# path to thirdparty dirs
set(WINDOWS_EXTERNAL_LIBS_PATH  "D:/trymix/test_omi" "D:/zero-devel")

# Set CMake project name
PROJECT(SPOOLES)

# Identify CMake directory
set(SPOOLES_CMAKE_DIR "${SPOOLES_SOURCE_DIR}/CMake")
if(SPOOLES_IS_SUBBUILD)
  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${SPOOLES_CMAKE_DIR}")
else(SPOOLES_IS_SUBBUILD)
  set(CMAKE_MODULE_PATH "${SPOOLES_CMAKE_DIR};${CMAKE_MODULE_PATH}")
endif(SPOOLES_IS_SUBBUILD)

if("${SPOOLES_ENABLE_TESTING}" STREQUAL "")
  set(SPOOLES_ENABLE_TESTING ON CACHE BOOL "Enable SPOOLES testing")
else("${SPOOLES_ENABLE_TESTING}" STREQUAL "")
  set(SPOOLES_ENABLE_TESTING ${SPOOLES_ENABLE_TESTING} CACHE BOOL "SPOOLES testing")
endif("${SPOOLES_ENABLE_TESTING}" STREQUAL "")

if(NOT M_LIBRARY)
  include(CheckLibraryExists)
  include(ResolveCompilerPaths)
  CHECK_LIBRARY_EXISTS(m cos "" HAVE_M_LIBRARY)
  if(HAVE_M_LIBRARY)
    RESOLVE_LIBRARIES(M_LIBRARY "-lm")
  endif(HAVE_M_LIBRARY)
endif(NOT M_LIBRARY)

# search for 32/64 bit pthread library on windows
# assumed that pthread binary placed on the same
# level with spooles directory
if(MSVC)
  find_path_by_regex(pthread_DIR "pthreads-w32-*[0-9]-*[0-9]-*[0-9]" PATHS ${WINDOWS_EXTERNAL_LIBS_PATH})

  FIND_PATH(pthread_INCLUDE_DIR pthread.h
    HINTS ${pthread_DIR} PATH_SUFFIXES Pre-built.2/include
    REQUIRED
  )

  FIND_LIBRARY(pthread_LIB pthreadVC2 REQUIRED
    HINTS ${pthread_DIR}/Pre-built.2/lib/x64
    REQUIRED
  )

  FIND_FILE(pthread_DLL Pre-built.2/dll/x64/pthreadVC2.dll
    HINTS ${pthread_DIR}
    REQUIRED
  )

  message(WARNING ${pthread_INCLUDE_DIR})
endif(MSVC)


add_subdirectory(src)

if(SPOOLES_ENABLE_TESTING)
  ENABLE_TESTING()
  add_subdirectory(src/A2/tests)
endif(SPOOLES_ENABLE_TESTING)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
