cmake_minimum_required (VERSION 2.8.4)
project (Tifixity)
set (tifixity_SRCS src/tiffdatacs.c)
add_executable(tifixity ${tifixity_SRCS})

# Search for OpenSSL and Tiff Libraries
FIND_PACKAGE(OpenSSL REQUIRED)
FIND_PACKAGE(Tiff REQUIRED)

# OpenSSL and Tiff Libs found
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR} ${TIFF_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(tifixity ${OPENSSL_LIBRARIES} ${TIFF_LIBRARIES})
