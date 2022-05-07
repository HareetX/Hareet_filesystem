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
	buffer = new char[Block_Num * BLOCK_SIZE];
}

DataBlock::~DataBlock()
{
	delete[] buffer;
}

void DataBlock::format()
{
	for (int i = 0; i < Block_Num * BLOCK_SIZE; i++) {
		buffer[i] = 0;
	}
	
	/*Dentry cur_dir(0, DIR_MODE, ".");
	Dentry par_dir(0, DIR_MODE, "..");

	*((size_t*)buffer) = 2;
	dentry_write(sizeof(size_t), cur_dir);
	dentry_write(sizeof(size_t) + cur_dir.getSize(), par_dir);*/

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

void DataBlock::block_read(FILE* fpr)
{
	fseek(fpr, Block_Address, SEEK_SET);
	fread(buffer, static_cast<size_t>(Block_Num) * BLOCK_SIZE, 1, fpr);
}

void DataBlock::block_write(FILE* fpw)
{
	fseek(fpw, Block_Address, SEEK_SET);
	fwrite(buffer, static_cast<size_t>(Block_Num) * BLOCK_SIZE, 1, fpw);
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
	free_Inode_Num = INODE_NUM - 1; // 第0号Inode作为root目录文件的Inode
	free_Block_Num = Block_Num - 1; // 第0号Block作为root目录文件
}

void Superblock::printInfo()
{
	cout << "Superblock信息：" << endl;
	cout << "	Inode总数  ：" << s_Inode_Num << endl;
	cout << "	空闲Inode数：" << free_Inode_Num << endl;
	cout << "	Block总数  ：" << s_Block_Num << endl;
	cout << "	空闲Block数：" << free_Inode_Num << endl;
}

void Superblock::block_read(FILE* fpr)
{
	fseek(fpr, Super_Block_Address, SEEK_SET);
	fread(&free_Inode_Num, sizeof(int), 1, fpr);
	fseek(fpr, Super_Block_Address + sizeof(int), SEEK_SET);
	fread(&free_Block_Num, sizeof(int), 1, fpr);
}

void Superblock::block_write(FILE* fpw)
{
	fseek(fpw, Super_Block_Address, SEEK_SET);
	fwrite(&free_Inode_Num, sizeof(int), 1, fpw);
	fseek(fpw, Super_Block_Address + sizeof(int), SEEK_SET);
	fwrite(&free_Block_Num, sizeof(int), 1, fpw);
}

Block_Bitmap::Block_Bitmap(bool isUsed[Block_Num]): Block(1)
{
	for (int i = 0; i < Block_Num; i++) {
		b_isUsed[i] = isUsed[i];
	}
}

Block_Bitmap::Block_Bitmap(): Block(BLOCKS_PER_SUPERBLOCK)
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
	b_isUsed[0] = 1;
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

void Block_Bitmap::block_read(FILE* fpr)
{
	fseek(fpr, Block_Bitmap_Address, SEEK_SET);
	fread(b_isUsed, Block_Num, 1, fpr);
}

void Block_Bitmap::block_write(FILE* fpw)
{
	fseek(fpw, Block_Bitmap_Address, SEEK_SET);
	fwrite(b_isUsed, Block_Num, 1, fpw);
}


Inode_Bitmap::Inode_Bitmap(bool isUsed[INODE_NUM]): Block(BLOCKS_PER_SUPERBLOCK + BLOCKS_PER_BBITMAP)
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
	i_isUsed[0] = 1;
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

void Inode_Bitmap::block_read(FILE* fpr)
{
	fseek(fpr, Inode_Bitmap_Address, SEEK_SET);
	fread(i_isUsed, INODE_NUM, 1, fpr);
}

void Inode_Bitmap::block_write(FILE* fpw)
{
	fseek(fpw, Inode_Bitmap_Address, SEEK_SET);
	fwrite(i_isUsed, INODE_NUM, 1, fpw);
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

int Inode::getF_Mode()
{
	return f_mode;
}

void Inode::setF_Mode(int mode)
{
	f_mode = mode;
}

int Inode::getF_Size()
{
	return f_size;
}

void Inode::setF_Size(int size)
{
	f_size = size;
}

int Inode::getC_Time()
{
	return c_time;
}

void Inode::setC_Time(int time)
{
	c_time = time;
}

bool Inode::addBlock(int index)
{
	int i;
	for (i = 0; i < BLOCK_INDEX; i++) {
		if (block_index[i] == -1) { break; }
	}
	if (i == BLOCK_INDEX) {
		return false;
	}
	else {
		block_index[i] = index;
		return true;
	}
}

char* Inode::getFile(char* buffer)
{
	int block_num = f_size / BLOCK_SIZE + ((f_size % BLOCK_SIZE) ? 1 : 0);
	char* buf = new char[BLOCK_INDEX * BLOCK_SIZE]; // new的数组需要delete[]释放！！！！！
	for (int i = 0; i < BLOCK_INDEX * BLOCK_SIZE; i++) {
		buf[i] = 0;
	}
	for (int i = 0; i < block_num; i++) {
		memcpy(buf + i * BLOCK_SIZE, buffer + block_index[i] * BLOCK_SIZE, BLOCK_SIZE);
	}

	return buf;
}

int Inode::getIndex(int No)
{
	return block_index[No];
}

Inode_Label::Inode_Label(): Block(BLOCKS_PER_SUPERBLOCK + BLOCKS_PER_BBITMAP + BLOCKS_PER_IBITMAP)
{
	//inode = new Inode[INODE_NUM];
	for (int i = 0; i < INODE_NUM; i++) {
		inode[i].setI_No(i);
	}
}

//Inode_Label::~Inode_Label()
//{
//	delete inode;
//}

void Inode_Label::format()
{
	for (int i = 0; i < INODE_NUM; i++) {
		inode[i].format();
	}
	inode[0].setF_Mode(DIR_MODE);
	inode[0].setF_Size(0); // TODO
	inode[0].setC_Time(0); // TODO
	inode[0].addBlock(0);
}

void Inode_Label::printInfo()
{
	cout << "TODO" << endl;
}

void Inode_Label::block_read(FILE* fpr)
{
	for (int i = 0; i < INODE_NUM; i++) {
		fseek(fpr, Inode_Label_Address + i * INODE_SIZE, SEEK_SET);
		fread(inode + i, sizeof(Inode), 1, fpr);
	}
}

void Inode_Label::block_write(FILE* fpw)
{
	for (int i = 0; i < INODE_NUM; i++) {
		fseek(fpw, Inode_Label_Address + i * INODE_SIZE, SEEK_SET);
		fwrite(inode + i, sizeof(Inode), 1, fpw);
	}
}

Inode* Inode_Label::getInode(int No)
{
	return &inode[No];
}


void Disk::format(Block& b)
{
	b.format();
}

void Disk::disk_format()
{
	format(spb);
	format(b_bmap);
	format(i_bmap);
	format(i_label);
	format(d_block);

	Dentry cur_dir(0, FILE_MODE, "."); // 暂时设置为一般文件类型，作测试文件
	Dentry par_dir(0, FILE_MODE, ".."); // 暂时设置为一般文件类型，作测试文件

	*((size_t*)d_block.buffer_return()) = 2;
	dentry_write(sizeof(size_t), cur_dir);
	dentry_write(sizeof(size_t) + cur_dir.getSize(), par_dir);
	int root_size = cur_dir.getSize() + par_dir.getSize();
	i_label.getInode(0)->setF_Size(root_size);
}

void Disk::block_read(Block& b, FILE* fpr)
{
	b.block_read(fpr);
}

void Disk::disk_read(FILE* fpr)
{
	block_read(spb, fpr);
	block_read(b_bmap, fpr);
	block_read(i_bmap, fpr);
	block_read(i_label, fpr);
	block_read(d_block, fpr);
}


void Disk::block_write(Block& b, FILE* fpw)
{
	b.block_write(fpw);
}

void Disk::disk_write(FILE* fpw)
{
	block_write(spb, fpw);
	block_write(b_bmap, fpw);
	block_write(i_bmap, fpw);
	block_write(i_label, fpw);
	block_write(d_block, fpw);
}


Dentry Disk::dentry_read(int dentry_address)
{
	int size = *((int*)(d_block.buffer_return() + dentry_address));
	int index = *((int*)(d_block.buffer_return() + dentry_address + sizeof(size)));
	int mode = *((int*)(d_block.buffer_return() + dentry_address + sizeof(size) + sizeof(index)));
	char* name = d_block.buffer_return() + dentry_address + sizeof(size) + sizeof(index) + sizeof(mode);
	return Dentry(index, mode, name);
}

Dentry Disk::dentry_read(int dentry_address, char* buf)
{
	int size = *((int*)(buf + dentry_address));
	int index = *((int*)(buf + dentry_address + sizeof(size)));
	int mode = *((int*)(buf + dentry_address + sizeof(size) + sizeof(index)));
	char* name = buf + dentry_address + sizeof(size) + sizeof(index) + sizeof(mode);
	return Dentry(index, mode, name);
}

void Disk::dentry_write(int dentry_address, Dentry dentry)
{
	*((int*)(d_block.buffer_return() + dentry_address)) = dentry.getSize();
	*((int*)(d_block.buffer_return() + dentry_address + sizeof(dentry.getSize()))) = dentry.getIndex();
	*((int*)(d_block.buffer_return() + dentry_address + sizeof(dentry.getSize()) + sizeof(dentry.getIndex()))) = dentry.getMode();
	char* str_p = d_block.buffer_return() + dentry_address + sizeof(dentry.getSize()) + sizeof(dentry.getIndex()) + sizeof(dentry.getMode());
	strcpy(str_p, dentry.getName().c_str());
}

void Disk::dentry_write(int dentry_address, char* buf, Dentry dentry)
{
	*((int*)(buf + dentry_address)) = dentry.getSize();
	*((int*)(buf + dentry_address + sizeof(dentry.getSize()))) = dentry.getIndex();
	*((int*)(buf + dentry_address + sizeof(dentry.getSize()) + sizeof(dentry.getIndex()))) = dentry.getMode();
	char* str_p = buf + dentry_address + sizeof(dentry.getSize()) + sizeof(dentry.getIndex()) + sizeof(dentry.getMode());
	strcpy(str_p, dentry.getName().c_str());
}

Directory Disk::dir_read(int i_No)
{
	// 默认传入的Inode对应目录文件，判断在传入前进行
	Inode* inode = i_label.getInode(i_No);
	char* dir_buf = inode->getFile(d_block.buffer_return()); // 用到了getFile函数，要注意delete
	Directory dir;
	size_t d_num = *((size_t*)dir_buf);
	int d_address = sizeof(size_t);
	int d_size = 0;
	for (int i = 0; i < d_num; i++) {
		d_address += d_size;
		Dentry dentry = dentry_read(d_address, dir_buf);
		dir.add_Dentry(dentry);
		d_size = dentry.getSize();
	}
	dir.renewDentryNum();
	delete[] dir_buf;
	return dir; // 只初始化了dir的dentry_num和dentryGroup
}

void Disk::dir_write(int i_No, Directory dir)
{
	Inode* inode = i_label.getInode(i_No);
	char dir_buf[BLOCK_INDEX * BLOCK_SIZE] = { 0 };
	*((size_t*)dir_buf) = dir.getDentryNum();
	size_t d_num = dir.getDentryNum();
	int d_address = sizeof(size_t);
	int d_size = 0;
	for (int i = 0; i < d_num; i++) {
		d_address += d_size;
		Dentry dentry = dir.getDentry(i);
		dentry_write(d_address, dir_buf, dentry);
		d_size = dentry.getSize();
	}
	for (int i = 0; i < BLOCK_INDEX; i++) {
		int i_index = inode->getIndex(i);
		if (i_index != -1) {
			int b_address = inode->getIndex(i) * BLOCK_SIZE;
			memcpy(d_block.buffer_return() + b_address, dir_buf + i * BLOCK_SIZE, BLOCK_SIZE);
		}
	}
}

