cmake_minimum_required(VERSION "3.12")

add_subdirectory("dependencies/SDL2" EXCLUDE_FROM_ALL)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


project(FlatEngine)

file(GLOB_RECURSE FLAT_ENGINE_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/FlatEngine/*.cpp)

add_library(FlatEngine ${FLAT_ENGINE_SOURCES})
target_include_directories(FlatEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/FlatEngine)

target_link_libraries(FlatEngine PUBLIC SDL2::SDL2main)
target_link_libraries(FlatEngine PUBLIC SDL2::SDL2)


project(FlatEngineTests)

enable_testing()

file(GLOB_RECURSE FLAT_ENGINE_TESTS_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/FlatEngineTests/*.cpp)

add_executable(FlatEngineTests ${FLAT_ENGINE_TESTS_SOURCES})
target_include_directories(FlatEngineTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/FlatEngineTests)

target_link_libraries(FlatEngineTests PUBLIC FlatEngine)
target_link_libraries(FlatEngineTests PUBLIC GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(FlatEngineTests)


project(LabyrinthShooterBase)

file(GLOB_RECURSE LAB_SHOOTER_BASE_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/LabyrinthShooterBase/*.cpp)

add_library(LabyrinthShooterBase ${LAB_SHOOTER_BASE_SOURCES})
target_include_directories(LabyrinthShooterBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/LabyrinthShooterBase)

target_link_libraries(LabyrinthShooterBase PUBLIC FlatEngine)
set_target_properties(LabyrinthShooterBase PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:FlatEngine")


project(Editor)

file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor/*.cpp)

add_executable(Editor ${EDITOR_SOURCES})
target_include_directories(Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor)

target_link_libraries(Editor PRIVATE FlatEngine)
target_link_libraries(Editor PRIVATE LabyrinthShooterBase)
set_target_properties(Editor PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:FlatEngine /WHOLEARCHIVE:LabyrinthShooterBase")


project(LabyrinthShooter)

file(GLOB_RECURSE LAB_SHOOTER_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/LabyrinthShooter/*.cpp)

add_executable(LabyrinthShooter ${LAB_SHOOTER_SOURCES})
target_include_directories(LabyrinthShooter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/LabyrinthShooter)

target_link_libraries(LabyrinthShooter PRIVATE LabyrinthShooterBase)
set_target_properties(LabyrinthShooter PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:FlatEngine /WHOLEARCHIVE:LabyrinthShooterBase")