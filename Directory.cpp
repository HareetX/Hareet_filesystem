//#include "Directory.h"
#include "XX_filesystem.h"

Dentry::Dentry(int index, int mode, int size, const char* str)
{
	//NO = -1;
	i_index = index;
	f_mode = mode;
	f_size = size;
	f_name = str;
	dentry_size = sizeof(i_index) + sizeof(f_mode) + sizeof(f_size) + f_name.length() + 1 + sizeof(dentry_size);
}

Dentry::Dentry()
{
	//NO = -1;
	i_index = -1;
	f_mode = -1;
	f_size = -1;
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

void Dentry::setF_Size(int size)
{
	f_size = size;
}

void Dentry::setName(const char* str)
{
	f_name = str;
}

void Dentry::renewSize()
{
	dentry_size = sizeof(i_index) + sizeof(f_mode) + f_name.length() + 1 + sizeof(dentry_size);
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

int Dentry::getF_Size()
{
	return f_size;
}

int Dentry::getSize()
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
	i_index = -1; // 需要后续初始化
	dentry_num = 2;
	parent_dir = 0;
	dentryGroup.clear();
	Dentry cur_dir(0, FILE_MODE, 0, ".");
	Dentry par_dir(0, FILE_MODE, 0, "..");
	add_Dentry(cur_dir);
	add_Dentry(par_dir);
}

//Directory::Directory(int dir_type)
//{
//	dir_name = "\0";
//	dentry_num = 0;
//	parent_dir = NULL;
//	dentryGroup.clear();
//	Dentry cur_dir(0, DIR_MODE, ".");
//	Dentry par_dir(0, DIR_MODE, "..");
//	add_Dentry(cur_dir);
//	add_Dentry(par_dir);
//}

void Directory::setName(const char* str)
{
	dir_name = str;
}

void Directory::setI_Index(int index)
{
	i_index = index;
}

void Directory::renewDentryNum()
{
	dentry_num = dentryGroup.size();
}

void Directory::setParentDir(int dir)
{
	parent_dir = dir;
}

void Directory::dir_clear()
{
	dentry_num = 0;
	dentryGroup.clear();
}

string Directory::getName()
{
	return dir_name;
}

int Directory::getI_Index()
{
	return i_index;
}

size_t Directory::getDentryNum()
{
	return dentry_num;
}

int Directory::getParentDir()
{
	return parent_dir;
}

int Directory::getDirSize()
{
	int dirSize = 0;
	for (int i = 0; i < dentryGroup.size(); i++) {
		dirSize += dentryGroup[i].getSize();
	}
	return dirSize;
}

Dentry Directory::getDentry(int No)
{
	return dentryGroup[No];
}

void Directory::add_Dentry(int index, int mode, const char* str)
{
	Dentry d;
	d.setIndex(index);
	d.setMode(mode);
	d.setName(str);
	d.renewSize();
	//Dentry d(int index, int mode, const char* str);
	dentryGroup.push_back(d);	// 将新增的目录项添加至目录项组尾部
	dentry_num = dentryGroup.size();	// 更新目录项的个数	
}

void Directory::add_Dentry(Dentry dentry)
{
	dentryGroup.push_back(dentry);	// 将新增的目录项添加至目录项组尾部
	dentry_num = dentryGroup.size();	// 更新目录项的个数
}

void Directory::del_Dentry(int No)
{
	dentryGroup.erase(dentryGroup.begin() + No - 1); // 删除第No个目录项
	dentry_num = dentryGroup.size();	// 更新目录项的个数
}

void Directory::printDir()
{
	for (int i = 0; i < dentry_num; i++) {
		cout << dentryGroup[i].getName() << "\t" 
			 << (dentryGroup[i].getMode() ? "目录文件" : "一般文件") << "\t" 
			 << dentryGroup[i].getF_Size() << "B"  // 打印的不应该是目录项的大小，而是文件大小
			 << endl;
	}
}
