#pragma once
#include "XX_filesystem.h"

class Block
{
private:
	short int No;

public:
	Block(short int No);
	virtual void printInfo();
	short int getNO();
	virtual void block_read(FILE *fr, char buf[]);
	virtual void block_write(FILE* fw, char buf[]);
};

class Superblock : Block
{
private:
	const short int Inode_Num = INODE_NUM;
	short int free_Inode_Num;
	const short int Block_Num = BLOCK_NUM;
	short int free_Block_Num;

public:
	Superblock(short int free_INum, short int free_BNum);
	Superblock();
	virtual void printInfo();
};

class Block_Bitmap : Block
{
private:
	bool b_isUsed[Block_Num];
public:
	Block_Bitmap(bool isUsed[Block_Num]);
	Block_Bitmap();
	virtual void printInfo();
};

class Inode_Bitmap : Block
{
private:
	bool i_isUsed[INODE_NUM];
public:
	Inode_Bitmap(bool isUsed[INODE_NUM]);
	Inode_Bitmap();
	virtual void printInfo();
};