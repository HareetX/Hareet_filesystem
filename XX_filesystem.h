#pragma once
#include <iostream>
#include <string>
#include "Disk.h"
using namespace std;

// �����ļ���غ궨��
#define FILESYSTEMNAME "XX_filesystem.sys" // ��������ļ�����
#define BLOCK_SIZE 4096 // block��СΪ 4096 B = 4 KB
#define BLOCK_NUM 10240 // block����Ϊ 10240
#define INODE_SIZE 128 // inode��СΪ 128 B
#define INODE_NUM 640 // inode����Ϊ 640

#define BLOCKS_PER_SUPERBLOCK 1 // 1��superblockռ1��block
#define BLOCKS_PER_IBITMAP 1 // 1��inodeλͼռ1��block
#define BLOCKS_PER_BBITMAP 3 // 1��blockλͼռ3��block ��3 * 4 KB = 12 KB > 10 KB
#define BLOCKS_PER_ILABEL 20 // 1��superblockռ1��block

// Inode��غ궨��
#define BLOCK_INDEX 8 // Inode��ֱ�ӿ���
#define FILE_MODE 0 // ��ͨ�ļ�����
#define DIR_MODE 1 // Ŀ¼�ļ�����

// �����ļ���س���
const int Sum_Size = BLOCK_SIZE * BLOCK_NUM + INODE_SIZE * INODE_NUM;
const int Super_Block_Address  = 0;
const int Inode_Bitmap_Address = Super_Block_Address  + BLOCK_SIZE * BLOCKS_PER_SUPERBLOCK;
const int Block_Bitmap_Address = Inode_Bitmap_Address + BLOCK_SIZE * BLOCKS_PER_IBITMAP;
const int Inode_Label_Address  = Block_Bitmap_Address + BLOCK_SIZE * BLOCKS_PER_BBITMAP;
const int Block_Address =        Inode_Label_Address  + BLOCK_SIZE * BLOCKS_PER_ILABEL;
const int Block_Num = BLOCK_NUM - BLOCKS_PER_SUPERBLOCK - BLOCKS_PER_IBITMAP - BLOCKS_PER_BBITMAP;

// �ļ�ϵͳ��ر���
bool isLogin = false; // �Ƿ��ѵ�¼
char buffer[Sum_Size] = { 0 }; // ��������ļ�����

FILE* fr; // �����ļ��Ķ�ָ��
FILE* fw; // �����ļ���дָ��

bool Format(); // ��ʽ����������ļ�
void Initial(); // ��ʼ����������ļ�
bool Install(); // ��װ��������ļ�
void Login(); // �û���¼

void cmd(string args);