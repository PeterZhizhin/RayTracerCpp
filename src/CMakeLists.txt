add_executable(RayTracerCpp main.cpp)
conan_target_link_libraries(
  RayTracerCpp
  PRIVATE project_options
          project_warnings
          CONAN_PKG::abseil
          CONAN_PKG::fmt
          CONAN_PKG::spdlog
)
