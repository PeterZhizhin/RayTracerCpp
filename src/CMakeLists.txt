# automatically enable catch2 to generate ctest targets
if (CONAN_CATCH2_ROOT_DEBUG)
  include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else ()
  include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif ()

add_executable(vec3_test vec3_test.cpp vec3.h)
target_link_libraries(vec3_test PRIVATE project_warnings project_options CONAN_PKG::catch2)

add_executable(ray_test ray_test.cpp vec3.h ray.h)
target_link_libraries(ray_test PRIVATE project_warnings project_options CONAN_PKG::catch2)

add_executable(RayTracerCpp main.cpp vec3.h image.h image.cpp ray.h)
conan_target_link_libraries(
        RayTracerCpp
        PRIVATE project_options
        project_warnings
        CONAN_PKG::abseil
        CONAN_PKG::fmt
        CONAN_PKG::spdlog
        CONAN_PNG::stb
)
