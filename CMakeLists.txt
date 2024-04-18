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


project(FlintEngine)

file(GLOB_RECURSE FLINT_ENGINE_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/FlintEngine/*.cpp)

add_library(FlintEngine ${FLINT_ENGINE_SOURCES})
target_include_directories(FlintEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/FlintEngine)

target_link_libraries(FlintEngine PUBLIC SDL2::SDL2main)
target_link_libraries(FlintEngine PUBLIC SDL2::SDL2)


project(FlintEngineTests)

enable_testing()

file(GLOB_RECURSE FLINT_ENGINE_TESTS_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/FlintEngineTests/*.cpp)

add_executable(FlintEngineTests ${FLINT_ENGINE_TESTS_SOURCES})
target_include_directories(FlintEngineTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/FlintEngineTests)

target_link_libraries(FlintEngineTests PUBLIC FlintEngine)
target_link_libraries(FlintEngineTests PUBLIC GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(FlintEngineTests)


project(LabyrinthShooterBase)

file(GLOB_RECURSE LAB_SHOOTER_BASE_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/LabyrinthShooterBase/*.cpp)

add_library(LabyrinthShooterBase ${LAB_SHOOTER_BASE_SOURCES})
target_include_directories(LabyrinthShooterBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/LabyrinthShooterBase)

target_link_libraries(LabyrinthShooterBase PUBLIC FlintEngine)
set_target_properties(LabyrinthShooterBase PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:FlintEngine")


project(Editor)

file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor/*.cpp)

add_executable(Editor ${EDITOR_SOURCES})
target_include_directories(Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Editor)

target_link_libraries(Editor PRIVATE FlintEngine)
target_link_libraries(Editor PRIVATE LabyrinthShooterBase)
set_target_properties(Editor PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:FlintEngine /WHOLEARCHIVE:LabyrinthShooterBase")


project(LabyrinthShooter)

file(GLOB_RECURSE LAB_SHOOTER_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/LabyrinthShooter/*.cpp)

add_executable(LabyrinthShooter ${LAB_SHOOTER_SOURCES})
target_include_directories(LabyrinthShooter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/LabyrinthShooter)

target_link_libraries(LabyrinthShooter PRIVATE LabyrinthShooterBase)
set_target_properties(LabyrinthShooter PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:FlintEngine /WHOLEARCHIVE:LabyrinthShooterBase")