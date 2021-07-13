# CMake generated Testfile for 
# Source directory: C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/db
# Build directory: C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/test/db
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Database.GetPutBasic "C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/test/db/test_db.exe" "--gtest_filter=Database.GetPutBasic")
set_tests_properties(Database.GetPutBasic PROPERTIES  _BACKTRACE_TRIPLES "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.20/Modules/GoogleTest.cmake;390;add_test;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/db/CMakeLists.txt;13;gtest_add_tests;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/db/CMakeLists.txt;0;")
add_test(Database.DeleteBasic "C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/test/db/test_db.exe" "--gtest_filter=Database.DeleteBasic")
set_tests_properties(Database.DeleteBasic PROPERTIES  _BACKTRACE_TRIPLES "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.20/Modules/GoogleTest.cmake;390;add_test;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/db/CMakeLists.txt;13;gtest_add_tests;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/db/CMakeLists.txt;0;")
add_test(Database.BatchGetPutBasic "C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/test/db/test_db.exe" "--gtest_filter=Database.BatchGetPutBasic")
set_tests_properties(Database.BatchGetPutBasic PROPERTIES  _BACKTRACE_TRIPLES "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.20/Modules/GoogleTest.cmake;390;add_test;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/db/CMakeLists.txt;13;gtest_add_tests;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/db/CMakeLists.txt;0;")
add_test(Database.BatchGetPutWithDelete "C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/test/db/test_db.exe" "--gtest_filter=Database.BatchGetPutWithDelete")
set_tests_properties(Database.BatchGetPutWithDelete PROPERTIES  _BACKTRACE_TRIPLES "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.20/Modules/GoogleTest.cmake;390;add_test;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/db/CMakeLists.txt;13;gtest_add_tests;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/db/CMakeLists.txt;0;")
