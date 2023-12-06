#set(OpenGL_GL_PREFERENC LEGACY)
SET(CXX_STANDARD 17)

find_package (Threads REQUIRED)
find_package (glfw3 REQUIRED)

find_package(mujoco REQUIRED)
#link_libraries(mujoco::mujoco)

set(CPP_SOURCES ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${LF_MAIN_TARGET} ${CMAKE_THREAD_LIBS_INIT} mujoco::mujoco glfw)
target_sources(${LF_MAIN_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../src/mujoco_simulator.cpp)
