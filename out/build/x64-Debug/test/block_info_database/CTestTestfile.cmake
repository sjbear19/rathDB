# CMake generated Testfile for 
# Source directory: C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/block_info_database
# Build directory: C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/test/block_info_database
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(BlockRecord.Serializaiton "C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/test/block_info_database/test_block_info_database.exe" "--gtest_filter=BlockRecord.Serializaiton")
set_tests_properties(BlockRecord.Serializaiton PROPERTIES  _BACKTRACE_TRIPLES "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.20/Modules/GoogleTest.cmake;390;add_test;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/block_info_database/CMakeLists.txt;13;gtest_add_tests;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/block_info_database/CMakeLists.txt;0;")
add_test(UndoBlock.Serializaiton "C:/Users/sambe/OneDrive/Documents/rathDB_stencil/out/build/x64-Debug/test/block_info_database/test_block_info_database.exe" "--gtest_filter=UndoBlock.Serializaiton")
set_tests_properties(UndoBlock.Serializaiton PROPERTIES  _BACKTRACE_TRIPLES "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/share/cmake-3.20/Modules/GoogleTest.cmake;390;add_test;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/block_info_database/CMakeLists.txt;13;gtest_add_tests;C:/Users/sambe/OneDrive/Documents/rathDB_stencil/test/block_info_database/CMakeLists.txt;0;")
