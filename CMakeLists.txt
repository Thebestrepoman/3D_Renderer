cmake_minimum_required(VERSION 3.10)
project(3d_renderer)

find_package(Eigen3 3.3 REQUIRED)

add_executable(3d_renderer main.cpp utility/object.cpp utility/triangle.cpp utility/colour.cpp utility/vertex.cpp utility/object_parser.cpp utility/light.cpp utility/world.cpp renderer/camera.cpp renderer/plane.cpp)

target_link_libraries(3d_renderer PRIVATE Eigen3::Eigen)