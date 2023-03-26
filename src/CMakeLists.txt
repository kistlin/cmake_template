SET(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME Template)
set(PROJECT_NAME_LIB ${PROJECT_NAME}Lib)
project (${PROJECT_NAME})

set(SOURCES
        template.cpp
        )

add_library(${PROJECT_NAME_LIB} ${SOURCES})
target_include_directories(${PROJECT_NAME_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME}
        main.cpp
        )

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME_LIB})
