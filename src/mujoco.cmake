find_package (Threads)
find_package (glfw)

find_package(mujoco REQUIRED)
link_libraries(mujoco::mujoco)

set(CPP_SOURCES ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${LF_MAIN_TARGET} ${CMAKE_THREAD_LIBS_INIT} mujoco::mujoco glfw)

