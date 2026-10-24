cmake_minimum_required(VERSION 3.24)
project(HolaIphone C)

include(FetchContent)

# SDL3 se descarga y se compila como libreria estatica dentro del proyecto.
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.2.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3)

add_executable(holaiphone MACOSX_BUNDLE src/main.c)
target_link_libraries(holaiphone PRIVATE SDL3::SDL3)

set_target_properties(holaiphone PROPERTIES
  MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist.in"
  MACOSX_BUNDLE_GUI_IDENTIFIER "com.ejemplo.holaiphone"
  MACOSX_BUNDLE_BUNDLE_NAME "HolaIphone"
  MACOSX_BUNDLE_BUNDLE_VERSION "1"
  MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
  XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO"
  XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
)
