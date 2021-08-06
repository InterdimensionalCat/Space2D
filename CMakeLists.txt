# CMakeList.txt : CMake project for Space2D, include source and define
# project specific logic here.
#

macro(set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

set_option(GENERATE_TESTS    TRUE    BOOL   "If true, generates the project unit tests"  )
set_option(GENERATE_DRIVER   TRUE    BOOL   "If true, generates the project unit tests"  )
set_option(ENABLE_SFML       TRUE    BOOL   "If true, enables SFML specific functions "  )

# set minimum version required for CMake
cmake_minimum_required (VERSION 3.16)

set(PROJECT_NAME "Space2D")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(DRIVER_SRC_DIR "${PROJECT_SOURCE_DIR}/src/driversrc")
set(DRIVER_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src/driverinclude")

set(TEST_SRC_DIR "${PROJECT_SOURCE_DIR}/src/testsrc")
set(TEST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/src/testinclude")

set(LIBRARY_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

include_directories(${LIBRARY_INCLUDE_DIR})

##if SFML IS DEFINED
if(ENABLE_SFML)
    find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
    include_directories(${SFML_INCLUDE_DIR})
    message(STATUS "SFML is enabled, _SFML_ENABLED defined")
    add_compile_definitions(_SFML_ENABLED)

endif()
#END SFML IF

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Building in debug mode, debug_mode defined")
    add_compile_definitions(debug_mode)
endif()

if( NOT ENABLE_SFML AND GENERATE_DRIVER)
    message(FATAL_ERROR "SFML must be enabled in order to generate the driver")
endif()

#file(GLOB LIB_H "${LIBRARY_INCLUDE_DIR}/*.h")
add_library(${PROJECT_NAME} INTERFACE) 
#target_include_directories (${PROJECT_NAME} PUBLIC ${LIBRARY_INCLUDE_DIR})


if(GENERATE_DRIVER)
set(SFML_STATIC_LIBRARIES FALSE)
    file(GLOB DRIVER_SRC "${DRIVER_SRC_DIR}/*.cpp")
    include_directories(${DRIVER_INCLUDE_DIR})
    add_executable(${PROJECT_NAME}_DRIVER ${DRIVER_SRC})
    target_link_libraries(${PROJECT_NAME}_DRIVER PUBLIC ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME}_DRIVER PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
endif()

if(GENERATE_TESTS)
    file(GLOB TEST_SRC "${TEST_SRC_DIR}/*.cpp")
    include_directories(${TEST_INCLUDE_DIR})
    add_executable(${PROJECT_NAME}_TEST ${TEST_SRC})
    target_link_libraries(${PROJECT_NAME}_TEST PUBLIC ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME}_TEST PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
    enable_testing()

    include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
    target_link_libraries(${PROJECT_NAME}_TEST PRIVATE gtest_main)

    add_test(VecTest ${PROJECT_NAME}_TEST VecTest)
    add_test(PointTest ${PROJECT_NAME}_TEST PointTest)
    add_test(DimTest ${PROJECT_NAME}_TEST DimTest)
    add_test(NormVecTest ${PROJECT_NAME}_TEST NormVecTest)
    add_test(RectTest ${PROJECT_NAME}_TEST RectTest)

    #add_test(Test1 ${PROJECT_NAME}_TEST Test1)
    #add_test(Test2 ${PROJECT_NAME}_TEST Test2)

    #include(CTest)
    #include(Catch)
    #catch_discover_tests(Test1)
    #catch_discover_tests(Test2)
    #set(CTEST_OUTPUT_ON_FAILURE TRUE)
endif()