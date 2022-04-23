#pragma once
#include <iostream>
#include <string>
#include "Disk.h"
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
#define BLOCKS_PER_ILABEL 20 // 1个superblock占1个block

// Inode相关宏定义
#define BLOCK_INDEX 8 // Inode的直接块数
#define FILE_MODE 0 // 普通文件类型
#define DIR_MODE 1 // 目录文件类型

// 磁盘文件相关常量
const int Sum_Size = BLOCK_SIZE * BLOCK_NUM + INODE_SIZE * INODE_NUM;
const int Super_Block_Address  = 0;
const int Inode_Bitmap_Address = Super_Block_Address  + BLOCK_SIZE * BLOCKS_PER_SUPERBLOCK;
const int Block_Bitmap_Address = Inode_Bitmap_Address + BLOCK_SIZE * BLOCKS_PER_IBITMAP;
const int Inode_Label_Address  = Block_Bitmap_Address + BLOCK_SIZE * BLOCKS_PER_BBITMAP;
const int Block_Address =        Inode_Label_Address  + BLOCK_SIZE * BLOCKS_PER_ILABEL;
const int Block_Num = BLOCK_NUM - BLOCKS_PER_SUPERBLOCK - BLOCKS_PER_IBITMAP - BLOCKS_PER_BBITMAP;

// 文件系统相关变量
bool isLogin = false; // 是否已登录
char buffer[Sum_Size] = { 0 }; // 虚拟磁盘文件缓存

FILE* fr; // 虚拟文件的读指针
FILE* fw; // 虚拟文件的写指针

bool Format(); // 格式化虚拟磁盘文件
void Initial(); // 初始化虚拟磁盘文件
bool Install(); // 安装虚拟磁盘文件
void Login(); // 用户登录

void cmd(string args);