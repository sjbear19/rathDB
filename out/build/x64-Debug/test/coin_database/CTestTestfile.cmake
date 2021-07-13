# CMake generated Testfile for 
# Source directory: C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/coin_database
# Build directory: C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/test/coin_database
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(CoinLocator.Serialize "C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/test/coin_database/test_coin_database.exe" "--gtest_filter=CoinLocator.Serialize")
set_tests_properties(CoinLocator.Serialize PROPERTIES  _BACKTRACE_TRIPLES "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.20/Modules/GoogleTest.cmake;390;add_test;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/coin_database/CMakeLists.txt;13;gtest_add_tests;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/coin_database/CMakeLists.txt;0;")
add_test(CoinRecord.Serialize "C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/test/coin_database/test_coin_database.exe" "--gtest_filter=CoinRecord.Serialize")
set_tests_properties(CoinRecord.Serialize PROPERTIES  _BACKTRACE_TRIPLES "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.20/Modules/GoogleTest.cmake;390;add_test;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/coin_database/CMakeLists.txt;13;gtest_add_tests;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/coin_database/CMakeLists.txt;0;")
