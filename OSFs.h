#include<iostream>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

// ���к궨��
// ����ڵ�
	
#define OSFS_NAME "OSfs.sys"				//������������ļ���
#define OS_NAME	"OSfS(OS-File-System)"		// �ļ�ϵͳ��

#define BLOCK_SIZE 4096						//ÿ������Ĵ�СΪ4096B
#define BLOCK_NUMBER 10240					//������Ŀ�趨Ϊ10240

#define INODE_SIZE 256						//ÿ��inode�ڵ�Ĵ�СΪ256B
#define INODE_NUMBER 640					//inode�ڵ���Ŀ�趨Ϊ640


#define BLOCK_EACH_GROUP 64	 // ��������	//����һ�����п��ջ����64�����̿��ַ
#define MAX_NAME_SIZE 28					//������ֳ���


// �����ṹ��

struct SuperBlock{
	unsigned int S_SUPERBLOCK_SIZE;			//������Ĵ�С

	unsigned int S_INODE_NUMBER;			//inode�ڵ�����
	unsigned int S_INODE_FREE_NUMBER;		//���е�inode�ڵ�����
	unsigned int S_INODE_SIZE;				//inode��С
	
	unsigned int S_BLOCK_NUMBER;			//�������
	unsigned int S_BLOCK_FREE_NUMBER;		//���еĿ�����
	unsigned int S_BLOCK_SIZE;				//��Ĵ�С

	unsigned int S_FREE_ADDR;				//���п��ջָ��

	
	//�滮���̷ֲ�
	int S_SUPERBLOCK_START_ADDR;
	int S_INODEBITMAP_START_ADDR;
	int S_BLOCKBITMAP_START_ADDR;
	int S_INODE_START_ADDR;
	int S_BLOCK_START_ADDR;

};


// ����inode�ڵ�

struct Inode{
	unsigned int I_INODE;					//inode�ı�ʶ
	unsigned int I_MODE;					//Ȩ�ޣ�rwx�ֱ�Ϊ����д��ִ��
	unsigned int I_CONNECT;					//���������ж����ļ�ָ��һ��ͬһ���ڵ�


	unsigned int I_SIZE;					//�ļ���С
	
	time_t I_ctime;							//inode��һ�α䶯��ʱ��
	time_t I_mtime;							//�ļ�������һ�α䶯��ʱ��
	time_t I_atime;						//�ļ���һ�δ򿪵�ʱ��

	int I_DIR_BLOCK[10];					//10��ֱ�ӿ�
	int I_INDIR_BLOCK_1;					//һ����ӿ�

};

// ����Ŀ¼��Ŀ

struct DirItem {
	char ITEM_NAME[MAX_NAME_SIZE];			//Ŀ¼������ļ���
	int	D_INODE_ADDR;						//Ŀ¼���Ӧ��inode�ڵ��
};

// ����ȫ�ֱ���

extern SuperBlock* SUPERBLCOK;				//������ָ��

extern const int INODE_START_ADDR;
extern const int SUPERBLOCK_START_ADDR;		//������ ƫ�Ƶ�ַ,ռһ�����̿�
extern const int INODEBITMAP_START_ADDR;	//inodeλͼ ƫ�Ƶ�ַ
extern const int BLOCKBITMAP_START_ADDR;	//blockλͼ ƫ�Ƶ�ַ
extern const int INODE_START_ADDR;			//inode�ڵ��� ƫ�Ƶ�ַ
extern const int BLOCK_START_ADDR;			//block������ ƫ�Ƶ�ַ

extern const int MAX_FILE_SIZE;				//�����ļ�����С
extern const int SUM_SIZE;					//��������ļ���С


extern int ROOT_DIR_ADDR;					//��Ŀ¼inode��ַ
extern int CUR_DIR_ADDR;					//��ǰĿ¼


extern bool isLogin;						//�Ƿ����û���½

extern FILE* fw;							//��������ļ� д�ļ�ָ��
extern FILE* fr;							//��������ļ� ���ļ�ָ��

extern bool INODE_BITMAP[INODE_NUMBER];		//inodeλͼ
extern bool BLOCK_BITMAP[BLOCK_NUMBER];		//���̿�λͼ

extern char BUFFER[1024 * 1024 * 50] = {0};				//50M������������������ļ�








