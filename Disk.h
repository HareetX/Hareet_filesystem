#pragma once
#include "XX_filesystem.h"

// 数据块Block
class Block
{
private:
	int No; // 数据块块号

public:
	Block(int No);

	// 打印数据块块号
	virtual void printInfo();

	//数据块接口
	int getNO(); // 获取块号
	virtual void block_read(FILE *fr, char buf[]); // 从磁盘文件读取该数据块数据存入buf缓存
	virtual void block_write(FILE* fw, char buf[]); // 把buf缓存数据存入磁盘文件对应的数据块
};

class Superblock : Block
{
private:
	const int s_Inode_Num = INODE_NUM; // 总Inode数
	int free_Inode_Num; // 空闲Inode数
	const int s_Block_Num = Block_Num; // 总Block数
	int free_Block_Num; // 空闲Block数

public:
	Superblock(int free_INum, int free_BNum);
	Superblock();

	// 打印Superblock信息
	virtual void printInfo();
};

class Block_Bitmap : Block
{
private:
	bool b_isUsed[Block_Num]; // Block占用情况
public:
	Block_Bitmap(bool isUsed[Block_Num]);
	Block_Bitmap();

	// 打印Block占用情况
	virtual void printInfo();
};

class Inode_Bitmap : Block
{
private:
	bool i_isUsed[INODE_NUM]; // Inode占用情况
public:
	Inode_Bitmap(bool isUsed[INODE_NUM]);
	Inode_Bitmap();

	// 打印Inode占用情况
	virtual void printInfo();
};

class Inode
{
private:
	int i_No; // Inode编号
	int f_mode; // 文件类型
	int f_size; // 文件大小
	int c_time; // 创建时间
	int block_index[BLOCK_INDEX]; // Inode指向的块号序列

public:
	Inode(int No, int mode, int size, int time, int index[BLOCK_INDEX]);
	Inode(int No, int mode, int size, int time);

	// 打印该Inode信息
	void printInfo();
};