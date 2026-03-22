#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "QXlsx::QXlsx" for configuration "Debug"
set_property(TARGET QXlsx::QXlsx APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(QXlsx::QXlsx PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/libQXlsxQt6.dll.a"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/libQXlsxQt6.dll"
  )

list(APPEND _cmake_import_check_targets QXlsx::QXlsx )
list(APPEND _cmake_import_check_files_for_QXlsx::QXlsx "${_IMPORT_PREFIX}/lib/libQXlsxQt6.dll.a" "${_IMPORT_PREFIX}/bin/libQXlsxQt6.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
