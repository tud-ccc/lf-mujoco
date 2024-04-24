SET(CXX_STANDARD 17)
# in order to make a file visible in the whole target we can update the include path using this command
target_include_directories(${LF_MAIN_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../src/common/)
# in order to include also the .cpp files we execute these two commands
ADD_LIBRARY(cpp_files 
    ${CMAKE_CURRENT_LIST_DIR}/../src/common/common_vector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../src/common/common_position_evaluator.cpp)
target_link_libraries(${LF_MAIN_TARGET} cpp_files)


target_include_directories(${LF_MAIN_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/test_common/)