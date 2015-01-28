cmake_minimum_required (VERSION 2.8.4)
project (Tifixity)

set (PROJECT "Tifixity")

# Version Info
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_REVISION 0)

# Bug/Issues Address
set (ISSUES_ADDRESS "<https://github.com/pmay/Tifixity/issues>")

set (Tifixity_SRCS src/tiffdatacs.c)
add_executable(tifixity ${Tifixity_SRCS})

# Configure header file to pass relevant cmake settings to source
configure_file (
  "${PROJECT_SOURCE_DIR}/src/tifixityConfig.h.in"
  "${PROJECT_BINARY_DIR}/tifixityConfig.h"
)

# Set cmake module path to include our own modules, e.g. for argp
set(TIFIXITY_CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${TIFIXITY_CMAKE_MODULE_PATH}")

# Search for required libraries
FIND_PACKAGE(Argp REQUIRED)		# Processing CLI
FIND_PACKAGE(OpenSSL REQUIRED)	# MD5 Checksum
FIND_PACKAGE(Tiff REQUIRED)		# Tiff file access

# Libraries found
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR} ${OPENSSL_INCLUDE_DIR} ${TIFF_INCLUDE_DIR} ${ARGP_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(tifixity ${OPENSSL_LIBRARIES} ${TIFF_LIBRARIES} ${ARGP_LIBRARY})
