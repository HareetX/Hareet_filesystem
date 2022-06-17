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

	// ��ӡ���ݿ���Ϣ�������κβ�����
	virtual void printInfo();

	// ���ݿ�ӿ�
	char* buffer_return(); // ����ָ�����ݿ����ݵ�ָ��

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
	//Superblock(int free_INum, int free_BNum);
	Superblock();

	// ��ʽ��Superblock
	virtual void format();

	// ��ӡSuperblock��Ϣ
	virtual void printInfo();

	// Superblock�ӿ�
	virtual void block_read(FILE* fpr); // ����Superblock
	virtual void block_write(FILE* fpw); // д������ļ�

	void use_renew(); // �����ļ�ʱSuperblock����
	void use_renew(int blocks_num); // д���ļ�ռ�ó���ԭ�п�ʱ�����ݴ���Ŀ��blocks_num����Superblock�Ŀ�ռ����Ϣ
	void free_renew(int b_cout); // ɾ���ļ�ʱ�����ݴ������Ҫɾ��block������������Superblock
};

class Block_Bitmap :
	public Block
{
private:
	bool b_isUsed[Block_Num]; // Blockռ�����
public:
	Block_Bitmap();

	// ��ʽ��Blockλͼ
	virtual void format();

	// ��ӡBlockռ�����
	virtual void printInfo();

	// Block_Bitmap�ӿ�
	virtual void block_read(FILE* fpr); // ����Block_Bitmap
	virtual void block_write(FILE* fpw); // д������ļ�

	int balloc(); // ������е�block�������׸�����block������

	void use_renew(int b_index); // ��Ҫռ�ÿ���blockʱ�����ݴ���Ŀ�ţ����¿�λͼ
	void free_renew(int b_index); // ��Ҫ�ͷſ���blockʱ�����ݴ���Ŀ�ţ����¿�λͼ
};

class Inode_Bitmap :
	public Block
{
private:
	bool i_isUsed[INODE_NUM]; // Inodeռ�����
public:
	Inode_Bitmap();

	// ��ʽ��Inodeλͼ
	virtual void format();

	// ��ӡInodeռ�����
	virtual void printInfo();

	// Inode_Bitmap�ӿ�
	virtual void block_read(FILE* fpr); // ����Inode_Bitmap
	virtual void block_write(FILE* fpw); // д������ļ�

	int ialloc(); // ������е�inode�������׸�����inode���������

	void use_renew(int i_index); // ��Ҫռ�ÿ���Inodeʱ�����ݴ����inode����������Inodeλͼ
	void free_renew(int i_index); // ��Ҫ�ͷſ���Inodeʱ�����ݴ����inode����������Inodeλͼ
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

	bool addBlock(int index); // ���ӿ��ÿ����

	char* getFile(char* buffer); // ����������ݿ黺�棬���ظ�Inodeָ����ļ����ݻ���

	int getIndex(int No); // ����Inodeָ��Ŀ�������еĵ�No�ſ�Ŀ��
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

	// Inode_Label�ӿ�
	virtual void block_read(FILE* fpr); // ����Inode_Label
	virtual void block_write(FILE* fpw); // д������ļ�

	Inode* getInode(int No); // ���ص�No��Inode

	// �����ļ�ʱ�����ݴ���Ŀ���inode���������п��š��ļ����͡��ļ���С����ǰĿ¼��inode��������ǰĿ¼���ļ����ݴ�С������Inode�����������ļ���inode���ļ�����Ŀ¼�ļ���inode
	void use_renew(int i_index, int b_index, int mode, int f_size, int cur_dir_i_index, int cur_dir_size); 
	// ɾ���ļ�ʱ�����ݴ���Ŀ���inode��������ǰĿ¼��inode��������ǰĿ¼���ļ����ݴ�С������Inode�����������ļ���inode���ļ�����Ŀ¼�ļ���inode
	void free_renew(int i_index, int cur_dir_i_index, int cur_dir_size); 
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

	// ��ʾ����������ݿ�ռ�����
	void printBlocks();
	// ��ʾ�������Inodeռ�����
	void printInodes();
	// ��ʾ������̳�����ռ�����
	void printSuperblock();

	// ���ݴ����inode�������Ѷ�Ӧ���ļ����ļ����ݴ����ݿ����
	char* file_read(int i_index);
	// ���ݴ����inode���������ض�Ӧ���ļ����ļ���С
	int file_size(int i_index);

	// ���ݴ����inode�������ļ����棬���ļ�����д�����ݿ��Ӧ��λ��
	bool file_write(int i_index, const char* buf);

	// Disk��Ŀ¼�Ľӿ�
	Dentry dentry_read(int dentry_address, char* buf); // ���ݴ����Ŀ¼���ַ�����ݿ�����ݻ��棬�����ݻ����ȡ������һ����Ӧ��Ŀ¼��
	void dentry_write(int dentry_address, Dentry dentry); // ����ʼλ�������ݿ����ݻ������λ����Ŀ¼����ݴ����Ŀ¼���ַ��д�����ݿ����ݻ���Ķ�Ӧλ��
	void dentry_write(int dentry_address, char* buf, Dentry dentry); // ����ʼλ���Ǵ����buf����λ����Ŀ¼����ݴ����Ŀ¼���ַ��д�����ݿ����ݻ���Ķ�Ӧλ��

	Directory dir_read(int i_No); // ���ݴ����inode�����������ݿ����ݻ���buffer�ж�ȡ��������Ŀ¼
	void dir_write(int i_No, Directory dir); // ���ݴ����inode��������Ŀ¼д�����ݿ����ݻ���buffer

	// ռ�ÿռ�ʱ�����ݴ���Ŀ��п��š�����inode�������ļ����͡��ļ���С����ǰĿ¼��inode��������ǰĿ¼���ļ����ݴ�С������Disk
	void use_renew(int b_index, int i_index, int mode, int f_size, int cur_dir_i_index, int cur_dir_size);
	// �ͷſռ�ʱ�����ݴ���Ŀ���inode��������ǰĿ¼��inode��������ǰĿ¼���ļ����ݴ�С������Disk
	void free_renew(int i_index, int cur_dir_i_index, int cur_dir_size);
};