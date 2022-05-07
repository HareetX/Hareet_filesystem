#pragma once
//#include "XX_filesystem.h"

class Dentry
{
private:
	//int NO;
	
	int dentry_size; // ��Ŀ¼��ĳ���
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
	int getSize();
};

class Directory
{
private:
	string dir_name; // Ŀ¼��
	int i_index; // ��Ŀ¼�ļ���Ӧ��Inode����
	size_t dentry_num; // ����Ŀ¼��ĸ���
	Directory* parent_dir; // �ϼ�Ŀ¼
	vector<Dentry> dentryGroup; // Ŀ¼����

public:
	Directory();
	//Directory(int dir_type);

	// ����Ŀ¼���ԵĽӿ�
	void setName(const char* str);
	void setI_Index(int index);
	void renewDentryNum();
	void setParentDir(Directory* dir);
	// ...
	
	// ���Ŀ¼���ԵĽӿ�
	string getName();
	int getI_Index();
	size_t getDentryNum();
	Directory* getParentDir();
	// ...

	// ���ص�No��Ŀ¼��
	Dentry getDentry(int No);
	
	// ���Ŀ¼��
	void add_Dentry(int index, int mode, const char* str);
	void add_Dentry(Dentry dentry);
	
	// ɾ��Ŀ¼��
	void del_Dentry(int No);

	// ��ӡĿ¼��Ϣ
	void printDir();
};

