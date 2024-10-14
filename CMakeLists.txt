cmake_minimum_required(VERSION 3.10)

project(FileEncryptor C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(FileEncryptor FileEncryptor.c)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(FileEncryptor PRIVATE -Wall -Werror -pedantic-errors -O2)
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(FileEncryptor PRIVATE /W4 /WX /permissive-)
else()
    message(WARNING "Unknown compiler. Check flags")
endif()

find_library(AKRYPT_BASE_LIB akrypt-base)
find_library(AKRYPT_LIB akrypt)

if (AKRYPT_BASE_LIB AND AKRYPT_LIB)
    target_link_libraries(FileEncryptor PRIVATE ${AKRYPT_BASE_LIB} ${AKRYPT_LIB})
else()
    message(FATAL_ERROR "Libraries not found")
endif()
