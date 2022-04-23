
#include "Disk.h"

Block::Block(int No)
{
	this->No = No;
}

void Block::printInfo()
{
	cout << "当前Block块号为" << No << endl;
}

int Block::getNO()
{
	return No;
}

void Block::block_read(FILE* fr, char buf[])
{
	fseek(fr, No * BLOCK_SIZE, SEEK_SET);
	fread(buf, BLOCK_SIZE, 1, fr);
}

void Block::block_write(FILE* fw, char buf[])
{
	fseek(fw, No * BLOCK_SIZE, SEEK_SET);
	fwrite(buf, BLOCK_SIZE, 1, fw);
}

Superblock::Superblock(int free_INum, int free_BNum) :Block(0)
{
	free_Inode_Num = free_INum;
	free_Block_Num = free_BNum;
}

Superblock::Superblock() :Block(0)
{
	free_Inode_Num = INODE_NUM;
	free_Block_Num = Block_Num;
}

void Superblock::printInfo()
{
	cout << "Superblock信息：" << endl;
	cout << "	Inode总数  ：" << s_Inode_Num << endl;
	cout << "	空闲Inode数：" << free_Inode_Num << endl;
	cout << "	Block总数  ：" << s_Block_Num << endl;
	cout << "	空闲Block数：" << free_Inode_Num << endl;
}

Block_Bitmap::Block_Bitmap(bool isUsed[Block_Num]): Block(1)
{
	for (int i = 0; i < Block_Num; i++) {
		b_isUsed[i] = isUsed[i];
	}
}

Block_Bitmap::Block_Bitmap(): Block(1)
{
	for (int i = 0; i < Block_Num; i++) {
		b_isUsed[i] = 0;
	}
}

void Block_Bitmap::printInfo()
{
	cout << "块位图信息：" << endl;
	int used_num = 0;
	int first_free = -1;
	for (int i = 0; i < Block_Num; i++) {
		if (!b_isUsed[i]) {
			first_free = i;
			break;
		}
	}
	for (int i = 0; i < Block_Num; i++) {
		if (b_isUsed[i]) {
			used_num++;
		}
	}
	cout << "	Block使用情况：" << used_num << " / " << Block_Num << endl;
	if (first_free == -1) {
		cout << "	空闲Block情况：无空闲Block ..." << endl;
	}
	else {
		cout << "	空闲Block情况：Block[ " << first_free << " ] ..." << endl;
	}
}

Inode_Bitmap::Inode_Bitmap(bool isUsed[INODE_NUM]): Block(2)
{
	for (int i = 0; i < INODE_NUM; i++) {
		i_isUsed[i] = isUsed[i];
	}
}

Inode_Bitmap::Inode_Bitmap(): Block(2)
{
	for (int i = 0; i < INODE_NUM; i++) {
		i_isUsed[i] = 0;
	}
}

void Inode_Bitmap::printInfo()
{
	cout << "Inode位图信息：" << endl;
	int used_num = 0;
	int first_free = -1;
	for (int i = 0; i < INODE_NUM; i++) {
		if (!i_isUsed[i]) {
			first_free = i;
			break;
		}
	}
	for (int i = 0; i < INODE_NUM; i++) {
		if (i_isUsed[i]) {
			used_num++;
		}
	}
	cout << "	Inode使用情况：" << used_num << " / " << INODE_NUM << endl;
	if (first_free == -1) {
		cout << "	空闲Inode情况：无空闲Inode ..." << endl;
	}
	else {
		cout << "	空闲Inode情况：Inode[ " << first_free << " ] ..." << endl;
	}
}

Inode::Inode(int No, int mode, int size, int time, int index[BLOCK_INDEX])
{
	i_No = No;
	f_mode = mode;
	f_size = size;
	c_time = time;
	for (int i = 0; i < BLOCK_INDEX; i++) {
		block_index[i] = index[i];
	}
}

Inode::Inode(int No, int mode, int size, int time)
{
	i_No = No;
	f_mode = mode;
	f_size = size;
	c_time = time;
	for (int i = 0; i < BLOCK_INDEX; i++) {
		block_index[i] = -1;
	}
}

void Inode::printInfo()
{
	cout << "Inode[ " << i_No << " ]：" << endl;
	cout << "	文件类型：" <<
		((f_mode) ? "目录文件类型" : "普通文件类型") << endl;
	cout << "	文件大小：" << f_size << " B" << endl;
	cout << "	创建时间：" << c_time << endl;
}
