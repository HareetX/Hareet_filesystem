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
	userGroup.clear();

	memset(Cur_Host_Name, 0, sizeof(Cur_Host_Name));
	DWORD k = 100;
	GetComputerName(Cur_Host_Name, &k);
	userGroup.push_back(User("root", "root"));
	cur_user = -1; // 系统一开始未登录
}

bool FileSystem::is_Login()
{
	//cout << isLogin;
	return isLogin;
}

//void inUsername(char username[])	//输入用户名
//{
//	
//	cout << "username: " ;
//	cin >> username;
//}

//void inPasswd(char passwd[])	//输入密码
//{
//	cout << "passwd: ";
//	cin >> passwd;
//	//cin.ignore();
//	//int i = 0;
//	//while (true)
//	//{
//	//	passwd[i] = getch();     //只能接收一个动作 
//	//	if (passwd[i] == '\r')     //回车键表示\r\n 
//	//	{
//	//		break;
//	//	}
//
//	//}
//}

//bool check(char username[], char passwd[]) //核对用户名，密码
//{	
//
//	if ((strcmp(username, "root") == 0) && (strcmp(passwd, "root") == 0)) {
//
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//}


void FileSystem::Login()	//登录界面
{
	 //只计划做一名用户为root
	cout << "本文件系统只有root用户，请输入用户名root以及密码root登录:" << endl;
	char username[100] = { 0 };
	char passwd[100] = { 0 };
	//inUsername(username);	
	// 输入用户名
	cout << "username: ";
	cin >> username;
	//inPasswd(passwd);		
	// 输入用户密码
	cout << "passwd: ";
	cin >> passwd;

	//核对用户名和密码
	int usersize = userGroup.size();
	for (int i = 0; i < usersize; i++) {
		if (userGroup[i].check(username, passwd)) {
			isLogin = true;
			cur_user = i;
			break;
		}
		else {
			isLogin = false;
		}
	}
	if (!isLogin) {
		cout << "用户名或密码错误" << endl;
	}
	/*if (check(username, passwd)) {	
		isLogin = true;
		
	}
	else {
		isLogin = false;
		
	}*/
	cin.ignore();
	system("pause");
	system("cls");
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


int FileSystem::find_dir(const char* name)
{
	int dir_size = dirGroup.size();
	for (int i = 0; i < dir_size; i++) {
		if (name == dirGroup[i].getName()) {
			return i;
		}
	}
	return -1;
}

bool FileSystem::check_fname(const char* name, int mode)
{
	if (dirGroup[cur_dir].find_file(name, mode) == -1) { 
		// 没找到文件，说明没有重名
		return false;
	}
	// 找到了文件，说明有重名
	return true;
}

int FileSystem::find_file(const char* name, int mode)
{
	return dirGroup[cur_dir].find_file(name, mode);
}

void FileSystem::printUserPos()
{
	cout << userGroup[cur_user].getUsername() << "@" << Cur_Host_Name << ": " << getDirPos(cur_dir) << "$";
}

string FileSystem::getDirPos(int cur)
{
	if (cur == 0) {
		return dirGroup[cur].getName();
	}
	else {
		return getDirPos(dirGroup[cur].getParentDir()) + "/" + dirGroup[cur].getName();
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
	// 检查是否有重名文件
	if (check_fname(name, FILE_MODE)) {
		cout << "文件名重复，请更换文件名" << endl;
		return;
	}
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

void FileSystem::rm_f(const char* name)
{
	// 在当前目录删除相应的目录项
	int d_index = find_file(name, FILE_MODE);
	if (d_index == -1) {
		cout << "未找到文件" << endl;
		return;
	}

	// 记录删除文件的inode索引
	int i_index = dirGroup[cur_dir].getDentry(d_index).getIndex();
	
	dirGroup[cur_dir].del_Dentry(d_index);
	
	// 更新Superblock；更新位图；更新Inode表（包括文件Inode的更新，和当前目录对应的Inode的更新）
	disk.free_renew(i_index, dirGroup[cur_dir]);
	
	// 将当前目录写回数据块
	disk.dir_write(dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir]);
}


void FileSystem::mkdir(const char* name)
{
	// 检查是否有重名目录
	if (check_fname(name, DIR_MODE)) {
		cout << "目录名重复，请更换目录名" << endl;
		return;
	}

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

void FileSystem::rm_rf(const char* name)
{
	// 注意：删除目录要把该目录下所有文件都删除（可能用到递归函数）
	// 在当前目录找到对应的目录项
	int d_index = find_file(name, DIR_MODE);
	if (d_index == -1) {
		cout << "未找到目录" << endl;
		return;
	}
	int cur = cur_dir;
	
	/*int d_num = dirGroup[cur_dir].getDentryNum();
	for (i = 0; i < d_num; i++) {
		if (dirGroup[cur_dir].getDentry(i).getIndex() == i_index) {
			break;
		}
	}
	if (i == d_num) {
		cout << "未找到相应文件" << endl;
		return;
	}*/
	

	// 找到要删除的目录
	/*for (dir = 0; dir < dirGroup.size(); dir++) {
		if (dirGroup[dir].getI_Index() == i_index) {
			break;
		}
	}*/
	int dir = find_dir(name);
	if (d_index == -1) {
		cout << "未找到目录" << endl;
		return;
	}
	// 记录删除目录的inode索引
	int i_index = dirGroup[dir].getI_Index();
	
	cd(dir);
	Dentry dentry;
	for (int i = 0; i < dirGroup[cur_dir].getDentryNum(); i++) {
		dentry = dirGroup[cur_dir].getDentry(i);
		if (dentry.getMode() == FILE_MODE) {
			rm_f(dentry.getName().c_str());
		}
		else if (dentry.getMode() == DIR_MODE) {
			rm_rf(dentry.getName().c_str());
		}
	}
	cd(cur);

	// 删除对应目录项和目录
	dirGroup[cur_dir].del_Dentry(d_index);
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
	cout << "***************************************" << endl;
	cout << "*    ls     - 显示当前目录清单        *" << endl;
	cout << "*    cd     - 转入目录                *" << endl;
	cout << "*    touch  - 在该目录下创建文件      *" << endl;
	cout << "*    mkdir  - 创建目录                *" << endl;
	cout << "*    rm -f  - 删除该目录下的文件      *" << endl;
	cout << "*    rm -rf - 删除该目录下的目录      *" << endl;
	cout << "*    open   - 打开文件（可读写文件）  *" << endl;
	cout << "*    q      - 退出文件系统            *" << endl;
	cout << "***************************************" << endl;
	return;
}

bool FileSystem::cmd(string args)	//处理输入的命令
{
	if (args == "") {
		return true;
	}
	//用于存放分割后的字符串
	vector<string> res;
	//待分割的字符串，含有空格 
	string inp = args;
	string result;
	//将字符串读到input中 
	stringstream input(inp);
	//依次输出到result中，并存入res中 
	while (input >> result)
		res.push_back(result);
	
	int inp2;

	if (res[0] == "ls") {
		ls();	
	}
	// 转到该目录（cd）
	else if (res[0] == "cd") {
		if (res.size() <= 1) { // 未输入转入目录名
			cout << "未输入转入目录名" << endl;
			return true;
		}
		else if (res[1] == ".") {
			return true;
		}
		else if (res[1] == "..") {
			inp2 = dirGroup[cur_dir].getParentDir();
		}
		else {
			inp2 = find_dir(res[1].c_str()); // 找到相应的目录
		}
		cd(inp2);
	}
	// 在该目录下创建文件（touch）
	else if (res[0] == "touch") {
		if (res.size() <= 1) { // 未输入文件名
			cout << "未输入文件名" << endl;
			return true;
		}
		touch(res[1].c_str());
	}
	// 在该目录下创建目录（mkdir）
	else if (res[0] == "mkdir") {
		if (res.size() <= 1) { // 未输入目录名
			cout << "未输入目录名" << endl;
			return true;
		}
		mkdir(res[1].c_str());
	}
	// 删除该目录下的文件（rm）
	else if (res[0] == "rm") {
		if (res.size() <= 1 || (res[1] != "-f" && res[1] != "-rf")) {
			cout << "rm -f  - 删除该目录下的文件" << endl;
			cout << "rm -rf - 删除该目录下的目录" << endl;
		}
		else if (res[1] == "-f") { // 删除该目录下的某文件（rm_f）
			if (res.size() <= 2) {
				cout << "未输入要删除的文件名" << endl;
			}
			else {
				rm_f(res[2].c_str());
			}
		}
		else if (res[1] == "-rf") { // 删除该目录下的某目录（rm_rf）
			if (res.size() <= 2) {
				cout << "未输入要删除的目录名" << endl;
			}
			else {
				rm_rf(res[2].c_str());
			}
		}
		else {
			cout << "rm -f  - 删除该目录下的文件" << endl;
			cout << "rm -rf - 删除该目录下的目录" << endl;
		}
	}
	// 打开一般文件
	else if (res[0] == "open") {
		if (res.size() <= 1) { // 未输入文件名
			cout << "未输入文件名" << endl;
			return true;
		}
		openfile(res[1].c_str());
	}
	// 显示帮助
	else if (res[0] == "help") {
		help();
	}
	// 退出系统
	else if (res[0] == "q") {
		return false;
	}
	else {
		cout << "抱歉，没有该命令" << endl;
	}
	
	return true;
}

void FileSystem::openfile(const char* name)
{
	// 找到需要打开的文件
	int d_index = find_file(name, FILE_MODE);
	if (d_index == -1) {
		cout << "未找到文件" << endl;
		return;
	}
	int i_index = dirGroup[cur_dir].getDentry(d_index).getIndex();
	cout << name << "已打开..." << endl;
	// 读出文本内容
	char* context = disk.file_read(i_index);
	cout << context << endl;
	delete[] context;

	// 用户可以输入文本内容
	cout << "您可写入文本内容（输入 q 退出编辑）：" << endl;
	string text = "\0";
	while (1) {
		getline(cin, text);
		if (text == "q") {
			cout << "文件已关闭..." << endl;
			break;
		}
		text = "\n" + text;
		if (!disk.file_write(i_index, text.c_str())) {
			cout << "文件已关闭..." << endl;
			break;
		}
		dirGroup[cur_dir].setDentryFsize(name, disk.file_size(i_index));
	}
}

User::User(string username, string password)
{
	this->username = username;
	this->password = password;
}

void User::setUser(string username, string password)
{
	this->username = username;
	this->password = password;
}

bool User::check(string username, string password)
{
	if (username == this->username && password == this->password) {
		return true;
	}
	return false;
}

string User::getUsername()
{
	return username;
}
