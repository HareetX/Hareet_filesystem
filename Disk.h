#pragma once
#include "Directory.h"

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
	//int getNO(); // ��ȡ���

	virtual void block_read(FILE* fpr) = 0; // �ѿ������ݴӴ����ļ��������
	virtual void block_write(FILE* fpw) = 0; // �ѿ������ݴӳ���д������ļ�
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
	virtual void printInfo(); // TODO*

	// ���ݿ�ӿ�
	char* buffer_return(); // ����ָ�����ݿ����ݵ�ָ��
	//void buffer_write(char buf[Block_Num * BLOCK_SIZE]); // ���ݴ���������д���ݿ������

	virtual void block_read(FILE* fpr); // �������ݿ�
	virtual void block_write(FILE* fpw); // д������ļ�

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

	// Superblock�ӿ�
	virtual void block_read(FILE* fpr); // ����Superblock
	virtual void block_write(FILE* fpw); // д������ļ�

	void use_renew(); // �����ļ�ʱSuperblock����
	void use_renew(int blocks_num); // д���ļ�ռ�ó���ԭ�п�ʱ������Superblock�Ŀ�ռ����Ϣ
	void free_renew(int b_cout); // ɾ���ļ�ʱSuperblock����
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

	// Block_Bitmap�ӿ�
	virtual void block_read(FILE* fpr); // ����Block_Bitmap
	virtual void block_write(FILE* fpw); // д������ļ�

	int balloc(); // ������е�block�������׸�����block������

	void use_renew(int b_index); // ��Ҫռ�ÿ���blockʱ���¿�λͼ
	void free_renew(int b_index); // ��Ҫ�ͷſ���blockʱ���¿�λͼ
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

	// Inode_Bitmap�ӿ�
	virtual void block_read(FILE* fpr); // ����Inode_Bitmap
	virtual void block_write(FILE* fpw); // д������ļ�

	int ialloc(); // ������е�inode�������׸�����inode���������

	void use_renew(int i_index); // ��Ҫռ�ÿ���Inodeʱ���¿�λͼ
	void free_renew(int i_index); // ��Ҫ�ͷſ���Inodeʱ���¿�λͼ
};

class Inode
{
private:
	int i_No; // Inode���
	int f_mode; // �ļ�����
	int f_size; // �ļ���С
	//int c_time; // ����ʱ��
	int block_index[BLOCK_INDEX]; // Inodeָ��Ŀ������

public:
	Inode(int No, int mode, int size, int index[BLOCK_INDEX]);
	Inode(int No);
	Inode();

	// ��ʽ��Inode
	void format();

	// ��ӡ��Inode��Ϣ
	void printInfo();

	// Inode�ӿ�
	int getI_No(); // ���Inode���
	void setI_No(int No); // ����Inode���

	int getF_Mode(); // ����ļ�����
	void setF_Mode(int mode); // �����ļ�����

	int getF_Size(); // ����ļ���С
	void setF_Size(int size); // �����ļ���С

	//int getC_Time(); // ��ô���ʱ��
	//void setC_Time(int time); // ���ô���ʱ��

	bool addBlock(int index); // ���ӿ��ÿ����

	char* getFile(char* buffer); // ���ظ�Inodeָ����ļ�

	int getIndex(int No); // ���ص�No�ſ�Ŀ��
};

// Inode��
class Inode_Label :
	public Block
{
private:
	Inode inode[INODE_NUM];
public:
	Inode_Label();
	//~Inode_Label();

	// ��ʽ��Inode��
	virtual void format();

	// ��ӡInode����Ϣ
	virtual void printInfo(); /*TODO*/

	// Inode_Label�ӿ�
	virtual void block_read(FILE* fpr); // ����Inode_Label
	virtual void block_write(FILE* fpw); // д������ļ�

	Inode* getInode(int No); // ���ص�No��Inode

	void use_renew(int i_index, int b_index, int mode, int f_size, int cur_dir_i_index, int cur_dir_size); // �����ļ�ʱ������Inode�����������ļ���inode���ļ�����Ŀ¼�ļ���inode
	void free_renew(int i_index, int cur_dir_i_index, int cur_dir_size); // �����ļ�ʱ������Inode�����������ļ���inode���ļ�����Ŀ¼�ļ���inode
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

	void format(Block& b); // Disk��Ԫ�ڸ�������ʽ��
	void disk_format(); // Disk��Ԫ��ʽ��

	/*
	* Disk��Ԫ�ӿ�
	*/ 

	// �������ļ�����Disk��Ԫ
	void block_read(Block& b, FILE* fpr); // Disk��Ԫ�ڸ��������ݶ���
	void disk_read(FILE* fpr); // Disk��Ԫ����
		
	// ��Disk��Ԫ��������ļ�
	void block_write(Block& b, FILE* fpw); // Disk��Ԫ�ڸ���������д��
	void disk_write(FILE* fpw); // // Disk��Ԫд��

	// �����׸�����block�����
	int d_balloc();
	// �����׸�����Inode�����
	int d_ialloc();

	// ���ļ����ݴ����ݿ����
	char* file_read(int i_index);
	int file_size(int i_index);
	// ���ļ�����д�����ݿ�
	bool file_write(int i_index, const char* buf);

	// Disk��Ŀ¼�Ľӿ�
	Dentry dentry_read(int dentry_address); // �����ݿ��ȡĿ¼��
	Dentry dentry_read(int dentry_address, char* buf); // �����ݿ��ȡĿ¼��
	void dentry_write(int dentry_address, Dentry dentry); // ��Ŀ¼��д�����ݿ�
	void dentry_write(int dentry_address, char* buf, Dentry dentry); // ��Ŀ¼��д�����ݿ�

	Directory dir_read(int i_No); // ��buffer��ȡĿ¼
	void dir_write(int i_No, Directory dir); // ��Ŀ¼д��buffer

	// ռ�ÿռ�ʱ����Disk
	void use_renew(int b_index, int i_index, int mode, int f_size, int cur_dir_i_index, int cur_dir_size);
	// �ͷſռ�ʱ����Disk
	void free_renew(int i_index, int cur_dir_i_index, int cur_dir_size);
};