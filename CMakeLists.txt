cmake_minimum_required(VERSION 3.28.3)
project(MyUtils LANGUAGES C VERSION 1.0.0)

if(MYUTILS_SHARED)
    add_library(MyUtils SHARED)
else()
    add_library(MyUtils STATIC)
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
message("SOURCES = ${SOURCES}")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/MyUtils/*.h")
message("HEADERS = ${HEADERS}")

set(ENABLE_PRINT_IF_AVAILABLE ON)
function(print_if_available level message)
    if (ENABLE_PRINT_IF_AVAILABLE)
        message(${level} ${message})
    endif ()
endfunction()

# 过滤平台特定的头文件和源文件
set(PLATFORM_SPECIFIC_WIN32_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/win32/")
set(PLATFORM_SPECIFIC_WIN32_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/MyUtils/win32/")
if (WIN32)
    message("正在在Windows平台上编译")
else ()
    message("正在在Linux/Unix/Macos平台上编译")
endif ()
foreach (source_file IN LISTS SOURCES)
    if (NOT WIN32)
        if (${source_file} MATCHES ${PLATFORM_SPECIFIC_WIN32_SOURCE_DIR})
            message("仅在Windows平台上可用的源文件${source_file}已被剔除")
        else ()
            print_if_available(STATUS "${source_file}将会被编译到库中")
            list(APPEND AVAILABLE_SOURCES ${source_file})
        endif ()
    else ()
        print_if_available(STATUS "${source_file}将会被编译到库中")
        list(APPEND AVAILABLE_SOURCES ${source_file})
    endif ()
endforeach ()
foreach (header_file IN LISTS HEADERS)
    if (NOT WIN32)
        if (header_file MATCHES ${PLATFORM_SPECIFIC_WIN32_HEADER_DIR})
            message("仅在Windows平台上可用的头文件${header_file}已被剔除")
        else ()
            print_if_available(STATUS "${header_file}头文件可用")
            list(APPEND AVAILABLE_HEADERS ${header_file})
        endif ()
    else ()
        print_if_available(STATUS "${header_file}头文件可用")
        list(APPEND AVAILABLE_HEADERS ${header_file})
    endif ()
endforeach ()

target_compile_features(MyUtils PRIVATE c_std_11)
target_sources(MyUtils PRIVATE ${AVAILABLE_SOURCES})
target_sources(MyUtils PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${AVAILABLE_HEADERS})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/MyUtilsConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/MyUtilsConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/MyUtilsConfig.cmake"
        INSTALL_DESTINATION lib/cmake/MyUtils
)

install(
        TARGETS MyUtils
        EXPORT MyUtils
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        FILE_SET HEADERS
)
install(
        EXPORT MyUtils
        NAMESPACE MyUtils::
        DESTINATION lib/cmake/MyUtils
)
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/MyUtilsConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/MyUtilsConfigVersion.cmake"
        DESTINATION lib/cmake/MyUtils
)