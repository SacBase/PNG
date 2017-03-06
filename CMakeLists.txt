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

FIND_PACKAGE (PNG REQUIRED)

SET (include_str)
FOREACH (path ${PNG_INCLUDE_DIRS})
    SET (include_str ${include_str} -I${path})
ENDFOREACH ()
SET (SAC2C_EXTRA_INC ${include_str}
     CACHE STRING "Extra include files that should be used by sac2c")
UNSET (include_str)

# TODO test if librt is ever needed, see SDL2:40, and add the check.

# For every target run CMakeLists.txt in src
FOREACH (TARGET IN ITEMS ${TARGETS})
    ADD_SUBDIRECTORY (src   src-${TARGET})
ENDFOREACH ()

# This build target is responsible for generating the package sac2crc file
ADD_CUSTOM_TARGET (create-sac2crc-file ALL
    COMMAND ${CMAKE_COMMAND}
        -DUSER_HOME="$ENV{HOME}"
        # XXX ideally we should use PROJECT_NAME, but sadly this has a dash `-'
        #     which sac2crc does not support within target names
        -DPACKAGE_NAME="png"
        -DLIB_PATH="${DLL_BUILD_DIR}"
        -DTREE_PATH="${DLL_BUILD_DIR}"
        -P "${PROJECT_SOURCE_DIR}/cmake-common/generate-sac2crc-file.cmake"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Creating package sac2crc file in user's home directory")
