#include"XX_filesystem.h"
#include"DiskStruct.h"


// ��ʼ����غ���

bool Format()		// ��ʽ����������ļ�
{
	// ��ʼ��������
	// ʹ��ͷ�ļ�����ĳ�����ָ��Superblock
	Superblock->s_SUPERBLOCK_SIZE = BLOCK_SIZE * 1; //������Ĵ�С
	Superblock->s_INODE_NUMBER = INODE_NUM;			//inode�ڵ�����
	Superblock->s_INODE_SIZE = INODE_SIZE;			//inode��С
	Superblock->s_BLOCK_NUMBER = BLOCK_NUM;			//�������
	Superblock->s_BLOCK_SIZE = BLOCK_SIZE;			//��Ĵ�С
	Superblock->s_BLOCK_FREE_NUMBER = Block_Num;	//��Ĵ�С
	Superblock->s_FREE_ADDR = Block_Address;		//��һ�����п��λ��,
	Superblock->s_SUPERBLOCK_START_ADDR = Super_Block_Address;
	Superblock->s_INODEBITMAP_START_ADDR = Inode_Bitmap_Address;
	Superblock->s_BLOCKBITMAP_START_ADDR = Block_Bitmap_Address;
	Superblock->s_INODE_LABEL_ADDR = Inode_Label_Address;
	Superblock->s_BLOCK_START_ADDR = Block_Address;
	
	// ��ʼ�����̿��������ó������ӷ�
	int i, j;
	for (i = BLOCK_NUM / BLOCKS_PER_GROUP - 1; i >= 0; i--) {
		if (i == BLOCK_NUM / BLOCKS_PER_GROUP - 1)
			Superblock->s_free[0] = -1;	//û����һ�����п���
		else
			Superblock->s_free[0] = Block_Address + (i + 1) * BLOCKS_PER_GROUP * BLOCK_SIZE;	//ָ����һ�����п�
		for (j = 1; j < BLOCKS_PER_GROUP; j++) {
			Superblock->s_free[j] = Block_Address + (i * BLOCKS_PER_GROUP + j) * BLOCK_SIZE;
		}
		fseek(fw, Block_Address + i * BLOCKS_PER_GROUP * BLOCK_SIZE, SEEK_SET);
		fwrite(Superblock->s_free, sizeof(Superblock->s_free), 1, fw);	//����������̿飬4096B
	}
	
	//������д�뵽�ոճ�ʼ������������ļ�
	fseek(fw, Super_Block_Address, SEEK_SET);
	fwrite(Superblock, sizeof(SuperBlock), 1, fw);


	// ��ʼ��inodeλͼ,
	memset(Inode_Bitmap, 0, sizeof(Inode_Bitmap));
	fseek(fw,Inode_Bitmap_Address,SEEK_SET);
	fwrite(Inode_Bitmap, sizeof(Inode_Bitmap), 1, fw);
	//��ȡinodeλͼ
	fseek(fr, Inode_Bitmap_Address, SEEK_SET);
	fread(Inode_Bitmap, sizeof(Inode_Bitmap), 1, fr);


	// ��ʼ��blockλͼ
	memset(Block_Bitmap, 0, sizeof(Block_Bitmap));
	fseek(fw, Block_Bitmap_Address, SEEK_SET);
	fwrite(Block_Bitmap, sizeof(Block_Bitmap), 1, fw);
	// ��ȡblockλͼ
	fseek(fr, Block_Bitmap_Address, SEEK_SET);
	fread(Block_Bitmap, size(Block_Bitmap), 1, fr);
	
	fflush(fw);
	fflush(fr);

	
	// ������Ŀ¼ "/"
	
	int inoAddr = Inode_Label_Address;	// ����inode	
	//���³�����
	Superblock->s_INODE_FREE_NUMBER--;	//����inode��-1
	fseek(fw, Super_Block_Address, SEEK_SET);
	fwrite(Superblock, sizeof(SuperBlock), 1, fw);
	//����inodeλͼ
	Inode_Bitmap[0] = 1;
	fseek(fw, Inode_Bitmap_Address, SEEK_SET);
	fwrite(&Inode_Bitmap[0], sizeof(bool), 1, fw);
	fflush(fw);
	
	int bloAddr = Block_Address;		// �����
	//���³�����
	Superblock->s_BLOCK_FREE_NUMBER--;	//���п���-1
	fseek(fw, Super_Block_Address, SEEK_SET);
	fwrite(Superblock, sizeof(SuperBlock), 1, fw);
	//����blockλͼ
	Block_Bitmap[0] = 1;
	fseek(fw, Block_Bitmap_Address, SEEK_SET);	//(retAddr-Block_StartAddr)/BLOCK_SIZEΪ�ڼ������п�
	fwrite(&Block_Bitmap[0], sizeof(bool), 1, fw);
	fflush(fw);

	//	���������Ŀ��м���һ����Ŀ "."
	DirItem dirlist[16] = { 0 };
	strcpy(dirlist[0].d_ITEM_NAME, ".");
	dirlist[0].d_INODE_ADDR = inoAddr;

	//д�ش��̿�
	fseek(fw, bloAddr, SEEK_SET);
	fwrite(dirlist, sizeof(dirlist), 1, fw);
	
	//����inode
	Inode dir;
	dir.i_INODE = 0;			// inode�ı�ʶ
	dir.i_CONNECT = 1;			//������Ϊ1����ǰĿ¼,"."
	dir.i_atime = time(NULL);
	dir.i_ctime = time(NULL);
	dir.i_mtime = time(NULL);
	
	dir.i_DIR_BLOCK[0] = bloAddr;	//��һ��ֱ�ӿ飬��ַΪbloAddr
	for (i = 1; i < 10; i++) {		//δʹ��ʣ�µ�9��ֱ�ӿ�
		dir.i_DIR_BLOCK[i] = -1;
	}
	dir.i_SIZE = Superblock->s_BLOCK_SIZE;
	dir.i_INDIR_BLOCK_1 = -1;		//ûʹ��һ����ӿ�

	//д��inode
	fseek(fw, inoAddr, SEEK_SET);
	fwrite(&dir, sizeof(Inode), 1, fw);
	fflush(fw);

};

void Initial()		// ��ʼ����������ļ�
{




};
bool Install()		// ��װ��������ļ�,���ؼ���Ϣ�糬������뵽�ڴ�
{
	//��ȡ������
	fseek(fr, Super_Block_Address, SEEK_SET);
	fread(Superblock, sizeof(SuperBlock), 1, fr);

	//��ȡinodeλͼ
	fseek(fr, Inode_Bitmap_Address, SEEK_SET);
	fread(Inode_Bitmap, sizeof(Inode_Bitmap), 1, fr);

	//��ȡblockλͼ
	fseek(fr, Block_Bitmap_Address, SEEK_SET);
	fread(Block_Bitmap, sizeof(Block_Bitmap), 1, fr);

	return true;



};	
	
void Login()// �û���¼

{

};		

void cmd(string args);
