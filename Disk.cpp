#pragma once
#include "XX_filesystem.h"

Block::Block(int No)
{
	this->No = No;
}

//int Block::getNO()
//{
//	return No;
//}

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

//void DataBlock::buffer_write(char buf[Block_Num * BLOCK_SIZE])
//{
//	for (int i = 0; i < Block_Num * BLOCK_SIZE; i++) {
//		buffer[i] = buf[i];
//	}
//}

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

void Superblock::use_renew()
{
	free_Block_Num--;
	free_Inode_Num--;
}

void Superblock::use_renew(int blocks_num)
{
	free_Block_Num -= blocks_num;
}

void Superblock::free_renew(int b_cout)
{
	free_Block_Num += b_cout;
	free_Inode_Num++;
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

int Block_Bitmap::balloc()
{
	for (int i = 0; i < Block_Num; i++) {
		if (b_isUsed[i] == 0) {
			return i;
		}
	}
	return -1;
}

void Block_Bitmap::use_renew(int b_index)
{
	b_isUsed[b_index] = 1;
}

void Block_Bitmap::free_renew(int b_index)
{
	b_isUsed[b_index] = 0;
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

int Inode_Bitmap::ialloc()
{
	for (int i = 0; i < INODE_NUM; i++) {
		if (i_isUsed[i] == 0) {
			return i;
		}
	}
	return -1;
}

void Inode_Bitmap::use_renew(int i_index)
{
	i_isUsed[i_index] = 1;
}

void Inode_Bitmap::free_renew(int i_index)
{
	i_isUsed[i_index] = 0;
}

Inode::Inode(int No, int mode, int size, int index[BLOCK_INDEX])
{
	i_No = No;
	f_mode = mode;
	f_size = size;
	for (int i = 0; i < BLOCK_INDEX; i++) {
		block_index[i] = index[i];
	}
}

Inode::Inode(int No)
{
	i_No = No;
	f_mode = -1;
	f_size = -1;
	for (int i = 0; i < BLOCK_INDEX; i++) {
		block_index[i] = -1;
	}
}

Inode::Inode()
{
	i_No = -1;
	f_mode = -1;
	f_size = -1;
	for (int i = 0; i < BLOCK_INDEX; i++) {
		block_index[i] = -1;
	}
}

void Inode::format()
{
	f_mode = -1;
	f_size = -1;
	for (int i = 0; i < BLOCK_INDEX; i++) {
		block_index[i] = -1;
	}
}

void Inode::printInfo()
{
	cout << "Inode[ " << i_No << " ]：" << endl;
	if (f_mode == FILE_MODE) {
		cout << "	文件类型：普通文件类型" << endl;
	}
	else if (f_mode == DIR_MODE) {
		cout << "	文件类型：目录文件类型" << endl;
	}
	else {
		cout<< "	文件类型：其他文件类型" << endl;
	}
	cout << "	文件大小：" << f_size << " B" << endl;
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

//int Inode::getC_Time()
//{
//	return c_time;
//}

//void Inode::setC_Time(int time)
//{
//	c_time = time;
//}

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
	if (f_size == 0) {
		block_num = 1;
	}
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
	//inode[0].setC_Time(0); // TODO
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

void Inode_Label::use_renew(int i_index, int b_index, int mode, int f_size, int cur_dir_i_index, int cur_dir_size)
{
	// 更新文件Inode
	//inode[i_index].setC_Time(0); // TODO
	inode[i_index].setF_Mode(mode);
	inode[i_index].setF_Size(f_size);
	inode[i_index].addBlock(b_index); // 新申请的Inode一定是空的

	// 更新目录文件大小
	inode[cur_dir_i_index].setF_Size(cur_dir_size);
}

void Inode_Label::free_renew(int i_index, int cur_dir_i_index, int cur_dir_size)
{
	// 更新文件Inode
	inode[i_index].format();
	// 更新目录文件大小
	inode[cur_dir_i_index].setF_Size(cur_dir_size);
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

	Dentry cur_dir(0, OTHER_MODE, 0, "."); // 暂时设置为一般文件类型，作测试文件
	Dentry par_dir(0, OTHER_MODE, 0, ".."); // 暂时设置为一般文件类型，作测试文件

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

int Disk::d_balloc()
{
	return b_bmap.balloc();
}

int Disk::d_ialloc()
{
	return i_bmap.ialloc();
}

char* Disk::file_read(int i_index)
{
	Inode* inode = i_label.getInode(i_index);
	int fsize = inode->getF_Size();
	char* buffer = inode->getFile(d_block.buffer_return());
	buffer[fsize] = '\0';
	return buffer;
}

int Disk::file_size(int i_index)
{
	return i_label.getInode(i_index)->getF_Size();
}

bool Disk::file_write(int i_index, const char* buf)
{
	Inode* inode = i_label.getInode(i_index);
	int fsize = inode->getF_Size();
	int blocks = (fsize + (int)strlen(buf)) / BLOCK_SIZE - fsize / BLOCK_SIZE;
	vector<int> b_index;
	b_index.clear();
	if (blocks > 0) { // 需要申请新的block
		if (fsize / BLOCK_SIZE + blocks > BLOCK_INDEX) {
			cout << "文件过大，无法写入文件" << endl;
			return false;
		}
		for (int i = 0; i < blocks; i++) {
			b_index.push_back(b_bmap.balloc());
			if (b_index[i] == -1) {
				cout << "磁盘空间已满，无法写入文件" << endl;
				return false;
			}
		}// 申请到所有需要的blocks
		for (int i = 0; i < blocks; i++) {
			b_bmap.use_renew(b_index[i]);
			spb.use_renew(blocks);
			inode->addBlock(b_index[i]);
		}
	}
	inode->setF_Size(fsize + strlen(buf));
	char* buffer = inode->getFile(d_block.buffer_return()); // 用到了getFile函数，要注意delete
	memcpy(buffer + fsize, buf, strlen(buf) + 1);
	for (int i = 0; i < BLOCK_INDEX; i++) {
		int i_index = inode->getIndex(i);
		if (i_index != -1) {
			int b_address = i_index * BLOCK_SIZE;
			memcpy(d_block.buffer_return() + b_address, buffer + i * BLOCK_SIZE, BLOCK_SIZE);
		}
	}
	delete[] buffer;
	return true;
}


Dentry Disk::dentry_read(int dentry_address)
{
	char* buf = d_block.buffer_return();
	int size = *((int*)(buf + dentry_address));
	int index = *((int*)(buf + dentry_address + sizeof(size)));
	int mode = *((int*)(buf + dentry_address + sizeof(size) + sizeof(index)));
	int f_size = *((int*)(buf + dentry_address + sizeof(size) + sizeof(index) + sizeof(mode)));
	char* name = buf + dentry_address + sizeof(size) + sizeof(index) + sizeof(mode) + sizeof(f_size);
	return Dentry(index, mode, f_size, name);
}

Dentry Disk::dentry_read(int dentry_address, char* buf)
{
	int size = *((int*)(buf + dentry_address));
	int index = *((int*)(buf + dentry_address + sizeof(size)));
	int mode = *((int*)(buf + dentry_address + sizeof(size) + sizeof(index)));
	int f_size = *((int*)(buf + dentry_address + sizeof(size) + sizeof(index) + sizeof(mode)));
	char* name = buf + dentry_address + sizeof(size) + sizeof(index) + sizeof(mode) + sizeof(f_size);
	return Dentry(index, mode, f_size, name);
}

void Disk::dentry_write(int dentry_address, Dentry dentry)
{
	char* buf = d_block.buffer_return();
	int d_size = dentry.getSize();
	int index = dentry.getIndex();
	int mode = dentry.getMode();
	int f_size = dentry.getF_Size();
	*((int*)(buf + dentry_address)) = d_size;
	*((int*)(buf + dentry_address + sizeof(d_size))) = index;
	*((int*)(buf + dentry_address + sizeof(d_size) + sizeof(index))) = mode;
	*((int*)(buf + dentry_address + sizeof(d_size) + sizeof(index) + sizeof(mode))) = f_size;
	char* str_p = buf + dentry_address + sizeof(d_size) + sizeof(index) + sizeof(mode) + sizeof(f_size);
	strcpy(str_p, dentry.getName().c_str());
}

void Disk::dentry_write(int dentry_address, char* buf, Dentry dentry)
{
	int d_size = dentry.getSize();
	int index = dentry.getIndex();
	int mode = dentry.getMode();
	int f_size = dentry.getF_Size();
	*((int*)(buf + dentry_address)) = d_size;
	*((int*)(buf + dentry_address + sizeof(d_size))) = index;
	*((int*)(buf + dentry_address + sizeof(d_size) + sizeof(index))) = mode;
	*((int*)(buf + dentry_address + sizeof(d_size) + sizeof(index) + sizeof(mode))) = f_size;
	char* str_p = buf + dentry_address + sizeof(d_size) + sizeof(index) + sizeof(mode) + sizeof(f_size);
	strcpy(str_p, dentry.getName().c_str());
}

Directory Disk::dir_read(int i_No)
{
	// 默认传入的Inode对应目录文件，判断在传入前进行
	Inode* inode = i_label.getInode(i_No);
	char* dir_buf = inode->getFile(d_block.buffer_return()); // 用到了getFile函数，要注意delete
	Directory dir;
	dir.dir_clear();
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
			int b_address = i_index * BLOCK_SIZE;
			memcpy(d_block.buffer_return() + b_address, dir_buf + i * BLOCK_SIZE, BLOCK_SIZE);
		}
	}
}

void Disk::use_renew(int b_index, int i_index, int mode, int f_size, int cur_dir_i_index, int cur_dir_size)
{
	spb.use_renew();
	b_bmap.use_renew(b_index);
	i_bmap.use_renew(i_index);
	i_label.use_renew(i_index, b_index, mode, f_size, cur_dir_i_index, cur_dir_size);
}

void Disk::free_renew(int i_index, int cur_dir_i_index, int cur_dir_size)
{
	Inode* inode = i_label.getInode(i_index);
	int b_index = -1;
	int b_count = 0;
	for (int i = 0; i < BLOCK_INDEX; i++) {
		b_index = inode->getIndex(i);
		if (b_index != -1) {
			b_bmap.free_renew(b_index);
			b_count++;
		}
	}
	spb.free_renew(b_count);
	i_bmap.free_renew(i_index);
	i_label.free_renew(i_index, cur_dir_i_index, cur_dir_size);
}

