cmake_minimum_required(VERSION 3.8)
project(hobot_s316_cam)

set(CMAKE_BUILD_TYPE RELEASE)
message("=> Build type: ${CMAKE_BUILD_TYPE}")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(OpenCV REQUIRED)

# include
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/hbre_1.0.14/include
)

# link
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/hbre_1.0.14/lib
)

# add executable
add_executable(hobot_s316_cam src/main.cpp)
target_include_directories(hobot_s316_cam PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(hobot_s316_cam PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
ament_target_dependencies(hobot_s316_cam
  rclcpp
  sensor_msgs
  OpenCV
)
set(HB_LIBS
-l:libvpf.so.1
-l:libcam.so.1
-l:libhbmem.so.1
-l:libcjson.so.1.7.15
-l:libalog.so.1.0.1
-l:libgdcbin.so.1.0.2
-l:libcnn_intf.so
-l:libcnn_intf.so.1
-l:libdnn.so
-l:libhbrt_bayes_aarch64.so
)
set(METOAK_LIBS -l:libMoGeneralSDK_static_d.a)
target_link_libraries(${PROJECT_NAME} ${METOAK_LIBS} ${HB_LIBS})

# install
install(TARGETS hobot_s316_cam DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})
install(DIRECTORY config DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
