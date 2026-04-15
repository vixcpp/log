#  @file CMakeLists.txt
#  @author Gaspard Kirira
#
#  Copyright 2025, Gaspard Kirira. All rights reserved.
#  https://github.com/vixcpp/vix
#  Use of this source code is governed by a MIT license
#  that can be found in the License file.
#
# ====================================================================
# Vix.cpp - Log Module
# ====================================================================
# Purpose:
#   Public logging facade for Vix built on top of vix::utils::Logger.
#   This module does not reimplement a logger. It exposes a stable
#   vix::log API that delegates to the existing utils logger backend.
#
#   Builds as STATIC when sources exist, otherwise as a header-only
#   INTERFACE target.
#
# Public Targets:
#   - vix_log  : The actual library target (STATIC or INTERFACE)
#   - vix::log : Namespaced alias for consumers
#
# Public Dependencies:
#   - vix::utils
#
# Installation/Export:
#   Installs into the umbrella export-set `VixTargets`.
# ====================================================================

cmake_minimum_required(VERSION 3.20)
project(vix_log VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --------------------------------------------------------------------
# Umbrella build policy:
# When building inside the Vix umbrella, dependencies are managed by the
# umbrella (add_subdirectory order + options). Log must not FetchContent
# nor add sibling subdirectories in umbrella mode.
# --------------------------------------------------------------------
if (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
  set(VIX_LOG_FETCH_UTILS OFF CACHE BOOL "Auto-fetch vix::utils if missing" FORCE)
endif()

# Sources discovery
file(GLOB_RECURSE LOG_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Utils dependency
option(VIX_LOG_FETCH_UTILS "Auto-fetch vix::utils if missing" ON)

if (NOT TARGET vix::utils AND NOT TARGET vix_utils)
  if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/../utils/CMakeLists.txt")
    message(STATUS "[log] Adding utils from sibling: ../utils")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../utils" "utils")
  else()
    find_package(vix_utils CONFIG QUIET)
    find_package(utils CONFIG QUIET)
  endif()

  if (NOT TARGET vix::utils AND NOT TARGET vix_utils AND VIX_LOG_FETCH_UTILS)
    include(FetchContent)
    message(STATUS "[log] Fetching vix::utils via FetchContent")
    FetchContent_Declare(vix_utils
      GIT_REPOSITORY https://github.com/vixcpp/utils.git
      GIT_TAG        v0.1.0
    )
    FetchContent_MakeAvailable(vix_utils)
  endif()
endif()

set(VIX_UTILS_TARGET "")
if (TARGET vix::utils)
  set(VIX_UTILS_TARGET vix::utils)
elseif (TARGET vix_utils)
  set(VIX_UTILS_TARGET vix_utils)
endif()

if (NOT VIX_UTILS_TARGET)
  message(FATAL_ERROR
    "vix::utils not found.\n"
    "Provide ../utils, install/export the utils package, or enable VIX_LOG_FETCH_UTILS."
  )
endif()

# STATIC
if (LOG_SOURCES)
  message(STATUS "[log] Building STATIC library with detected sources.")

  add_library(vix_log STATIC ${LOG_SOURCES})
  add_library(vix::log ALIAS vix_log)

  target_compile_features(vix_log PUBLIC cxx_std_20)

  target_include_directories(vix_log
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  target_link_libraries(vix_log
    PUBLIC
      ${VIX_UTILS_TARGET}
  )

  if (TARGET vix_warnings)
    target_link_libraries(vix_log PUBLIC vix_warnings)
  endif()

  if (VIX_ENABLE_SANITIZERS AND TARGET vix_sanitizers)
    target_link_libraries(vix_log PUBLIC vix_sanitizers)
  endif()

  set_target_properties(vix_log PROPERTIES
    OUTPUT_NAME vix_log
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    EXPORT_NAME log
  )

  install(TARGETS vix_log
    EXPORT VixTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

# HEADER-ONLY
else()
  message(STATUS "[log] Building HEADER-ONLY library (no sources).")

  add_library(vix_log INTERFACE)
  add_library(vix::log ALIAS vix_log)

  target_compile_features(vix_log INTERFACE cxx_std_20)

  target_include_directories(vix_log INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  target_link_libraries(vix_log INTERFACE
    ${VIX_UTILS_TARGET}
  )

  if (VIX_ENABLE_SANITIZERS AND TARGET vix_sanitizers)
    target_link_libraries(vix_log INTERFACE vix_sanitizers)
  endif()

  install(TARGETS vix_log
    EXPORT VixTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

endif()

# Install headers
if (NOT (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD))
  install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
          FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")
endif()

# Package config
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/vix_logConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/vix_logConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_log"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/vix_logConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/vix_logConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/vix_logConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_log"
)

install(EXPORT VixTargets
  FILE VixTargets.cmake
  NAMESPACE vix::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_log"
)

# Tests
option(VIX_LOG_BUILD_TESTS "Build log module tests" OFF)

if (VIX_LOG_BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()

# Summary
message(STATUS "------------------------------------------------------")
message(STATUS "vix::log configured (${PROJECT_VERSION})")
if (LOG_SOURCES)
  message(STATUS "Mode: STATIC / sources found")
else()
  message(STATUS "Mode: HEADER-ONLY / no sources")
endif()
message(STATUS "Utils target: ${VIX_UTILS_TARGET}")
message(STATUS "Include dir: ${CMAKE_CURRENT_SOURCE_DIR}/include (for <vix/log/...>)")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "------------------------------------------------------")
