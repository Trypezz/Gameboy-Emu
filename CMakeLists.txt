cmake_minimum_required(VERSION 3.25)
project(Gameboy-Emu
    VERSION 1.0
    DESCRIPTION "Learning Project"
    LANGUAGES CXX
)

# =============================================================================
# Globale CMake-Settings (Modern C++ Best Practices)
# =============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Standard Build Type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

# =============================================================================
# Create executable
# =============================================================================
add_executable(Gameboy-Emu)

# =============================================================================
# Include Directories
# =============================================================================
target_include_directories(Gameboy-Emu PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

# =============================================================================
# Source Files
# =============================================================================
target_sources(Gameboy-Emu PRIVATE
    src/main.cpp
    src/Cartridge/cart.cpp
)

# =============================================================================
# Packages
# =============================================================================
find_package(SDL3 REQUIRED CONFIG)

target_link_libraries(Gameboy-Emu PRIVATE SDL3::SDL3)

# =============================================================================
# Compiler Warnings & Flags
# =============================================================================
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(Gameboy-Emu PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wunused
        -Wunused-parameter
        -Wno-missing-field-initializers
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(Gameboy-Emu PRIVATE DEBUG_BUILD=1)
endif()

message(STATUS "Application configured successfully!")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
