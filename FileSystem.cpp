#pragma once
#include "XX_filesystem.h"

FileSystem::FileSystem()
{
	dirGroup.clear();
	fr = NULL;
	fw = NULL;
	isLogin = false;
	doFormat = false;
	cur_dir = 0;
}

//void FileSystem::f_read(FILE* fpr)
//{
//	fr = fpr;
//}
//
//void FileSystem::f_write(FILE* fpw)
//{
//	fw = fpw;
//}

bool FileSystem::readSysFile()
{
	if (fr == NULL) { // 没有以读方式打开文件
		cout << "文件未打开，请先打开文件 ..." << endl;
		return false;
	}
	else { // 文件已打开
		// 把磁盘文件读入disk
		disk.disk_read(fr);
	}
	return true;
}

bool FileSystem::writeSysFile()
{
	if (fw == NULL) { // 没有以写方式打开文件
		cout << "文件未打开，请先打开文件 ..." << endl;
		return false;
	}
	else { // 文件已打开
		// 把disk数据写入磁盘文件
		disk.disk_write(fw);
	}
	return true;
}

bool FileSystem::openSysFile()
{
	// 读方式打开虚拟磁盘文件
	if ((fr = fopen(FILESYSTEMNAME, "rb")) == NULL) {
		// 虚拟磁盘文件不存在

		// 创建虚拟磁盘文件
		if ((fw = fopen(FILESYSTEMNAME, "wb")) == NULL) {
			// 虚拟磁盘文件创建失败
			cout << "虚拟磁盘文件创建失败..." << endl;
			return false;
		}
		// 虚拟文件创建成功，重新打开文件
		fr = fopen(FILESYSTEMNAME, "rb");

		doFormat = true; // 创建新磁盘文件必须格式化
	}
	else { // 虚拟磁盘文件存在
		// 把磁盘文件数据读入disk
		readSysFile();
		// 写方式打开文件
		if ((fw = fopen(FILESYSTEMNAME, "wb")) == NULL) {
			// 写方式打开文件失败
			cout << " 虚拟磁盘文件打开失败..." << endl;
			return false;
		}
		// 写方式打开文件成功，把disk数据写回磁盘文件 （写方式打开文件会清空文件）
		writeSysFile();
	}
	return true;
}

void FileSystem::closeSysFile()
{
	fclose(fr);
	fclose(fw);
	fr = NULL;
	fw = NULL;
}

bool FileSystem::formatSysFile()
{
	if (doFormat) {
		disk.disk_format(); // 格式化disk
		return writeSysFile(); // 把格式化的disk写入磁盘文件
	}
	return true;
}

void FileSystem::readDirGroup()
{
	Dentry dentry;
	Directory dir = disk.dir_read(0);
	dir.setName("root");
	dir.setI_Index(0);
	dir.setParentDir(NULL);
	dirGroup.push_back(dir);
	int j = 0;
	while (1) {
		for (int i = 0; i < dirGroup[j].getDentryNum(); i++) {
			dentry = dirGroup[j].getDentry(i);
			if (dentry.getMode() == DIR_MODE) {
				int i_index = dentry.getIndex();
				dir = disk.dir_read(i_index);
				dir.setName(dentry.getName().c_str());
				dir.setI_Index(i_index);
				dir.setParentDir(j);
				dirGroup.push_back(dir);
			}
		}
		j++;
		if (j == dirGroup.size()) {
			break;
		}
	}
}

void FileSystem::writeDirGroup()
{
	Dentry dentry;
	disk.dir_write(0, dirGroup[0]);
	int k;
	int d_num;
	int dir_size = dirGroup.size();
	int d_index;
	for (int j = 0; j < dir_size; j++) {
		d_num = dirGroup[j].getDentryNum();
		for (int i = 0; i < d_num; i++) {
			dentry = dirGroup[j].getDentry(i);
			d_index = dentry.getIndex();
			if (dentry.getMode() == DIR_MODE) {
				for (k = 0; k < dir_size; k++) {
					if (d_index == dirGroup[k].getI_Index()) {
						break;
					}
				}
				disk.dir_write(d_index, dirGroup[k]);
			}
		}
	}
}


void FileSystem::ls()
{
	dirGroup[cur_dir].printDir();
}

void FileSystem::cd(int cur)
{
	if (cur == cur_dir) {
		cout << "已在该目录下" << endl;
	}
	else if (cur < dirGroup.size() && cur >= 0) {
		cur_dir = cur;
	}
	else {
		cout << "没有找到该目录" << endl;
	}
}

void FileSystem::touch(const char* name)
{
	// 申请Inode，Block
	int i_index = disk.d_ialloc();
	int b_index = disk.d_balloc();
	if (i_index == -1 || b_index == -1) {
		cout << "磁盘空间不够，无法创建文件" << endl;
		return;
	}	
	
	// 在当前目录添加相应的目录项
	Dentry f_dentry(i_index, FILE_MODE, 0, name);
	dirGroup[cur_dir].add_Dentry(f_dentry);

	// 在Inode中，目录文件大小要更新
	// 更新Superblock；更新位图；更新Inode表（包括文件Inode的更新，和当前目录对应的Inode的更新）
	disk.use_renew(b_index, i_index, FILE_MODE, 0, dirGroup[cur_dir]);
	
	// 将当前目录写回数据块
	disk.dir_write(dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir]);
}

void FileSystem::rm_f()
{
	// 在当前目录删除相应的目录项
	// 更新Superblock；更新位图；更新Inode表（包括文件Inode的更新，和当前目录对应的Inode的更新）
	// 将当前目录写回数据块
}

void FileSystem::mkdir(const char* name)
{
	// 申请Inode，Block
	int i_index = disk.d_ialloc();
	int b_index = disk.d_balloc();
	if (i_index == -1 || b_index == -1) {
		cout << "磁盘空间不够，无法创建文件" << endl;
		return;
	}

	// 在当前目录添加相应的目录项
	Directory dir;
	dir.setName(name);
	dir.setI_Index(i_index);
	dir.setParentDir(cur_dir);

	dirGroup.push_back(dir);

	int dir_size = dir.getDirSize();

	Dentry dir_dentry(i_index, DIR_MODE, dir_size, name);
	dirGroup[cur_dir].add_Dentry(dir_dentry);

	// 在Inode中，目录文件大小要更新
	// 更新Superblock；更新位图；更新Inode表（包括文件Inode的更新，和当前目录对应的Inode的更新）
	disk.use_renew(b_index, i_index, FILE_MODE, dir_size, dirGroup[cur_dir]);

	// 将当前目录写回数据块
	disk.dir_write(dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir]);
	disk.dir_write(i_index, dir);
}

void FileSystem::rm_rf()
{
	// 注意：删除目录要把该目录下所有文件都删除（可能用到递归函数）
}

void FileSystem::needFormat(bool doFormat)
{
	this->doFormat = doFormat;
}

bool FileSystem::isFormat()
{
	return doFormat;
}
