CMAKE_MINIMUM_REQUIRED (VERSION 3.3)

PROJECT (sac-png)
#FIXME(artem) we can create a definition for language "SAC" which will
#             automatically pull sac2c compiler.

# Where the compiled sac modules result
SET (DLL_BUILD_DIR  "${PROJECT_BINARY_DIR}/lib")

# For what targets we build modules
SET (TARGETS            seq mt_pth  CACHE STRING "Build stdlib-jpeg for these targets")
SET (SAC2C_EXEC                     CACHE STRING "A path to sac2c compiler")
SET (LINKSETSIZE        "0"         CACHE STRING "Set a value for -linksetsize parameter of sac2c")

# Check whether sac2c is operational
INCLUDE ("cmake-common/check-sac2c.cmake")
INCLUDE ("cmake-common/misc-macros.cmake")

FIND_PACKAGE (PNG REQUIRED)

SET (include_str)
FOREACH (path ${PNG_INCLUDE_DIRS})
    SET (include_str ${include_str} -I${path})
ENDFOREACH ()
SET (SAC2C_EXTRA_INC ${include_str}
     CACHE STRING "Extra include files that should be used by sac2c")
UNSET (include_str)

# For every target run CMakeLists.txt in src
FOREACH (TARGET IN ITEMS ${TARGETS})
    ADD_SUBDIRECTORY (src   src-${TARGET})
ENDFOREACH ()

CREATE_SAC2CRC_TARGET ("png" "${DLL_BUILD_DIR}" "${DLL_BUILD_DIR}" "")
