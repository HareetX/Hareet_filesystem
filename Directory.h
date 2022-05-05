#pragma once
#include "XX_filesystem.h"

class Dentry
{
private:
	//int NO;
	
	size_t dentry_size; // ��Ŀ¼��ĳ���
	int i_index; // �ļ�inode
	int f_mode; // �ļ�����
	string f_name; // �ļ���

public:
	Dentry(int index, int mode, const char* str);
	Dentry();

	// ����Ŀ¼�����ԵĽӿ�
	void setIndex(int index);
	void setMode(int mode);
	void setName(const char* str);
	void renewSize();

	//void setNo(int No);

	// ���Ŀ¼�����ԵĽӿ�
	int getIndex();
	int getMode();
	string getName();
	size_t getSize();
};

class Directory
{
private:
	string dir_name; // Ŀ¼��
	size_t dentry_num; // ����Ŀ¼��ĸ���
	Directory* parent_dir; // �ϼ�Ŀ¼
	vector<Dentry> dentryGroup; // Ŀ¼����

public:
	Directory();

	// ����Ŀ¼���ԵĽӿ�
	void setName(const char* str);
	void renewDentryNum();
	void setParentDir(Directory* dir);
	// ...
	
	// ���Ŀ¼���ԵĽӿ�
	string getName();
	size_t getDentryNum();
	Directory* getParentDir();
	// ...
	
	// ���Ŀ¼��
	void add_Dentry(int index, int mode, const char* str);
	
	// ɾ��Ŀ¼��
	void del_Dentry(int No);
};

