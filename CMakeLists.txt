cmake_minimum_required(VERSION 3.15)
project(lazyvariant VERSION 1.3.0 LANGUAGES CXX)
add_definitions (-Dlazy_variant_version="${CMAKE_PROJECT_VERSION}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_MODE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(
    include/
)

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

target_compile_definitions(lazyvariant PRIVATE LazyVariant_LIBRARY)


option(variant_tests "build tests of lazy variant" OFF)
if(variant_tests)
add_subdirectory(tests)
endif(variant_tests)


target_include_directories(lazyvariant PUBLIC include)


string(JOIN ";" LazyVAR_HEADERS_STRING ${LazyVAR_HEADERS})
set_target_properties(lazyvariant PROPERTIES
    PUBLIC_HEADER "${LazyVAR_HEADERS_STRING}"
)

# install(TARGETS lazyvariant)


include(GNUInstallDirs)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/lazyvariantConfig.cmake"
    INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/lazyvariant
)

install(FILES ${LazyVAR_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LazyVariant)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lazyvariantConfig.cmake
#              ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lazyvariant)
install(TARGETS lazyvariant
    #PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LazyVariant
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
