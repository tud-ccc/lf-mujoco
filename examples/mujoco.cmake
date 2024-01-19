#set(OpenGL_GL_PREFERENC LEGACY)
SET(CXX_STANDARD 17)

find_package (Threads REQUIRED)
find_package (glfw3 REQUIRED)
find_package(mujoco REQUIRED)

find_library(X_ARM_LIB xarm)
target_link_libraries(${LF_MAIN_TARGET} "${X_ARM_LIB}")

target_link_libraries(${LF_MAIN_TARGET} ${CMAKE_THREAD_LIBS_INIT} mujoco::mujoco glfw)
target_sources(${LF_MAIN_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../src/simulator/mujoco_simulator.cpp)

find_path(X_ARM_INCLUDE_PATH xarm)
target_include_directories(${LF_MAIN_TARGET} PUBLIC "${X_ARM_INCLUDE_PATH}")
add_definitions(-lxarm)
