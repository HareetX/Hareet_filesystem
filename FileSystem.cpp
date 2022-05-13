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

bool FileSystem::is_Login()
{
	return isLogin;
}

void FileSystem::Login()	//登录界面
{
	// 只计划做一名用户为root
	cout << "本文件系统只有root用户，请输入用户名root以及密码root登录:" << endl;
	char username[100] = { 0 };
	char passwd[100] = { 0 };
	inUsername(username);	//输入用户名
	inPasswd(passwd);		//输入用户密码
	if (check(username, passwd)) {	//核对用户名和密码
		isLogin = true;
	}
	else {
		isLogin = false;
	}
}

void inUsername(string username)	//输入用户名
{
	cout << "username:" << endl;
	scanf("%s", username);	//用户名
}

void inPasswd(string passwd)	//输入密码
{
	int pwlen = 0;
	char c;
	fflush(stdin);	//清空缓冲区
	cout << "passwd:" << endl;
	while (c = getch()) {
		if (c == '\r') {	//输入回车，密码确定
			passwd[pwlen] = '\0';
			fflush(stdin);	//清缓冲区
			printf("\n");
			break;
		}
		else if (c == '\b') {	//退格，删除一个字符
			if (pwlen != 0) {	//没有删到头
				pwlen--;
			}
		}
		else {	//密码字符
			passwd[pwlen++] = c;
		}
	}
}

bool check(string username, string passwd) //核对用户名，密码
{
	if (username == "root" & passwd == "root") {
		return 1;
	}
	else
	{
		return 0;
	}
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

void FileSystem::rm_f(int i_index)
{
	// 在当前目录删除相应的目录项
	int i;
	int d_num = dirGroup[cur_dir].getDentryNum();
	for (i = 0; i < d_num; i++) {
		if (dirGroup[cur_dir].getDentry(i).getIndex() == i_index) {
			break;
		}
	}
	if (i == d_num) {
		cout << "未找到相应文件" << endl;
		return;
	}
	dirGroup[cur_dir].del_Dentry(i);
	
	// 更新Superblock；更新位图；更新Inode表（包括文件Inode的更新，和当前目录对应的Inode的更新）
	disk.free_renew(i_index, dirGroup[cur_dir]);
	
	// 将当前目录写回数据块
	disk.dir_write(dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir]);
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

void FileSystem::rm_rf(int i_index)
{
	// 注意：删除目录要把该目录下所有文件都删除（可能用到递归函数）
	// 在当前目录找到对应的目录项
	int i;
	int cur = cur_dir;
	int dir;
	int d_num = dirGroup[cur_dir].getDentryNum();
	for (i = 0; i < d_num; i++) {
		if (dirGroup[cur_dir].getDentry(i).getIndex() == i_index) {
			break;
		}
	}
	if (i == d_num) {
		cout << "未找到相应文件" << endl;
		return;
	}
	

	// 找到要删除的目录
	for (dir = 0; dir < dirGroup.size(); dir++) {
		if (dirGroup[dir].getI_Index() == i_index) {
			break;
		}
	}
	cd(dir);
	Dentry dentry;
	for (int i = 0; i < dirGroup[cur_dir].getDentryNum(); i++) {
		dentry = dirGroup[cur_dir].getDentry(i);
		if (dentry.getMode() == FILE_MODE) {
			rm_f(dentry.getIndex());
		}
		else if (dentry.getMode() == DIR_MODE) {
			rm_rf(dentry.getIndex());
		}
	}
	cd(cur);

	// 删除对应目录项和目录
	dirGroup[cur_dir].del_Dentry(i);
	dirGroup.erase(dirGroup.begin() + dir);

	// 更新Superblock；更新位图；更新Inode表（包括文件Inode的更新，和当前目录对应的Inode的更新）
	disk.free_renew(i_index, dirGroup[cur_dir]);
	// 将当前目录写回数据块
	disk.dir_write(dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir]);
}

void FileSystem::needFormat(bool doFormat)
{
	this->doFormat = doFormat;
}

bool FileSystem::isFormat()
{
	return doFormat;
}

void FileSystem::help()	//显示所有命令清单 
{
	cout << "ls - 显示当前目录清单" << endl;
	cout << "cd - 进入目录" << endl;
	cout << "touch - 在该目录下创建文件" << endl;
	cout << "rm_f - 删除该目录下的某文件" << endl;
	cout << "mkdir - 创建目录" << endl;
	cout << "rm_rf - 删除该目录下的某目录" << endl;
	return;
}

void FileSystem::cmd(string args)	//处理输入的命令
{
	char inp1[100];
	char inp2[100];

	sscanf(args.c_str(), "%s", inp1);
	// 显示该目录下的所有文件信息（ls）
	if (strcmp(inp1, "ls") == 0) { 
		ls();	
	}
	// 转到该目录（cd）
	else if (strcmp(inp1, "cd") == 0) {
		sscanf(args.c_str(), "%s%s", inp1, inp2);
		int iinp2 = atoi(inp2);
		rm_f(iinp2);
	}
	// 在该目录下创建文件（touch）
	else if (strcmp(inp1, "touch") == 0) {
		sscanf(args.c_str(), "%s%s", inp1, inp2);
		touch(inp2);
	}
	// 删除该目录下的某文件（rm_f）
	else if (strcmp(inp1, "rm_f") == 0) {
		sscanf(args.c_str(), "%s%s", inp1, inp2);
		int iinp2 = atoi(inp2);
		rm_f(iinp2);
	}
	// 在该目录下创建目录（mkdir）
	else if (strcmp(inp1, "mkdir") == 0) {
		sscanf(args.c_str(), "%s%s", inp1, inp2);
		mkdir(inp2);
	}
	// 删除该目录下的某目录（rm_rf）
	else if (strcmp(inp1, "rm_rf") == 0) {	
		sscanf(args.c_str(), "%s%s", inp1, inp2);
		int iinp2 = atoi(inp2);
		rm_f(iinp2);
	}
	// 显示帮助 
	else if (strcmp(inp1, "help") == 0) {
		help();
	}
	else {
		cout << "抱歉，没有该命令" << endl;
	}
	return;
}