if (CONAN_EXPORTED) # in conan local cache
    # standard conan installation, deps will be defined in conanfile.py
    # and not necessary to call conan again, conan is alredy running
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
else() # in user space
    # Download automatically, you can also just copy the conan.cmake file
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
      message(
        STATUS
          "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
      file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
           "${CMAKE_BINARY_DIR}/conan.cmake")
    endif()

    include(${CMAKE_BINARY_DIR}/conan.cmake)
    # Add other remotes
    conan_add_remote(
        NAME maxxboehme_conan_packages
        URL https://mboehme.jfrog.io/artifactory/api/conan/mboehme-conan)

    to_python_bool(${ENABLE_TESTING} ENABLE_TESTING_PYTHON)
    to_python_bool(${ENABLE_BENCHMARKING} ENABLE_BENCHMARKING_PYTHON)
    conan_cmake_run(
        CONANFILE conan/conanfile.py
        OPTIONS
            reportportal-agent-googletest:enable_testing=${ENABLE_TESTING_PYTHON}
            reportportal-agent-googletest:enable_benchmarking=${ENABLE_BENCHMARKING_PYTHON}
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing)
endif()

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

