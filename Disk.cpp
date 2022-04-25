#pragma once
#include "XX_filesystem.h"

Block::Block(int No)
{
	this->No = No;
}

int Block::getNO()
{
	return No;
}

DataBlock::DataBlock():
	Block(BLOCKS_PER_SUPERBLOCK + BLOCKS_PER_BBITMAP + BLOCKS_PER_IBITMAP + BLOCKS_PER_ILABEL)
{
	buffer = new char[Block_Num * BLOCK_SIZE]();
}

DataBlock::~DataBlock()
{
	delete buffer;
}

void DataBlock::format()
{
	for (int i = 0; i < Block_Num * BLOCK_SIZE; i++) {
		buffer[i] = 0;
	}
}

void DataBlock::printInfo()
{
}

char* DataBlock::buffer_return()
{
	return buffer;
}

void DataBlock::buffer_write(char buf[Block_Num * BLOCK_SIZE])
{
	for (int i = 0; i < Block_Num * BLOCK_SIZE; i++) {
		buffer[i] = buf[i];
	}
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

void Superblock::format()
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

void Block_Bitmap::format()
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

Inode_Bitmap::Inode_Bitmap(bool isUsed[INODE_NUM]): Block(4)
{
	for (int i = 0; i < INODE_NUM; i++) {
		i_isUsed[i] = isUsed[i];
	}
}

Inode_Bitmap::Inode_Bitmap(): Block(4)
{
	for (int i = 0; i < INODE_NUM; i++) {
		i_isUsed[i] = 0;
	}
}

void Inode_Bitmap::format()
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

Inode::Inode(int No, int mode, int size, int time_c, int index[BLOCK_INDEX])
{
	i_No = No;
	f_mode = mode;
	f_size = size;
	c_time = time_c;
	for (int i = 0; i < BLOCK_INDEX; i++) {
		block_index[i] = index[i];
	}
}

Inode::Inode(int No)
{
	i_No = No;
	f_mode = -1;
	f_size = -1;
	c_time = -1;
	for (int i = 0; i < BLOCK_INDEX; i++) {
		block_index[i] = -1;
	}
}

Inode::Inode()
{
	i_No = -1;
	f_mode = -1;
	f_size = -1;
	c_time = -1;
	for (int i = 0; i < BLOCK_INDEX; i++) {
		block_index[i] = -1;
	}
}

void Inode::format()
{
	f_mode = -1;
	f_size = -1;
	c_time = -1;
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

int Inode::getI_No()
{
	return i_No;
}

void Inode::setI_No(int No)
{
	i_No = No;
}

Inode_Label::Inode_Label(): Block(5)
{
	for (int i = 0; i < INODE_NUM; i++) {
		inode[i].setI_No(i);
	}
}

void Inode_Label::format()
{
	for (int i = 0; i < INODE_NUM; i++) {
		inode[i].format();
	}
}

void Inode_Label::printInfo()
{
}


void Disk::format()
{
	spb.format();
	b_bmap.format();
	i_bmap.format();
	i_label.format();
	d_block.format();
}

void Disk::spb_read(FILE *fr)
{
	fseek(fr, Super_Block_Address, SEEK_SET);
	fread(&spb, sizeof(Superblock), 1, fr);
}

void Disk::b_bmap_read(FILE *fr)
{
	fseek(fr, Block_Bitmap_Address, SEEK_SET);
	fread(&b_bmap, sizeof(Block_Bitmap), 1, fr);
}

void Disk::i_bmap_read(FILE* fr)
{
	fseek(fr, Inode_Bitmap_Address, SEEK_SET);
	fread(&i_bmap, sizeof(Inode_Bitmap), 1, fr);
}

void Disk::inode_read(FILE* fr)
{
	fseek(fr, Inode_Label_Address, SEEK_SET);
	fread(&i_label, sizeof(Inode_Label), 1, fr);
}

void Disk::d_block_read(FILE* fr)
{
	fseek(fr, Block_Address, SEEK_SET);
	fread(d_block.buffer_return(), static_cast<size_t>(Block_Num) * BLOCK_SIZE, 1, fr);
}

void Disk::spb_write(FILE* fw)
{
	fseek(fw, Super_Block_Address, SEEK_SET);
	fwrite(&spb, sizeof(Superblock), 1, fw);
}

void Disk::b_bmap_write(FILE* fw)
{
	fseek(fw, Block_Bitmap_Address, SEEK_SET);
	fwrite(&b_bmap, sizeof(Block_Bitmap), 1, fw);
}

void Disk::i_bmap_write(FILE* fw)
{
	fseek(fw, Inode_Bitmap_Address, SEEK_SET);
	fwrite(&i_bmap, sizeof(Inode_Bitmap), 1, fw);
}

void Disk::inode_write(FILE* fw)
{
	fseek(fw, Inode_Label_Address, SEEK_SET);
	fread(&i_label, sizeof(Inode_Label), 1, fw);
}

void Disk::d_block_write(FILE* fw)
{
	fseek(fw, Block_Address, SEEK_SET);
	fwrite(d_block.buffer_return(), static_cast<size_t>(Block_Num) * BLOCK_SIZE, 1, fw);
}
