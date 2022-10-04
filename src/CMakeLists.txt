find_package(reportportal-client-cpp CONFIG REQUIRED)
find_package(GTest MODULE REQUIRED)

add_library(reportportal-agent-googletest)
add_library(${PROJECT_NAME}::reportportal-agent-googletest ALIAS reportportal-agent-googletest)
target_sources(reportportal-agent-googletest
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/reportportal/gtest/event_listener.hpp>
    PRIVATE
        event_listener.cpp)

# This seems redundant since we declare the headers PUBLIC in sources but
# We need to call this to tell cmake what is the public headers so installing happens automatically.
target_public_headers(reportportal-agent-googletest
    ${CMAKE_CURRENT_SOURCE_DIR}/reportportal/gtest/event_listener.hpp)
target_include_directories(reportportal-agent-googletest
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(reportportal-agent-googletest
    PUBLIC
        reportportal-client-cpp::reportportal-client-cpp
        CONAN_PKG::gtest)
target_compile_features(reportportal-agent-googletest PUBLIC cxx_std_17)
# Needed on Windows platforms as WinDef.h has macros for min/max that interfere with std::min/max
target_compile_definitions(reportportal-agent-googletest
    PRIVATE
      NOMINMAX)
