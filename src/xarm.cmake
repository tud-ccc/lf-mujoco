SET(CXX_STANDARD 17)

find_library(X_ARM_LIB xarm)
target_link_libraries(${LF_MAIN_TARGET} "${X_ARM_LIB}")

find_path(X_ARM_INCLUDE_PATH xarm)
target_include_directories(${LF_MAIN_TARGET} PUBLIC "${X_ARM_INCLUDE_PATH}")

# in order to make a file visible in the whole target we can update the include path using this command
target_include_directories(${LF_MAIN_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/common/)