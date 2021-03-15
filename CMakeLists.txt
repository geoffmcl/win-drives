# CMakeLists.txt generated 2016/09/01 16:02:22
# 2021/03/15 - add sprtf log
# 2016/09/01 from vcproj05.pl, using C:\GTools\ConApps\drives\drives.sln

# local CMake Scripts.
# set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules )

cmake_minimum_required (VERSION 2.8.8)

project (drives)

# Setup MSVC 3rd party directories if available and needed
# include( ConfigureMsvc3rdParty )

if(CMAKE_COMPILER_IS_GNUCXX)
    set(WARNING_FLAGS_CXX "-Wall")
    set(WARNING_FLAGS_C   "-Wall")
endif(CMAKE_COMPILER_IS_GNUCXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
   set (WARNING_FLAGS_CXX "-Wall -Wno-overloaded-virtual")
   set (WARNING_FLAGS_C   "-Wall")
endif() 

if(WIN32 AND MSVC)
    set(MSVC_FLAGS "-D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS")
    # turn off various warnings
    # foreach(warning 4244 4251 4267 4275 4290 4786 4305 4996)
    #     set(MSVC_FLAGS "${MSVC_FLAGS} /wd${warning}")
    # endforeach()
    #set(NOMINMAX 1)
    # to distinguish between debug and release lib in windows
    set( CMAKE_DEBUG_POSTFIX "d" ) # little effect in unix
endif()

# Uncomment to REDUCE the Windows configurations buildable
# set(CMAKE_CONFIGURATION_TYPES "Release;Debug" CACHE STRING "" FORCE) # Disables MinSizeRel & MaxSpeedRel

set (BOOST_CXX_FLAGS "-DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -DBOOST_BIMAP_DISABLE_SERIALIZATION")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS_C} ${MSVC_FLAGS} -D_REENTRANT")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS_CXX} ${MSVC_FLAGS} -D_REENTRANT ${BOOST_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MSVC_LD_FLAGS}")

#------------------------------------------------------------------------
# Static Windows Runtime
#   Option to statically link to the Windows runtime. Maybe only 
#   applies to WIN32/MSVC.
#------------------------------------------------------------------------
if (MSVC)
    option( USE_STATIC_RUNTIME "Set ON to change /MD(DLL) to /MT(static)" ON )
    if (USE_STATIC_RUNTIME)
        set(CompilerFlags
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            )
        foreach(CompilerFlag ${CompilerFlags})
            string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach()    
        message(STATUS "Using /MT STATIC runtime")
    else ()
        message(STATUS "Using /MD DYNAMIC runtime")
    endif ()
endif ()

### 1 executables #############################################
# Project [drives], type [Console Application], 2 C/C++, 2 Hdrs, 1 Other, 0 INCS, 4 DEFS
# File dir [C:\GTools\ConApps\drives\], out [C:\GTools\ConApps\drives], rel = []
set(name drives)
set(dir src)
set( ${name}_SRCS ${dir}/drives.cxx ${dir}/drives_io.cxx ${dir}/sprtf.cxx)
set( ${name}_HDRS ${dir}/drives.hxx ${dir}/drives_io.hxx ${dir}/sprtf.hxx)
add_executable( ${name} ${${name}_SRCS} ${${name}_HDRS} )
if (add_LIBS)
target_Link_libraries(${name} ${add_LIBS} )
endif ()
if (MSVC)
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX d )
endif ()

# deal with install, if required
# install(TARGETS ${inst_LIBS}
#     RUNTIME DESTINATION bin
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib )
# install(TARGETS ${inst_BINS} DESTINATION bin)
# install(FILES ${inst_HDRS} DESTINATION include)

# eof
