#pragma once
#include "XX_filesystem.h"

// ���ݿ�Block
class Block
{
private:
	int No; // ���ݿ���

public:
	Block(int No);

	// ��ӡ���ݿ���
	virtual void printInfo();

	//���ݿ�ӿ�
	int getNO(); // ��ȡ���
	virtual void block_read(FILE *fr, char buf[]); // �Ӵ����ļ���ȡ�����ݿ����ݴ���buf����
	virtual void block_write(FILE* fw, char buf[]); // ��buf�������ݴ�������ļ���Ӧ�����ݿ�
};

class Superblock : Block
{
private:
	const int s_Inode_Num = INODE_NUM; // ��Inode��
	int free_Inode_Num; // ����Inode��
	const int s_Block_Num = Block_Num; // ��Block��
	int free_Block_Num; // ����Block��

public:
	Superblock(int free_INum, int free_BNum);
	Superblock();

	// ��ӡSuperblock��Ϣ
	virtual void printInfo();
};

class Block_Bitmap : Block
{
private:
	bool b_isUsed[Block_Num]; // Blockռ�����
public:
	Block_Bitmap(bool isUsed[Block_Num]);
	Block_Bitmap();

	// ��ӡBlockռ�����
	virtual void printInfo();
};

class Inode_Bitmap : Block
{
private:
	bool i_isUsed[INODE_NUM]; // Inodeռ�����
public:
	Inode_Bitmap(bool isUsed[INODE_NUM]);
	Inode_Bitmap();

	// ��ӡInodeռ�����
	virtual void printInfo();
};

class Inode
{
private:
	int i_No; // Inode���
	int f_mode; // �ļ�����
	int f_size; // �ļ���С
	int c_time; // ����ʱ��
	int block_index[BLOCK_INDEX]; // Inodeָ��Ŀ������

public:
	Inode(int No, int mode, int size, int time, int index[BLOCK_INDEX]);
	Inode(int No, int mode, int size, int time);

	// ��ӡ��Inode��Ϣ
	void printInfo();
};