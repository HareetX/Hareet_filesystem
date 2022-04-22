#include<iostream>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

// 进行宏定义
// 块与节点
	
#define OSFS_NAME "OSfs.sys"				//定义虚拟磁盘文件名
#define OS_NAME	"OSfS(OS-File-System)"		// 文件系统名

#define BLOCK_SIZE 4096						//每个区块的大小为4096B
#define BLOCK_NUMBER 10240					//区块数目设定为10240

#define INODE_SIZE 256						//每个inode节点的大小为256B
#define INODE_NUMBER 640					//inode节点数目设定为640


#define BLOCK_EACH_GROUP 64	 // 数量待定	//定义一个空闲块堆栈最多存64个磁盘块地址
#define MAX_NAME_SIZE 28					//最大名字长度


// 声明结构体

struct SuperBlock{
	unsigned int S_SUPERBLOCK_SIZE;			//超级块的大小

	unsigned int S_INODE_NUMBER;			//inode节点数量
	unsigned int S_INODE_FREE_NUMBER;		//空闲的inode节点数量
	unsigned int S_INODE_SIZE;				//inode大小
	
	unsigned int S_BLOCK_NUMBER;			//块的数量
	unsigned int S_BLOCK_FREE_NUMBER;		//空闲的块数量
	unsigned int S_BLOCK_SIZE;				//块的大小

	unsigned int S_FREE_ADDR;				//空闲块堆栈指针

	
	//规划磁盘分布
	int S_SUPERBLOCK_START_ADDR;
	int S_INODEBITMAP_START_ADDR;
	int S_BLOCKBITMAP_START_ADDR;
	int S_INODE_START_ADDR;
	int S_BLOCK_START_ADDR;

};


// 声明inode节点

struct Inode{
	unsigned int I_INODE;					//inode的标识
	unsigned int I_MODE;					//权限，rwx分别为读、写、执行
	unsigned int I_CONNECT;					//链接数，有多少文件指向一个同一个节点


	unsigned int I_SIZE;					//文件大小
	
	time_t I_ctime;							//inode上一次变动的时间
	time_t I_mtime;							//文件内容上一次变动的时间
	time_t I_atime;						//文件上一次打开的时间

	int I_DIR_BLOCK[10];					//10个直接块
	int I_INDIR_BLOCK_1;					//一级间接块

};

// 声明目录项目

struct DirItem {
	char ITEM_NAME[MAX_NAME_SIZE];			//目录项或者文件名
	int	D_INODE_ADDR;						//目录项对应的inode节点地
};

// 声明全局变量

extern SuperBlock* SUPERBLCOK;				//超级块指针

extern const int INODE_START_ADDR;
extern const int SUPERBLOCK_START_ADDR;		//超级块 偏移地址,占一个磁盘块
extern const int INODEBITMAP_START_ADDR;	//inode位图 偏移地址
extern const int BLOCKBITMAP_START_ADDR;	//block位图 偏移地址
extern const int INODE_START_ADDR;			//inode节点区 偏移地址
extern const int BLOCK_START_ADDR;			//block数据区 偏移地址

extern const int MAX_FILE_SIZE;				//单个文件最大大小
extern const int SUM_SIZE;					//虚拟磁盘文件大小


extern int ROOT_DIR_ADDR;					//根目录inode地址
extern int CUR_DIR_ADDR;					//当前目录


extern bool isLogin;						//是否有用户登陆

extern FILE* fw;							//虚拟磁盘文件 写文件指针
extern FILE* fr;							//虚拟磁盘文件 读文件指针

extern bool INODE_BITMAP[INODE_NUMBER];		//inode位图
extern bool BLOCK_BITMAP[BLOCK_NUMBER];		//磁盘块位图

extern char BUFFER[1024 * 1024 * 50] = {0};				//50M，缓存整个虚拟磁盘文件








