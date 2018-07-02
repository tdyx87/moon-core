file(GLOB TEST_MOON_SOURCES "*.cpp")

ADD_EXECUTABLE(test_moon ${TEST_MOON_SOURCES})
target_link_libraries(test_moon gtest_main gtest moon pthread)

