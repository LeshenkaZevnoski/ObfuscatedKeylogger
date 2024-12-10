cmake_minimum_required(VERSION 3.15)

# Project name
project(ObfuscatedKeylogger LANGUAGES CXX)

# Set C++ standard to the latest available
set(CMAKE_CXX_STANDARD 23) # Adjust as needed; replace with the latest known C++ standard, e.g., 26 for future versions
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/ObfuscatedKeylogger
)

# Source files
set(SOURCE_FILES
    ObfuscatedKeylogger/Keylogger.cpp
    ObfuscatedKeylogger/ObfuscatedKeylogger.cpp
    ObfuscatedKeylogger/Obfuscation.cpp
    ObfuscatedKeylogger/Transmission.cpp
)

# Header files
set(HEADER_FILES
    ObfuscatedKeylogger/Keylogger.h
    ObfuscatedKeylogger/Obfuscation.h
    ObfuscatedKeylogger/Transmission.h
    ObfuscatedKeylogger/resource.h
)

# Resource files (Windows-specific)
set(RESOURCE_FILES
    ObfuscatedKeylogger/ObfuscatedKeylogger.rc
)

# Add executable
add_executable(ObfuscatedKeylogger
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${RESOURCE_FILES}
)

# Link required libraries (Windows-specific)
if(WIN32)
    target_link_libraries(ObfuscatedKeylogger
        ws2_32 # Windows Sockets
        kernel32
        user32
        gdi32
    )
endif()

# Output directory
set_target_properties(ObfuscatedKeylogger PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Release/ARM/Release
)

# Packaging (Optional: Create a ZIP with executables and dependencies)
set(CPACK_GENERATOR "ZIP")
set(CPACK_ARCHIVE_FILE_NAME "ObfuscatedKeylogger")
include(CPack)