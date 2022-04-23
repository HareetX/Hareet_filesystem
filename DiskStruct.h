#pragma once
#include<time.h>
#include"XX_filesystem.h"

// 声明结构体
// 声明超级块结构体
struct SuperBlock {
	unsigned int s_SUPERBLOCK_SIZE;			//超级块的大小

	unsigned int s_INODE_NUMBER;			//inode节点数量
	unsigned int s_INODE_FREE_NUMBER;		//空闲的inode节点数量
	unsigned int s_INODE_SIZE;				//inode大小

	unsigned int s_BLOCK_NUMBER;			//块的数量
	unsigned int s_BLOCK_FREE_NUMBER;		//空闲的块数量
	unsigned int s_BLOCK_SIZE;				//块的大小
	
	unsigned int s_free[BLOCKS_PER_GROUP];	//空闲块堆栈
	unsigned int s_FREE_ADDR;				//空闲块堆栈指针

	//规划磁盘分布
	int s_SUPERBLOCK_START_ADDR;
	int s_INODEBITMAP_START_ADDR;
	int s_BLOCKBITMAP_START_ADDR;
	int s_INODE_LABEL_ADDR;
	int s_BLOCK_START_ADDR;
};

// 声明inode结构体
struct Inode {
	unsigned int i_INODE;					//inode的标识
	unsigned int i_MODE;					//权限，rwx分别为读、写、执行
	unsigned int i_CONNECT;					//链接数，有多少文件指向一个同一个节点


	unsigned int i_SIZE;					//文件大小

	time_t i_ctime;							//inode上一次变动的时间
	time_t i_mtime;							//文件内容上一次变动的时间
	time_t i_atime;							//文件上一次打开的时间

	int i_DIR_BLOCK[10];					//10个直接块
	int i_INDIR_BLOCK_1;					//一级间接块，考虑到磁盘和文件大小限制，没有使用更高级间接块

};

// 声明目录结构体
struct DirItem {
	char d_ITEM_NAME[MAX_NAME_SIZE];			//目录项或者文件名
	int	d_INODE_ADDR;						//目录项对应的inode节点地址
};