#include "Directory.h"

Dentry::Dentry(int index, int mode, const char* str)
{
	//NO = -1;
	i_index = index;
	f_mode = mode;
	f_name = str;
	dentry_size = sizeof(i_index) + sizeof(f_mode) + f_name.length() + sizeof(dentry_size);
}

Dentry::Dentry()
{
	//NO = -1;
	i_index = -1;
	f_mode = -1;
	f_name = "\0";
	dentry_size = 0;
}

void Dentry::setIndex(int index)
{
	i_index = index;
}

void Dentry::setMode(int mode)
{
	f_mode = mode;
}

void Dentry::setName(const char* str)
{
	f_name = str;
}

void Dentry::renewSize()
{
	dentry_size = sizeof(i_index) + sizeof(f_mode) + f_name.length() + sizeof(dentry_size);
}

//void Dentry::setNo(int No)
//{
//	NO = No;
//}

int Dentry::getIndex()
{
	return i_index;
}

int Dentry::getMode()
{
	return f_mode;
}

size_t Dentry::getSize()
{
	return dentry_size;
}

string Dentry::getName()
{
	return f_name;
}


Directory::Directory()
{
	dir_name = "\0";
	dentry_num = 0;
	parent_dir = NULL;
	dentryGroup.clear();
}

void Directory::setName(const char* str)
{
	dir_name = str;
}

void Directory::renewDentryNum()
{
	dentry_num = dentryGroup.size();
}

void Directory::setParentDir(Directory* dir)
{
	parent_dir = dir;
}

string Directory::getName()
{
	return dir_name;
}

size_t Directory::getDentryNum()
{
	return dentry_num;
}

Directory* Directory::getParentDir()
{
	return parent_dir;
}

void Directory::add_Dentry(int index, int mode, const char* str)
{
	Dentry d;
	d.setIndex(index);
	d.setMode(mode);
	d.setName(str);
	d.renewSize();
	//Dentry d(int index, int mode, const char* str);
	dentryGroup.push_back(d);	// ��������Ŀ¼�������Ŀ¼����β��
	dentry_num = dentryGroup.size();	// ����Ŀ¼��ĸ���	
}

void Directory::del_Dentry(int No)
{
	dentryGroup.erase(dentryGroup.begin() + No - 1); // ɾ����No��Ŀ¼��
	dentry_num = dentryGroup.size();	// ����Ŀ¼��ĸ���
}