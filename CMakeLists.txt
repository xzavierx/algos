# CMake 最低版本号要求
cmake_minimum_required(VERSION 2.11)
# cmake_policy(SET CMP0037 OLD)

project(algos)

add_subdirectory(merge_sort)
add_subdirectory(leetcode)
add_subdirectory(splay)
add_subdirectory(btree)
add_subdirectory(seg_tree)