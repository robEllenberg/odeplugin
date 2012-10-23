###########################################
# oderave openrave plugin
###########################################
cmake_minimum_required(VERSION 2.6)
include(FindOpenRAVE.cmake)
include(CheckFunctionExists.cmake)
include(CheckCXXSourceCompiles.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/FindODE.cmake)
set(ODE_USE_MULTITHREAD)
set(PLUGIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../openHubo/plugins")
  include(FindOpenRAVE.cmake)

  if( NOT OPENRAVE_FOUND )
    message(FATAL_ERROR "could not find openrave, check for openrave-config in path")
  endif( NOT OPENRAVE_FOUND )

if(ODE_LIBRARY_FOUND)
  link_directories(${OPENRAVE_LINK_DIRS} ${ODE_LINK_DIRS})

  # doesn't work
  set(CMAKE_REQUIRED_INCLUDES ${ODE_INCLUDE_DIRS} )
  set(CMAKE_REQUIRED_FLAGS "${ODE_CXXFLAGS}")
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    foreach(DIRNAME ${ODE_LINK_DIRS})
      set(CMAKE_REQUIRED_FLAGS "${ODE_CXXFLAGSCMAKE_REQUIRED_FLAGS} -L${DIRNAME}")
    endforeach()
  else()
   # perhaps there is an msvc way to do this?
  endif()
  set(CMAKE_REQUIRED_LIBRARIES ${ODE_LIBRARY} ${OPENRAVE_LIBRARY})
  check_function_exists(dAllocateODEDataForThread ODE_HAVE_ALLOCATE_DATA_THREAD)
  if( ODE_HAVE_ALLOCATE_DATA_THREAD )
      add_definitions("-DODE_HAVE_ALLOCATE_DATA_THREAD")
    if( WIN32 OR ODE_USE_MULTITHREAD )
      add_definitions("-DODE_USE_MULTITHREAD")
    else()
      message(STATUS "Detected ODE multi-threading extensions, but not sure if should enable them... define ODE_USE_MULTITHREAD variable to use them")
    endif()
  else()
    message(STATUS "ODE not compiled with multi-threaded extensions")
  endif()

  include_directories(${ODE_INCLUDE_DIRS} ${OPENRAVE_INCLUDE_DIRS} )
  add_library(odeplugin SHARED oderave.cpp odecollision.h odephysics.h odespace.h odecontroller.h plugindefs.h)
  
  # test for ode version 0.10
  check_cxx_source_compiles("
    #include <ode/ode.h>

    typedef int dTriIndex;
    int main()
    {
      return 0;
    }"
    NEED_TRIINDEX)
  set(CMAKE_REQUIRED_FLAGS)
  set(CMAKE_REQUIRED_INCLUDES)
  
  if( NEED_TRIINDEX )
    set(ODE_CXXFLAGS "${ODE_CXXFLAGS} -DNEED_DTRIINDEX_TYPEDEF")
  endif()

  message(STATUS "ODE found, building oderave plugin, precision=${ODE_PRECISION}")

  target_link_libraries(odeplugin ${ODE_LIBRARY} ${OPENRAVE_LIBRARY})
  set_target_properties(odeplugin PROPERTIES COMPILE_FLAGS "${PLUGIN_COMPILE_FLAGS} ${ODE_CXXFLAGS} ${OPENRAVE_CXXFLAGS}" LINK_FLAGS "${PLUGIN_LINK_FLAGS} ${OPENRAVE_LINK_FLAGS}")
  install(TARGETS odeplugin DESTINATION ${PLUGIN_DIR} COMPONENT ${COMPONENT_PREFIX}plugin-odeplugin)
  
else()
  message(STATUS "Could not find ODE. Please install ODE (http://www.ode.org)")
endif()

