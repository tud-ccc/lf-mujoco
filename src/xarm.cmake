
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package (Threads)
# find_package (glfw)
find_package(mujoco REQUIRED)

find_library(X_ARM_LIB xarm)
find_package(Eigen3 REQUIRED)
find_package(realsense2 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

find_path(X_ARM_INCLUDE_PATH xarm)

ADD_LIBRARY(camera_library 
    ${CMAKE_CURRENT_LIST_DIR}/camera_library/library_source_files/camera_interface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/camera_library/library_source_files/common_blue_center_to_3d.cpp
    ${CMAKE_CURRENT_LIST_DIR}/camera_library/library_source_files/common.cpp
    ${CMAKE_CURRENT_LIST_DIR}/camera_library/library_source_files/data_structures.cpp)

target_include_directories(camera_library PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/common/) 

# add cpp files to library
ADD_LIBRARY(cpp_files_common
    ${CMAKE_CURRENT_LIST_DIR}/common/common_planner_sanity_checker_logic.cpp
    ${CMAKE_CURRENT_LIST_DIR}/common/common_vector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/common/common_world_data.cpp
    ${CMAKE_CURRENT_LIST_DIR}/common/common_deacceleration_controller.cpp
    )

# add hpp files to library
target_include_directories(cpp_files_common PRIVATE 
                ${CMAKE_CURRENT_LIST_DIR}/common/) 

# link the library 
target_link_libraries(${LF_MAIN_TARGET} camera_library
                                        cpp_files_common
                                        ${CMAKE_THREAD_LIBS_INIT} 
                                        mujoco::mujoco
                                        glfw
                                        OpenGL::GL
                                        GLU
                                        "${X_ARM_LIB}"
                                        realsense2::realsense2)

# make the library visible also for the .lf files
target_include_directories(${LF_MAIN_TARGET} PRIVATE 
                                    "${X_ARM_INCLUDE_PATH}"
                                    ${CMAKE_CURRENT_LIST_DIR}/common/
                                    ${CMAKE_CURRENT_LIST_DIR}/camera_library/)
                                    


