cmake_minimum_required(VERSION 3.8)
project(rviz_nmpc_plugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_srvs REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rviz_common REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

add_library(${PROJECT_NAME} SHARED
    src/rmf_ui.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    include
)

ament_target_dependencies(${PROJECT_NAME}
    rclcpp
    std_srvs
    pluginlib
    rviz_common
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
)

# Export the plugin description to pluginlib
pluginlib_export_plugin_description_file(rviz_common plugin_description.xml)

install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES plugin_description.xml
    DESTINATION share/${PROJECT_NAME}
)

ament_package()
