find_path(RAYLIB_INCLUDE_DIR
    NAMES
        cmocka.h
    PATHS
        ${RAYLIB_ROOT_DIR}/include
)

find_library(RAYLIB_LIBRARY
    NAMES
        raylib
    PATHS
        ${RAYLIB_ROOT_DIR}/include
)

if(RAYLIB_LIBRARY)
  set(RAYLIB_LIBRARIES
      ${RAYLIB_LIBRARIES}
      ${RAYLIB_LIBRARY}
  )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CMocka DEFAULT_MSG RAYLIB_LIBRARIES RAYLIB_INCLUDE_DIR)

# show the RAYLIB_INCLUDE_DIR and RAYLIB_LIBRARY variables only in the advanced view
mark_as_advanced(RAYLIB_INCLUDE_DIR RAYLIB_LIBRARY)