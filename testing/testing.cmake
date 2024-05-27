SET(CXX_STANDARD 17)

# add cpp files to library
ADD_LIBRARY(testing_cpp_files_common
    ${CMAKE_CURRENT_LIST_DIR}/../src/common/common_vector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../src/common/common_deacceleration_controller.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../src/common/common_planner_sanity_checker_logic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test_common/random_vectors_handler.cpp)

# add hpp files to library
target_include_directories(testing_cpp_files_common PRIVATE ${CMAKE_CURRENT_LIST_DIR}/test_common/ ${CMAKE_CURRENT_LIST_DIR}/../src/common/)

# link the library 
target_link_libraries(${LF_MAIN_TARGET} testing_cpp_files_common)

# make the library visible also for the .lf files
target_include_directories(${LF_MAIN_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/test_common/ ${CMAKE_CURRENT_LIST_DIR}/../src/common/)




