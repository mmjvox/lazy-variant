cmake_minimum_required(VERSION 3.15)
project(lazyvariant VERSION 1.3.1 LANGUAGES CXX)
add_definitions (-Dlazy_variant_version="${CMAKE_PROJECT_VERSION}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_MODE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(LazyVAR_HEADERS
    include/DbVariant.h
    include/DbList.h
    include/Result.h
    include/ResultRow.h
)

add_library(lazyvariant STATIC
    ${LazyVAR_HEADERS}

    src/DbVariant.cpp
    src/DbList.cpp
    src/Result.cpp
    src/ResultRow.cpp
)
add_library(lazyvariant::lazyvariant ALIAS lazyvariant)

target_compile_definitions(lazyvariant PRIVATE LazyVariant_LIBRARY)


option(variant_tests "build tests of lazy variant" OFF)
if(variant_tests)
add_subdirectory(tests)
endif(variant_tests)


target_include_directories(lazyvariant
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/LazyVariant>
)

set_target_properties(lazyvariant PROPERTIES
    PUBLIC_HEADER "${LazyVAR_HEADERS}"
)

include(GNUInstallDirs)

set(LAZYVARIANT_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/LazyVariant")
set(LAZYVARIANT_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/lazyvariant")

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/lazyvariantConfig.cmake"
    INSTALL_DESTINATION ${LAZYVARIANT_INSTALL_CMAKEDIR}
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/lazyvariantConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(TARGETS lazyvariant
    EXPORT lazyvariantTargets
    PUBLIC_HEADER DESTINATION ${LAZYVARIANT_INSTALL_INCLUDEDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT lazyvariantTargets
    FILE lazyvariantTargets.cmake
    NAMESPACE lazyvariant::
    DESTINATION ${LAZYVARIANT_INSTALL_CMAKEDIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/lazyvariantConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/lazyvariantConfigVersion.cmake"
    DESTINATION ${LAZYVARIANT_INSTALL_CMAKEDIR}
)
