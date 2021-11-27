# https://cmake.org/cmake/help/latest/module/FindFLEX.html
FIND_PACKAGE(FLEX REQUIRED)
# https://cmake.org/cmake/help/latest/module/FindBISON.html
FIND_PACKAGE(BISON REQUIRED)

IF(BISON_FOUND)
  BISON_TARGET(ParserTarget
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    VERBOSE REPORT_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.report
  )
ENDIF()

IF(FLEX_FOUND)
  FLEX_TARGET(ScannerTarget
    ${CMAKE_CURRENT_SOURCE_DIR}/scanner.l
    ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp
  )
ENDIF()

ADD_FLEX_BISON_DEPENDENCY(ScannerTarget ParserTarget)

message(STATUS "flex target outputs: ${FLEX_ScannerTarget_OUTPUTS}")
message(STATUS "bison target outputs: ${BISON_ParserTarget_OUTPUTS}")
message(STATUS "flex libraries: ${FLEX_LIBRARIES}")

INCLUDE_DIRECTORIES(../compilers)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/compilers)

FILE(GLOB_RECURSE ALL_LIB_CPP *.hpp)

ADD_LIBRARY(compilers
  ${ALL_LIB_CPP}
  ${BISON_ParserTarget_OUTPUTS}
  ${FLEX_ScannerTarget_OUTPUTS}
)

SET_TARGET_PROPERTIES(compilers PROPERTIES LINKER_LANGUAGE CXX)

TARGET_LINK_LIBRARIES(compilers
  ${DEPS}
  ${FLEX_LIBRARIES}
)
