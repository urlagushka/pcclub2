add_executable(pcclub_tests
  club_test.cpp
)

target_link_libraries(pcclub_tests
    PRIVATE
    pcclub
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(pcclub_tests)