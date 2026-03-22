# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Chess_Paring_system_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Chess_Paring_system_autogen.dir\\ParseCache.txt"
  "Chess_Paring_system_autogen"
  "QXlsx\\CMakeFiles\\QXlsx_autogen.dir\\AutogenUsed.txt"
  "QXlsx\\CMakeFiles\\QXlsx_autogen.dir\\ParseCache.txt"
  "QXlsx\\QXlsx_autogen"
  )
endif()
