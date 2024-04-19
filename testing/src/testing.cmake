SET(CXX_STANDARD 17)
# in order to make a file visible in the whole target we can update the include path using this command
target_include_directories(${LF_MAIN_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../src/common/)

target_include_directories(${LF_MAIN_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/test_common/)