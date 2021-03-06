#pragma once
#include "Directory.h"

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
	virtual void block_read(FILE* fpr) = 0; // 把块内数据从磁盘文件读入程序
	virtual void block_write(FILE* fpw) = 0; // 把块内数据从程序写入磁盘文件
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

	// 打印数据块信息（无需任何操作）
	virtual void printInfo();

	// 数据块接口
	char* buffer_return(); // 返回指向数据块数据的指针

	virtual void block_read(FILE* fpr); // 读入数据块
	virtual void block_write(FILE* fpw); // 写入磁盘文件

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
	//Superblock(int free_INum, int free_BNum);
	Superblock();

	// 格式化Superblock
	virtual void format();

	// 打印Superblock信息
	virtual void printInfo();

	// Superblock接口
	virtual void block_read(FILE* fpr); // 读入Superblock
	virtual void block_write(FILE* fpw); // 写入磁盘文件

	void use_renew(); // 创建文件时Superblock更新
	void use_renew(int blocks_num); // 写入文件占用超出原有块时，根据传入的块号blocks_num更新Superblock的块占用信息
	void free_renew(int b_cout); // 删除文件时，根据传入的需要删除block的数量，更新Superblock
};

class Block_Bitmap :
	public Block
{
private:
	bool b_isUsed[Block_Num]; // Block占用情况
public:
	Block_Bitmap();

	// 格式化Block位图
	virtual void format();

	// 打印Block占用情况
	virtual void printInfo();

	// Block_Bitmap接口
	virtual void block_read(FILE* fpr); // 读入Block_Bitmap
	virtual void block_write(FILE* fpw); // 写入磁盘文件

	int balloc(); // 申请空闲的block，返回首个空闲block的索引

	void use_renew(int b_index); // 需要占用空闲block时，根据传入的块号，更新块位图
	void free_renew(int b_index); // 需要释放空闲block时，根据传入的块号，更新块位图
};

class Inode_Bitmap :
	public Block
{
private:
	bool i_isUsed[INODE_NUM]; // Inode占用情况
public:
	Inode_Bitmap();

	// 格式化Inode位图
	virtual void format();

	// 打印Inode占用情况
	virtual void printInfo();

	// Inode_Bitmap接口
	virtual void block_read(FILE* fpr); // 读入Inode_Bitmap
	virtual void block_write(FILE* fpw); // 写入磁盘文件

	int ialloc(); // 申请空闲的inode，返回首个空闲inode的索引序号

	void use_renew(int i_index); // 需要占用空闲Inode时，根据传入的inode索引，更新Inode位图
	void free_renew(int i_index); // 需要释放空闲Inode时，根据传入的inode索引，更新Inode位图
};

class Inode
{
private:
	int i_No; // Inode编号
	int f_mode; // 文件类型
	int f_size; // 文件大小
	//int c_time; // 创建时间
	int block_index[BLOCK_INDEX]; // Inode指向的块号序列

public:
	Inode();

	// 格式化Inode
	void format();

	// 打印该Inode信息
	void printInfo();

	// Inode接口
	int getI_No(); // 获得Inode编号
	void setI_No(int No); // 设置Inode编号

	int getF_Mode(); // 获得文件类型
	void setF_Mode(int mode); // 设置文件类型

	int getF_Size(); // 获得文件大小
	void setF_Size(int size); // 设置文件大小

	bool addBlock(int index); // 增加可用块序号

	char* getFile(char* buffer); // 传入磁盘数据块缓存，返回该Inode指向的文件数据缓存

	int getIndex(int No); // 返回Inode指向的块号序列中的第No号块的块号
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

	// Inode_Label接口
	virtual void block_read(FILE* fpr); // 读入Inode_Label
	virtual void block_write(FILE* fpw); // 写入磁盘文件

	Inode* getInode(int No); // 返回第No号Inode

	// 创建文件时，根据传入的空闲inode索引、空闲块块号、文件类型、文件大小、当前目录的inode索引、当前目录的文件数据大小，更新Inode表，包括更新文件的inode和文件所在目录文件的inode
	void use_renew(int i_index, int b_index, int mode, int f_size, int cur_dir_i_index, int cur_dir_size); 
	// 删除文件时，根据传入的空闲inode索引、当前目录的inode索引、当前目录的文件数据大小，更新Inode表，包括更新文件的inode和文件所在目录文件的inode
	void free_renew(int i_index, int cur_dir_i_index, int cur_dir_size); 
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

	void format(Block& b); // Disk单元内各块区格式化
	void disk_format(); // Disk单元格式化

	/*
	* Disk单元接口
	*/ 

	// 将磁盘文件读入Disk单元
	void block_read(Block& b, FILE* fpr); // Disk单元内各块区数据读入
	void disk_read(FILE* fpr); // Disk单元读入
		
	// 将Disk单元存入磁盘文件
	void block_write(Block& b, FILE* fpw); // Disk单元内各块区数据写出
	void disk_write(FILE* fpw); // // Disk单元写出

	// 返回首个空闲block的序号
	int d_balloc();
	// 返回首个空闲Inode的序号
	int d_ialloc();

	// 显示虚拟磁盘数据块占用情况
	void printBlocks();
	// 显示虚拟磁盘Inode占用情况
	void printInodes();
	// 显示虚拟磁盘超级块占用情况
	void printSuperblock();

	// 根据传入的inode索引，把对应的文件的文件内容从数据块读出
	char* file_read(int i_index);
	// 根据传入的inode索引，返回对应的文件的文件大小
	int file_size(int i_index);

	// 根据传入的inode索引和文件缓存，把文件缓存写入数据块对应的位置
	bool file_write(int i_index, const char* buf);

	// Disk与目录的接口
	Dentry dentry_read(int dentry_address, char* buf); // 根据传入的目录项地址和数据块的数据缓存，从数据缓存读取并返回一个相应的目录项
	void dentry_write(int dentry_address, Dentry dentry); // （起始位置是数据块数据缓存的首位）把目录项根据传入的目录项地址，写入数据块数据缓存的对应位置
	void dentry_write(int dentry_address, char* buf, Dentry dentry); // （起始位置是传入的buf的首位）把目录项根据传入的目录项地址，写入数据块数据缓存的对应位置

	Directory dir_read(int i_No); // 根据传入的inode索引，从数据块数据缓存buffer中读取，并返回目录
	void dir_write(int i_No, Directory dir); // 根据传入的inode索引，把目录写入数据块数据缓存buffer

	// 占用空间时，根据传入的空闲块块号、空闲inode索引、文件类型、文件大小、当前目录的inode索引、当前目录的文件数据大小，更新Disk
	void use_renew(int b_index, int i_index, int mode, int f_size, int cur_dir_i_index, int cur_dir_size);
	// 释放空间时，根据传入的空闲inode索引、当前目录的inode索引、当前目录的文件数据大小，更新Disk
	void free_renew(int i_index, int cur_dir_i_index, int cur_dir_size);
};