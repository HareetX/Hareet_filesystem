#include"XX_filesystem.h"
#include"DiskStruct.h"


// 初始化相关函数

bool Format()		// 格式化虚拟磁盘文件
{
	// 初始化超级块
	// 使用头文件定义的超级块指针Superblock
	Superblock->s_SUPERBLOCK_SIZE = BLOCK_SIZE * 1; //超级块的大小
	Superblock->s_INODE_NUMBER = INODE_NUM;			//inode节点数量
	Superblock->s_INODE_SIZE = INODE_SIZE;			//inode大小
	Superblock->s_BLOCK_NUMBER = BLOCK_NUM;			//块的数量
	Superblock->s_BLOCK_SIZE = BLOCK_SIZE;			//块的大小
	Superblock->s_BLOCK_FREE_NUMBER = Block_Num;	//块的大小
	Superblock->s_FREE_ADDR = Block_Address;		//第一个空闲块的位置,
	Superblock->s_SUPERBLOCK_START_ADDR = Super_Block_Address;
	Superblock->s_INODEBITMAP_START_ADDR = Inode_Bitmap_Address;
	Superblock->s_BLOCKBITMAP_START_ADDR = Block_Bitmap_Address;
	Superblock->s_INODE_LABEL_ADDR = Inode_Label_Address;
	Superblock->s_BLOCK_START_ADDR = Block_Address;
	
	// 初始化磁盘块区，运用成组链接法
	int i, j;
	for (i = BLOCK_NUM / BLOCKS_PER_GROUP - 1; i >= 0; i--) {
		if (i == BLOCK_NUM / BLOCKS_PER_GROUP - 1)
			Superblock->s_free[0] = -1;	//没有下一个空闲块了
		else
			Superblock->s_free[0] = Block_Address + (i + 1) * BLOCKS_PER_GROUP * BLOCK_SIZE;	//指向下一个空闲块
		for (j = 1; j < BLOCKS_PER_GROUP; j++) {
			Superblock->s_free[j] = Block_Address + (i * BLOCKS_PER_GROUP + j) * BLOCK_SIZE;
		}
		fseek(fw, Block_Address + i * BLOCKS_PER_GROUP * BLOCK_SIZE, SEEK_SET);
		fwrite(Superblock->s_free, sizeof(Superblock->s_free), 1, fw);	//填满这个磁盘块，4096B
	}
	
	//超级块写入到刚刚初始化的虚拟磁盘文件
	fseek(fw, Super_Block_Address, SEEK_SET);
	fwrite(Superblock, sizeof(SuperBlock), 1, fw);


	// 初始化inode位图,
	memset(Inode_Bitmap, 0, sizeof(Inode_Bitmap));
	fseek(fw,Inode_Bitmap_Address,SEEK_SET);
	fwrite(Inode_Bitmap, sizeof(Inode_Bitmap), 1, fw);
	//读取inode位图
	fseek(fr, Inode_Bitmap_Address, SEEK_SET);
	fread(Inode_Bitmap, sizeof(Inode_Bitmap), 1, fr);


	// 初始化block位图
	memset(Block_Bitmap, 0, sizeof(Block_Bitmap));
	fseek(fw, Block_Bitmap_Address, SEEK_SET);
	fwrite(Block_Bitmap, sizeof(Block_Bitmap), 1, fw);
	// 读取block位图
	fseek(fr, Block_Bitmap_Address, SEEK_SET);
	fread(Block_Bitmap, size(Block_Bitmap), 1, fr);
	
	fflush(fw);
	fflush(fr);

	
	// 创建根目录 "/"
	
	int inoAddr = Inode_Label_Address;	// 分配inode	
	//更新超级块
	Superblock->s_INODE_FREE_NUMBER--;	//空闲inode数-1
	fseek(fw, Super_Block_Address, SEEK_SET);
	fwrite(Superblock, sizeof(SuperBlock), 1, fw);
	//更新inode位图
	Inode_Bitmap[0] = 1;
	fseek(fw, Inode_Bitmap_Address, SEEK_SET);
	fwrite(&Inode_Bitmap[0], sizeof(bool), 1, fw);
	fflush(fw);
	
	int bloAddr = Block_Address;		// 分配块
	//更新超级块
	Superblock->s_BLOCK_FREE_NUMBER--;	//空闲块数-1
	fseek(fw, Super_Block_Address, SEEK_SET);
	fwrite(Superblock, sizeof(SuperBlock), 1, fw);
	//更新block位图
	Block_Bitmap[0] = 1;
	fseek(fw, Block_Bitmap_Address, SEEK_SET);	//(retAddr-Block_StartAddr)/BLOCK_SIZE为第几个空闲块
	fwrite(&Block_Bitmap[0], sizeof(bool), 1, fw);
	fflush(fw);

	//	在这个申请的块中加入一个条目 "."
	DirItem dirlist[16] = { 0 };
	strcpy(dirlist[0].d_ITEM_NAME, ".");
	dirlist[0].d_INODE_ADDR = inoAddr;

	//写回磁盘块
	fseek(fw, bloAddr, SEEK_SET);
	fwrite(dirlist, sizeof(dirlist), 1, fw);
	
	//定义inode
	Inode dir;
	dir.i_INODE = 0;			// inode的标识
	dir.i_CONNECT = 1;			//链接数为1，当前目录,"."
	dir.i_atime = time(NULL);
	dir.i_ctime = time(NULL);
	dir.i_mtime = time(NULL);
	
	dir.i_DIR_BLOCK[0] = bloAddr;	//第一个直接块，地址为bloAddr
	for (i = 1; i < 10; i++) {		//未使用剩下的9个直接块
		dir.i_DIR_BLOCK[i] = -1;
	}
	dir.i_SIZE = Superblock->s_BLOCK_SIZE;
	dir.i_INDIR_BLOCK_1 = -1;		//没使用一级间接块

	//写回inode
	fseek(fw, inoAddr, SEEK_SET);
	fwrite(&dir, sizeof(Inode), 1, fw);
	fflush(fw);

};

void Initial()		// 初始化虚拟磁盘文件
{




};
bool Install()		// 安装虚拟磁盘文件,将关键信息如超级块读入到内存
{
	//读取超级块
	fseek(fr, Super_Block_Address, SEEK_SET);
	fread(Superblock, sizeof(SuperBlock), 1, fr);

	//读取inode位图
	fseek(fr, Inode_Bitmap_Address, SEEK_SET);
	fread(Inode_Bitmap, sizeof(Inode_Bitmap), 1, fr);

	//读取block位图
	fseek(fr, Block_Bitmap_Address, SEEK_SET);
	fread(Block_Bitmap, sizeof(Block_Bitmap), 1, fr);

	return true;



};	
	
void Login()// 用户登录

{

};		

void cmd(string args);
