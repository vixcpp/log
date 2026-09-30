cmake_minimum_required(VERSION 3.20)
project(vix_log VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
  set(VIX_LOG_EXPORT_SET VixTargets)
else()
  set(VIX_LOG_EXPORT_SET vix_logTargets)
endif()

option(VIX_LOG_FETCH_THIRD_PARTY "Auto-fetch fmt and spdlog when unavailable" ON)
if (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD)
  set(VIX_LOG_FETCH_THIRD_PARTY OFF CACHE BOOL "Auto-fetch fmt and spdlog when unavailable" FORCE)
endif()

file(GLOB_RECURSE LOG_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

if (TARGET fmt::fmt-header-only)
  set(VIX_LOG_FMT_TARGET fmt::fmt-header-only)
elseif (TARGET fmt::fmt)
  set(VIX_LOG_FMT_TARGET fmt::fmt)
else()
  find_package(fmt CONFIG QUIET)
  if (TARGET fmt::fmt-header-only)
    set(VIX_LOG_FMT_TARGET fmt::fmt-header-only)
  elseif (TARGET fmt::fmt)
    set(VIX_LOG_FMT_TARGET fmt::fmt)
  endif()
endif()

if (TARGET spdlog::spdlog_header_only)
  set(VIX_LOG_SPDLOG_TARGET spdlog::spdlog_header_only)
elseif (TARGET spdlog::spdlog)
  set(VIX_LOG_SPDLOG_TARGET spdlog::spdlog)
else()
  find_package(spdlog CONFIG QUIET)
  if (TARGET spdlog::spdlog_header_only)
    set(VIX_LOG_SPDLOG_TARGET spdlog::spdlog_header_only)
  elseif (TARGET spdlog::spdlog)
    set(VIX_LOG_SPDLOG_TARGET spdlog::spdlog)
  endif()
endif()

if ((NOT VIX_LOG_FMT_TARGET OR NOT VIX_LOG_SPDLOG_TARGET) AND VIX_LOG_FETCH_THIRD_PARTY)
  include(FetchContent)
  if (NOT VIX_LOG_FMT_TARGET)
    set(FMT_DOC OFF CACHE BOOL "" FORCE)
    set(FMT_TEST OFF CACHE BOOL "" FORCE)
    set(FMT_INSTALL ON CACHE BOOL "" FORCE)
    FetchContent_Declare(vix_log_fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 10.2.1)
    FetchContent_MakeAvailable(vix_log_fmt)
    if (TARGET fmt::fmt-header-only)
      set(VIX_LOG_FMT_TARGET fmt::fmt-header-only)
    elseif (TARGET fmt::fmt)
      set(VIX_LOG_FMT_TARGET fmt::fmt)
    endif()
  endif()
  if (NOT VIX_LOG_SPDLOG_TARGET)
    set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)
    set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
    set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
    set(SPDLOG_INSTALL ON CACHE BOOL "" FORCE)
    FetchContent_Declare(vix_log_spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.14.1)
    FetchContent_MakeAvailable(vix_log_spdlog)
    if (TARGET spdlog::spdlog_header_only)
      set(VIX_LOG_SPDLOG_TARGET spdlog::spdlog_header_only)
    elseif (TARGET spdlog::spdlog)
      set(VIX_LOG_SPDLOG_TARGET spdlog::spdlog)
    endif()
  endif()
endif()

if (NOT VIX_LOG_FMT_TARGET OR NOT VIX_LOG_SPDLOG_TARGET)
  message(FATAL_ERROR "[log] fmt and spdlog are required logging implementations.")
endif()

if (LOG_SOURCES)
  add_library(vix_log STATIC ${LOG_SOURCES})
  add_library(vix::log ALIAS vix_log)
  target_compile_features(vix_log PUBLIC cxx_std_20)
  target_include_directories(vix_log PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  target_link_libraries(vix_log
    PUBLIC ${VIX_LOG_FMT_TARGET}
    PRIVATE ${VIX_LOG_SPDLOG_TARGET})
  if (NOT MSVC)
    target_compile_options(vix_log PRIVATE -Wall -Wextra -Wpedantic)
  endif()
  if (VIX_ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(vix_log PRIVATE -fno-omit-frame-pointer -fsanitize=address,undefined)
    target_link_options(vix_log PRIVATE -fsanitize=address,undefined)
  endif()
else()
  add_library(vix_log INTERFACE)
  add_library(vix::log ALIAS vix_log)
  target_compile_features(vix_log INTERFACE cxx_std_20)
  target_include_directories(vix_log INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  target_link_libraries(vix_log INTERFACE ${VIX_LOG_FMT_TARGET})
endif()

set_target_properties(vix_log PROPERTIES OUTPUT_NAME vix_log VERSION ${PROJECT_VERSION} SOVERSION 0 EXPORT_NAME log)
install(TARGETS vix_log EXPORT ${VIX_LOG_EXPORT_SET}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if (NOT (DEFINED VIX_UMBRELLA_BUILD AND VIX_UMBRELLA_BUILD))
  install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")
  configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/vix_logConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/vix_logConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_log")
  write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/vix_logConfigVersion.cmake"
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/vix_logConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/vix_logConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_log")
  install(EXPORT vix_logTargets FILE vix_logTargets.cmake NAMESPACE vix::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/vix_log")
endif()

option(VIX_LOG_BUILD_TESTS "Build log module tests" OFF)
if (VIX_LOG_BUILD_TESTS)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()
