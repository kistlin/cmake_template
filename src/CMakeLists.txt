#add_subdirectory(statemachine)

project (cppscratch)
set(PROJECT_NAME_LIB ${PROJECT_NAME}lib)

set(SOURCES
        algorithms.cpp
        async.cpp
        MyClass.cpp
        smart_pointer.cpp
        template.cpp
        )

add_library(${PROJECT_NAME_LIB} ${SOURCES})
target_include_directories(${PROJECT_NAME_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME}
        main.cpp
        )
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME_LIB})

set(SOURCES_STATEMACHINE
        statemachine/c/main.c
)

add_executable(exe_statemachine ${SOURCES_STATEMACHINE})
# set_property(TARGET exe_statemachine PROPERTY CXX_STANDARD 17)
# set_property(TARGET exe_statemachine PROPERTY C_STANDARD 11)
target_include_directories(exe_statemachine PRIVATE statemachine/c)
