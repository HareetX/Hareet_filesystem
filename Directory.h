#pragma once
//#include "XX_filesystem.h"

class Dentry
{
private:
	//int NO;
	
	int dentry_size; // 本目录项的长度
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
	int getSize();
};

class Directory
{
private:
	string dir_name; // 目录名
	int i_index; // 该目录文件对应的Inode索引
	size_t dentry_num; // 包含目录项的个数
	Directory* parent_dir; // 上级目录
	vector<Dentry> dentryGroup; // 目录项组

public:
	Directory();
	//Directory(int dir_type);

	// 设置目录属性的接口
	void setName(const char* str);
	void setI_Index(int index);
	void renewDentryNum();
	void setParentDir(Directory* dir);
	// ...
	
	// 获得目录属性的接口
	string getName();
	int getI_Index();
	size_t getDentryNum();
	Directory* getParentDir();
	// ...

	// 返回第No号目录项
	Dentry getDentry(int No);
	
	// 添加目录项
	void add_Dentry(int index, int mode, const char* str);
	void add_Dentry(Dentry dentry);
	
	// 删除目录项
	void del_Dentry(int No);

	// 打印目录信息
	void printDir();
};

