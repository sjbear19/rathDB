cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(googletest-download NONE)

include(ExternalProject)
ExternalProject_Add(googletest
  GIT_REPOSITORY    https://github.com/google/googletest.git
  GIT_TAG           master
  SOURCE_DIR        "C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/googletest-src"
  BINARY_DIR        "C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/googletest-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)
