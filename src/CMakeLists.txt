
set(Python_ROOT_DIR "C:/Program Files/Python38" CACHE PATH "Path to your Python installation")

find_package (Python COMPONENTS Development)
set(Python_INCLUDE_DIR ${_Python_INCLUDE_DIR})

set(PYTHON_SCRIPT_FOLDER "${CMAKE_SOURCE_DIR}/src/python/")
add_definitions(-DPYTHON_SCRIPT_FOLDER="${PYTHON_SCRIPT_FOLDER}")

include_directories(${Python_INCLUDE_DIR})

include(sources.cmake)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${Python_LIBRARY_RELEASE})

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                           ${CMAKE_SOURCE_DIR}/embedded-python/build/ $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)

