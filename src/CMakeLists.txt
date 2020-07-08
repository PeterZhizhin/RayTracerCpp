# automatically enable catch2 to generate ctest targets
if (CONAN_CATCH2_ROOT_DEBUG)
  include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else ()
  include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif ()

add_subdirectory(tqdm)

add_library(image image.cpp image.h)
target_link_libraries(image CONAN_PKG::fmt CONAN_PKG::stb)

add_library(vec3 vec3.h)
set_target_properties(vec3 PROPERTIES LINKER_LANGUAGE CXX)
add_executable(vec3_test vec3_test.cpp)
target_link_libraries(vec3_test PRIVATE project_warnings project_options vec3 CONAN_PKG::catch2)

add_library(ray ray.h)
set_target_properties(ray PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ray vec3)
add_executable(ray_test ray_test.cpp)
target_link_libraries(ray_test PRIVATE project_warnings project_options ray CONAN_PKG::catch2)

add_library(material material.h)
set_target_properties(material PROPERTIES LINKER_LANGUAGE CXX)

add_library(hittable hittable.h)
set_target_properties(hittable PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(material hittable vec3 ray)
target_link_libraries(hittable vec3 ray material)

add_executable(hittable_test hittable_test.cpp)
target_link_libraries(hittable_test PRIVATE project_warnings project_options hittable CONAN_PKG::catch2)

add_library(absorbing absorbing.cpp absorbing.h)
target_link_libraries(absorbing material)

add_library(sphere sphere.h sphere.cpp)
target_link_libraries(sphere vec3 ray hittable material absorbing)
add_executable(sphere_test sphere_test.cpp)
target_link_libraries(sphere_test PRIVATE project_warnings project_options
        sphere
        CONAN_PKG::catch2 CONAN_PKG::trompeloeil)

add_library(hittable_list hittable_list.h hittable_list.cpp)
target_link_libraries(hittable_list ray)
add_executable(hittable_list_test hittable_list_test.cpp)
target_link_libraries(hittable_list_test PRIVATE
        project_warnings project_options
        hittable_list hittable ray vec3
        CONAN_PKG::catch2 CONAN_PKG::trompeloeil)

add_library(random random.cpp random.h)
target_link_libraries(random vec3)

add_library(camera camera.cpp camera.h)
add_executable(camera_test camera_test.cpp)
target_link_libraries(camera_test PRIVATE project_warnings project_options camera CONAN_PKG::catch2)

add_library(lambertian lambertian.cpp lambertian.h)
target_link_libraries(lambertian material random)

add_executable(RayTracerCpp main.cpp)
target_link_libraries(
        RayTracerCpp
        PRIVATE project_options
        camera
        hittable_list
        image
        lambertian
        random
        ray
        sphere
        tqdm
        vec3
        project_warnings
        CONAN_PKG::abseil
        CONAN_PKG::fmt
        CONAN_PKG::spdlog
)
