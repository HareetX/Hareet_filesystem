#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <conio.h>
#include <stdio.h> 
#include <cstdio>
#include <sstream>
#include <windows.h>



using namespace std;

// 磁盘文件相关宏定义
#define FILESYSTEMNAME "XX_filesystem.sys" // 虚拟磁盘文件名称
#define BLOCK_SIZE 4096 // block大小为 4096 B = 4 KB
#define BLOCK_NUM 10240 // block数量为 10240
#define INODE_SIZE 128 // inode大小为 128 B
#define INODE_NUM 640 // inode数量为 640

#define BLOCKS_PER_SUPERBLOCK 1 // 1个superblock占1个block
#define BLOCKS_PER_IBITMAP 1 // 1个inode位图占1个block
#define BLOCKS_PER_BBITMAP 3 // 1个block位图占3个block ：3 * 4 KB = 12 KB > 10 KB
#define BLOCKS_PER_ILABEL 20 // 1个Inode区占20个block

// Inode相关宏定义
#define BLOCK_INDEX 8 // Inode的直接块数
#define FILE_MODE 0 // 普通文件类型
#define DIR_MODE 1 // 目录文件类型
#define OTHER_MODE 2 // 其他文件类型

// 磁盘文件相关常量
const int Sum_Size = BLOCK_SIZE * BLOCK_NUM;
const int Super_Block_Address  = 0;
const int Inode_Bitmap_Address = Super_Block_Address  + BLOCK_SIZE * BLOCKS_PER_SUPERBLOCK;
const int Block_Bitmap_Address = Inode_Bitmap_Address + BLOCK_SIZE * BLOCKS_PER_IBITMAP;
const int Inode_Label_Address  = Block_Bitmap_Address + BLOCK_SIZE * BLOCKS_PER_BBITMAP;
const int Block_Address =        Inode_Label_Address  + BLOCK_SIZE * BLOCKS_PER_ILABEL;
const int Block_Num = BLOCK_NUM - BLOCKS_PER_SUPERBLOCK - BLOCKS_PER_IBITMAP - BLOCKS_PER_BBITMAP - BLOCKS_PER_ILABEL;

// 目录相关宏定义
#define ROOT 0 // 根目录标志
#define DIR  1 // 一般目录标志

#include "Disk.h"
#include "Directory.h"
#include "FileSystem.h"


bool Open(FileSystem& fs); // 打开磁盘文件
bool Format(FileSystem& fs); // 格式化虚拟磁盘文件
void Install(FileSystem& fs); // 读入目录信息
void Close(FileSystem& fs); // 关闭磁盘文件