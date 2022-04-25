#pragma once
#include "XX_filesystem.h"

// Block
class Block
{
private:
	int No; // 块号

public:
	Block(int No);

	// 格式化块
	virtual void format() = 0;

	// 打印块信息
	virtual void printInfo() = 0;

	// 块接口
	int getNO(); // 获取块号
};

// 数据块
class DataBlock :
	public Block
{
private:
	// 虚拟磁盘文件缓存,用于写回在写文件方式打开时被清空的文件
	char* buffer;
public:
	DataBlock();
	~DataBlock();
	
	// 格式化数据块
	virtual void format();

	// 打印数据块信息
	virtual void printInfo(); // TODO

	// 数据块接口
	char* buffer_return(); // 返回指向数据块数据的指针
	void buffer_write(char buf[Block_Num * BLOCK_SIZE]); // 根据传入的数组改写数据块的数据
};

class Superblock :
	public Block
{
private:
	const int s_Inode_Num = INODE_NUM; // 总Inode数
	int free_Inode_Num; // 空闲Inode数
	const int s_Block_Num = Block_Num; // 总Block数
	int free_Block_Num; // 空闲Block数

public:
	Superblock(int free_INum, int free_BNum);
	Superblock();

	// 格式化Superblock
	virtual void format();

	// 打印Superblock信息
	virtual void printInfo();
};

class Block_Bitmap :
	public Block
{
private:
	bool b_isUsed[Block_Num]; // Block占用情况
public:
	Block_Bitmap(bool isUsed[Block_Num]);
	Block_Bitmap();

	// 格式化Block位图
	virtual void format();

	// 打印Block占用情况
	virtual void printInfo();

};

class Inode_Bitmap :
	public Block
{
private:
	bool i_isUsed[INODE_NUM]; // Inode占用情况
public:
	Inode_Bitmap(bool isUsed[INODE_NUM]);
	Inode_Bitmap();

	// 格式化Inode位图
	virtual void format();

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
	Inode(int No, int mode, int size, int time_c, int index[BLOCK_INDEX]);
	Inode(int No);
	Inode();

	// 格式化Inode
	void format();

	// 打印该Inode信息
	void printInfo();

	// Inode接口
	int getI_No(); // 获得Inode编号
	void setI_No(int No); // 设置Inode编号
};

// Inode表
class Inode_Label :
	public Block
{
private:
	Inode inode[INODE_NUM];
public:
	Inode_Label();

	// 格式化Inode表
	virtual void format();

	// 打印Inode表信息
	virtual void printInfo(); /*TODO*/
};

// Disk单元
class Disk
{
private:
	Superblock spb; // Disk单元中的Superblock
	Block_Bitmap b_bmap; // Disk单元中的Block位图
	Inode_Bitmap i_bmap; // Disk单元中的Inode位图
	Inode_Label i_label; // Disk单元中的Inode表
	DataBlock d_block; // 数据块缓存

public:

	void format(); // Disk单元格式化

	/*
	* Disk单元接口
	*/ 

	// 将磁盘文件读入Disk单元
	
	void spb_read(FILE* fr);// Superblock读入
	void b_bmap_read(FILE *fr);// Block位图读入
	void i_bmap_read(FILE* fr);// Inode位图读入
	void inode_read(FILE* fr);// Inode表读入
	void d_block_read(FILE* fr);// 数据块缓存读入
	
	
	// 将Disk单元存入磁盘文件
	
	void spb_write(FILE* fw);// Superblock存入
	void b_bmap_write(FILE* fw);// Block位图存入
	void i_bmap_write(FILE* fw);// Inode位图存入
	void inode_write(FILE* fw);// Inode表存入
	void d_block_write(FILE* fw);// 数据块缓存存入
};