#pragma once
#include<time.h>
#include"XX_filesystem.h"

// �����ṹ��
// ����������ṹ��
struct SuperBlock {
	unsigned int s_SUPERBLOCK_SIZE;			//������Ĵ�С

	unsigned int s_INODE_NUMBER;			//inode�ڵ�����
	unsigned int s_INODE_FREE_NUMBER;		//���е�inode�ڵ�����
	unsigned int s_INODE_SIZE;				//inode��С

	unsigned int s_BLOCK_NUMBER;			//�������
	unsigned int s_BLOCK_FREE_NUMBER;		//���еĿ�����
	unsigned int s_BLOCK_SIZE;				//��Ĵ�С
	
	unsigned int s_free[BLOCKS_PER_GROUP];	//���п��ջ
	unsigned int s_FREE_ADDR;				//���п��ջָ��

	//�滮���̷ֲ�
	int s_SUPERBLOCK_START_ADDR;
	int s_INODEBITMAP_START_ADDR;
	int s_BLOCKBITMAP_START_ADDR;
	int s_INODE_LABEL_ADDR;
	int s_BLOCK_START_ADDR;
};

// ����inode�ṹ��
struct Inode {
	unsigned int i_INODE;					//inode�ı�ʶ
	unsigned int i_MODE;					//Ȩ�ޣ�rwx�ֱ�Ϊ����д��ִ��
	unsigned int i_CONNECT;					//���������ж����ļ�ָ��һ��ͬһ���ڵ�


	unsigned int i_SIZE;					//�ļ���С

	time_t i_ctime;							//inode��һ�α䶯��ʱ��
	time_t i_mtime;							//�ļ�������һ�α䶯��ʱ��
	time_t i_atime;							//�ļ���һ�δ򿪵�ʱ��

	int i_DIR_BLOCK[10];					//10��ֱ�ӿ�
	int i_INDIR_BLOCK_1;					//һ����ӿ飬���ǵ����̺��ļ���С���ƣ�û��ʹ�ø��߼���ӿ�

};

// ����Ŀ¼�ṹ��
struct DirItem {
	char d_ITEM_NAME[MAX_NAME_SIZE];			//Ŀ¼������ļ���
	int	d_INODE_ADDR;						//Ŀ¼���Ӧ��inode�ڵ��ַ
};