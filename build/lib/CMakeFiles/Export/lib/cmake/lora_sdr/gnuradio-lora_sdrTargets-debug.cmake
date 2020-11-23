#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gnuradio::gnuradio-lora_sdr" for configuration "Debug"
set_property(TARGET gnuradio::gnuradio-lora_sdr APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(gnuradio::gnuradio-lora_sdr PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libgnuradio-lora_sdr.so.f4d78445"
  IMPORTED_SONAME_DEBUG "libgnuradio-lora_sdr.so.1.0.0git"
  )

list(APPEND _IMPORT_CHECK_TARGETS gnuradio::gnuradio-lora_sdr )
list(APPEND _IMPORT_CHECK_FILES_FOR_gnuradio::gnuradio-lora_sdr "${_IMPORT_PREFIX}/lib/libgnuradio-lora_sdr.so.f4d78445" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
