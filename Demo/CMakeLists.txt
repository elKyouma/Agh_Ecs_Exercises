add_executable(App main.cpp App.cpp)
target_link_libraries(App PRIVATE Dependencies)
target_compile_features(App PRIVATE cxx_std_20)

include_directories(
  ${CMAKE_SOURCE_DIR}/Dependency/entt/src
)
add_definitions( -DART_PATH=\"${CMAKE_CURRENT_LIST_DIR}/Art/\" )
