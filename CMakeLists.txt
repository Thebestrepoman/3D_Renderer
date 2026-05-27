cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(3d_renderer)

find_package(Eigen3 3.3 REQUIRED)
find_package(SFML REQUIRED system window graphics)

add_executable(3d_renderer main.cpp utility/object.cpp utility/triangle.cpp utility/colour.cpp utility/vertex.cpp utility/object_parser.cpp utility/light.cpp utility/world.cpp renderer/camera.cpp renderer/plane.cpp renderer/renderer.cpp application/application.cpp application/except.cpp renderer/view.cpp renderer/runtime.cpp application/eventprocessor.cpp renderer/screen.cpp renderer/sfmlcolour.cpp)

target_link_libraries(3d_renderer Eigen3::Eigen sfml-system sfml-window sfml-graphics)
