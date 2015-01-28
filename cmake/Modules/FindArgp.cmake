# - Try to find Argp
# Once done, this will define
#
#  ARGP_FOUND - system has Argp
#  ARGP_INCLUDE_DIR - the Argp include directories
#  ARGP_LIBRARY - link these to use Argp

include(LibFindMacros)

# Include dir
find_path(ARGP_INCLUDE_DIR
  NAMES argp.h
)

# Finally the library itself
find_library(ARGP_LIBRARY
  NAMES argp
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(ARGP_PROCESS_INCLUDES ARGP_INCLUDE_DIR)
set(ARGP_PROCESS_LIBS ARGP_LIBRARY)
libfind_process(ARGP)