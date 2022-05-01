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

Directory::Directory()
{
	dir_name = "\0";
	dentry_num = 0;
	parent_dir = NULL;
	dentryGroup.clear();
}
