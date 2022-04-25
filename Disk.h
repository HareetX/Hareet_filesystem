#pragma once
#include "XX_filesystem.h"

// Block
class Block
{
private:
	int No; // ���

public:
	Block(int No);

	// ��ʽ����
	virtual void format() = 0;

	// ��ӡ����Ϣ
	virtual void printInfo() = 0;

	// ��ӿ�
	int getNO(); // ��ȡ���
};

// ���ݿ�
class DataBlock :
	public Block
{
private:
	// ��������ļ�����,����д����д�ļ���ʽ��ʱ����յ��ļ�
	char* buffer;
public:
	DataBlock();
	~DataBlock();
	
	// ��ʽ�����ݿ�
	virtual void format();

	// ��ӡ���ݿ���Ϣ
	virtual void printInfo(); // TODO

	// ���ݿ�ӿ�
	char* buffer_return(); // ����ָ�����ݿ����ݵ�ָ��
	void buffer_write(char buf[Block_Num * BLOCK_SIZE]); // ���ݴ���������д���ݿ������
};

class Superblock :
	public Block
{
private:
	const int s_Inode_Num = INODE_NUM; // ��Inode��
	int free_Inode_Num; // ����Inode��
	const int s_Block_Num = Block_Num; // ��Block��
	int free_Block_Num; // ����Block��

public:
	Superblock(int free_INum, int free_BNum);
	Superblock();

	// ��ʽ��Superblock
	virtual void format();

	// ��ӡSuperblock��Ϣ
	virtual void printInfo();
};

class Block_Bitmap :
	public Block
{
private:
	bool b_isUsed[Block_Num]; // Blockռ�����
public:
	Block_Bitmap(bool isUsed[Block_Num]);
	Block_Bitmap();

	// ��ʽ��Blockλͼ
	virtual void format();

	// ��ӡBlockռ�����
	virtual void printInfo();

};

class Inode_Bitmap :
	public Block
{
private:
	bool i_isUsed[INODE_NUM]; // Inodeռ�����
public:
	Inode_Bitmap(bool isUsed[INODE_NUM]);
	Inode_Bitmap();

	// ��ʽ��Inodeλͼ
	virtual void format();

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
	Inode(int No, int mode, int size, int time_c, int index[BLOCK_INDEX]);
	Inode(int No);
	Inode();

	// ��ʽ��Inode
	void format();

	// ��ӡ��Inode��Ϣ
	void printInfo();

	// Inode�ӿ�
	int getI_No(); // ���Inode���
	void setI_No(int No); // ����Inode���
};

// Inode��
class Inode_Label :
	public Block
{
private:
	Inode inode[INODE_NUM];
public:
	Inode_Label();

	// ��ʽ��Inode��
	virtual void format();

	// ��ӡInode����Ϣ
	virtual void printInfo(); /*TODO*/
};

// Disk��Ԫ
class Disk
{
private:
	Superblock spb; // Disk��Ԫ�е�Superblock
	Block_Bitmap b_bmap; // Disk��Ԫ�е�Blockλͼ
	Inode_Bitmap i_bmap; // Disk��Ԫ�е�Inodeλͼ
	Inode_Label i_label; // Disk��Ԫ�е�Inode��
	DataBlock d_block; // ���ݿ黺��

public:

	void format(); // Disk��Ԫ��ʽ��

	/*
	* Disk��Ԫ�ӿ�
	*/ 

	// �������ļ�����Disk��Ԫ
	
	void spb_read(FILE* fr);// Superblock����
	void b_bmap_read(FILE *fr);// Blockλͼ����
	void i_bmap_read(FILE* fr);// Inodeλͼ����
	void inode_read(FILE* fr);// Inode�����
	void d_block_read(FILE* fr);// ���ݿ黺�����
	
	
	// ��Disk��Ԫ��������ļ�
	
	void spb_write(FILE* fw);// Superblock����
	void b_bmap_write(FILE* fw);// Blockλͼ����
	void i_bmap_write(FILE* fw);// Inodeλͼ����
	void inode_write(FILE* fw);// Inode�����
	void d_block_write(FILE* fw);// ���ݿ黺�����
};