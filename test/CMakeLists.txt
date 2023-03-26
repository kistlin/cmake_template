set(TESTNAME UnitTests)

set(SOURCES
	test_template.cpp
	)

add_executable(${TESTNAME} ${SOURCES})
target_include_directories(${TESTNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TESTNAME} PRIVATE TemplateLib)
target_link_libraries(${TESTNAME} PUBLIC GTest::gtest_main)
target_link_libraries(${TESTNAME} PUBLIC GTest::gtest)
target_link_libraries(${TESTNAME} PUBLIC GTest::gmock)

IF (MSVC)
ELSE()
	target_link_libraries(${TESTNAME} PRIVATE pthread)
ENDIF()

add_test(${TESTNAME} ${TESTNAME})
gtest_discover_tests(${TESTNAME})
