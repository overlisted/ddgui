cmake_minimum_required(VERSION 3.16)

project(ddgui)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

set(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/resource)
set(RESOURCES_CONFIG ${CMAKE_SOURCE_DIR}/ddgui.gresource.xml)
set(RESOURCES_COMPILATION ${CMAKE_BINARY_DIR}/resources.c)

add_custom_target(
  resources
  COMMAND glib-compile-resources ${RESOURCES_CONFIG}
  --sourcedir=${RESOURCES_DIR}
  --target=${RESOURCES_COMPILATION}
  --generate-source
  BYPRODUCTS ${RESOURCES_COMPILATION}
  VERBATIM
)

add_executable(ddgui ${RESOURCES_COMPILATION} main.cpp)
add_dependencies(ddgui resources)

target_include_directories(ddgui PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_libraries(ddgui ${GTKMM_LIBRARIES})