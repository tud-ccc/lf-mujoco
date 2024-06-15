SET(CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package (Threads)
find_package (glfw)
find_package(mujoco REQUIRED)
find_library(X_ARM_LIB xarm)
find_package(Eigen3 REQUIRED)


#link_libraries(mujoco::mujoco)

target_link_libraries(${LF_MAIN_TARGET} ${CMAKE_THREAD_LIBS_INIT} mujoco::mujoco glfw "${X_ARM_LIB}")

find_path(X_ARM_INCLUDE_PATH xarm)
target_include_directories(${LF_MAIN_TARGET} PUBLIC "${X_ARM_INCLUDE_PATH}")

# add cpp files to library
ADD_LIBRARY(cpp_files_common
    ${CMAKE_CURRENT_LIST_DIR}/common/common_planner_sanity_checker_logic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/common/common_vector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/common/common_world_data.cpp
    ${CMAKE_CURRENT_LIST_DIR}/common/common_deacceleration_controller.cpp
    )

# add hpp files to library
target_include_directories(cpp_files_common PRIVATE ${CMAKE_CURRENT_LIST_DIR}/common/ ${Eigen_INCLUDE_DIRS})
target_link_libraries(cpp_files_common ${Eigen_LIBRARIES})

# link the library 
target_link_libraries(${LF_MAIN_TARGET} cpp_files_common)

# make the library visible also for the .lf files
target_include_directories(${LF_MAIN_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/common/)

