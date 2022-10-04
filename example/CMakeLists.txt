find_package(GTest MODULE REQUIRED)

add_executable(reportportal-googletest-example)
target_sources(reportportal-googletest-example
    PRIVATE
        gtest_main.cpp
        example_tests.cpp)

target_include_directories(reportportal-googletest-example
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(reportportal-googletest-example
    PRIVATE
        reportportal-agent-googletest
        CONAN_PKG::gtest)
target_compile_features(reportportal-googletest-example PUBLIC cxx_std_17)
# Needed on Windows platforms as WinDef.h has macros for min/max that interfere with std::min/max
target_compile_definitions(reportportal-googletest-example
    PRIVATE
      NOMINMAX)
