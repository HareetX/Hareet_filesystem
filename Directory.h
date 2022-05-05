#pragma once
#include "XX_filesystem.h"

class Dentry
{
private:
	//int NO;
	
	size_t dentry_size; // 本目录项的长度
	int i_index; // 文件inode
	int f_mode; // 文件类型
	string f_name; // 文件名

public:
	Dentry(int index, int mode, const char* str);
	Dentry();

	// 设置目录项属性的接口
	void setIndex(int index);
	void setMode(int mode);
	void setName(const char* str);
	void renewSize();

	//void setNo(int No);

	// 获得目录项属性的接口
	int getIndex();
	int getMode();
	string getName();
	size_t getSize();
};

class Directory
{
private:
	string dir_name; // 目录名
	size_t dentry_num; // 包含目录项的个数
	Directory* parent_dir; // 上级目录
	vector<Dentry> dentryGroup; // 目录项组

public:
	Directory();

	// 设置目录属性的接口
	void setName(const char* str);
	void renewDentryNum();
	void setParentDir(Directory* dir);
	// ...
	
	// 获得目录属性的接口
	string getName();
	size_t getDentryNum();
	Directory* getParentDir();
	// ...
	
	// 添加目录项
	void add_Dentry(int index, int mode, const char* str);
	
	// 删除目录项
	void del_Dentry(int No);
};

