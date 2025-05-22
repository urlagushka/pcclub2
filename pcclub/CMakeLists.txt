set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(pcclub
  club.cpp
  time.cpp
  events.cpp
  utils.cpp
)

target_include_directories(pcclub PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
