set(SDL_TEST OFF)
set(SDLIMAGE_VENDORED OFF)

add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(entt EXCLUDE_FROM_ALL)

add_library(Dependencies INTERFACE)
target_link_libraries(Dependencies INTERFACE SDL3::SDL3 SDL3_image::SDL3_image EnTT::EnTT)
