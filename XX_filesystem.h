#pragma once
#include <iostream>
using namespace std;

// 宏定义
#define FILESYSTEMNAME "XX_filesystem.sys" // 虚拟磁盘文件名称

const int Sum_Size = 0;

char buffer[10 * 1024 * 1024] = { 0 };

FILE* fr; // 虚拟文件的读指针
FILE* fw; // 虚拟文件的写指针

bool Format(); // 格式化虚拟磁盘文件
void Initial(); // 初始化虚拟磁盘文件