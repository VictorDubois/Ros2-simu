cmake_minimum_required(VERSION 3.5)
project(ezbot-v2-simulation)


find_package(ament_cmake REQUIRED)


install(
    DIRECTORY launch worlds models config
    DESTINATION share/${PROJECT_NAME}
)

install(
    DIRECTORY models/
    DESTINATION $ENV{HOME}/.gz/models/
)
ament_environment_hooks("${CMAKE_CURRENT_SOURCE_DIR}/hooks/${PROJECT_NAME}.dsv.in")


ament_package()
