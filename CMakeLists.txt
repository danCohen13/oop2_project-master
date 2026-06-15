cmake_minimum_required (VERSION 3.26)

project (oop1_project)
set (MY_AUTHORS "YOUR-NAME-HERE")

include (cmake/CompilerSettings.cmake)

add_executable (${CMAKE_PROJECT_NAME})

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address>)
if (NOT MSVC)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address>)
endif()

add_subdirectory (include)
add_subdirectory (src)


include (cmake/Zip.cmake)

set (SFML_LOCATION "C:/SFML/SFML-3.0.0")
set (SFML_DIR "${SFML_LOCATION}/lib/cmake/SFML")

find_package (SFML 3.0 COMPONENTS Graphics Main Audio REQUIRED)

target_link_libraries (${CMAKE_PROJECT_NAME} SFML::Graphics SFML::Audio SFML::Main)

include (cmake/SFML.cmake)

set(RESOURCE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(RESOURCE_BIN_DIR "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/resources")

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${RESOURCE_SRC_DIR}"
    "${RESOURCE_BIN_DIR}"
    COMMENT "Copying resources to build directory..."
)

if(MSVC)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()