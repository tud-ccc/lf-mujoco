SET(CXX_STANDARD 17)

find_library(X_ARM_LIB xarm)
target_link_libraries(${LF_MAIN_TARGET} "${X_ARM_LIB}")

find_path(X_ARM_INCLUDE_PATH xarm)
target_include_directories(${LF_MAIN_TARGET} PUBLIC "${X_ARM_INCLUDE_PATH}")

# add cpp files to library
ADD_LIBRARY(cpp_files_common
    ${CMAKE_CURRENT_LIST_DIR}/common/common_vector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/common/common_position_evaluator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/common/common_world_data.cpp
    ${CMAKE_CURRENT_LIST_DIR}/common/common_deacceleration_controller.cpp
    )

# add hpp files to library
target_include_directories(cpp_files_common PRIVATE ${CMAKE_CURRENT_LIST_DIR}/common/)

# link the library 
target_link_libraries(${LF_MAIN_TARGET} cpp_files_common)

# make the library visible also for the .lf files
target_include_directories(${LF_MAIN_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/common/)

