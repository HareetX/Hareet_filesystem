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
	if (fr == NULL) { // û���Զ���ʽ���ļ�
		cout << "�ļ�δ�򿪣����ȴ��ļ� ..." << endl;
		return false;
	}
	else { // �ļ��Ѵ�
		// �Ѵ����ļ�����disk
		disk.disk_read(fr);
	}
	return true;
}

bool FileSystem::writeSysFile()
{
	if (fw == NULL) { // û����д��ʽ���ļ�
		cout << "�ļ�δ�򿪣����ȴ��ļ� ..." << endl;
		return false;
	}
	else { // �ļ��Ѵ�
		// ��disk����д������ļ�
		disk.disk_write(fw);
	}
	return true;
}

bool FileSystem::openSysFile()
{
	// ����ʽ����������ļ�
	if ((fr = fopen(FILESYSTEMNAME, "rb")) == NULL) {
		// ��������ļ�������

		// ������������ļ�
		if ((fw = fopen(FILESYSTEMNAME, "wb")) == NULL) {
			// ��������ļ�����ʧ��
			cout << "��������ļ�����ʧ��..." << endl;
			return false;
		}
		// �����ļ������ɹ������´��ļ�
		fr = fopen(FILESYSTEMNAME, "rb");

		doFormat = true; // �����´����ļ������ʽ��
	}
	else { // ��������ļ�����
		// �Ѵ����ļ����ݶ���disk
		readSysFile();
		// д��ʽ���ļ�
		if ((fw = fopen(FILESYSTEMNAME, "wb")) == NULL) {
			// д��ʽ���ļ�ʧ��
			cout << " ��������ļ���ʧ��..." << endl;
			return false;
		}
		// д��ʽ���ļ��ɹ�����disk����д�ش����ļ� ��д��ʽ���ļ�������ļ���
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
		disk.disk_format(); // ��ʽ��disk
		return writeSysFile(); // �Ѹ�ʽ����diskд������ļ�
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
				dir.setParentDir(&dirGroup[j]);
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
		cout << "���ڸ�Ŀ¼��" << endl;
	}
	else if (cur < dirGroup.size() && cur >= 0) {
		cur_dir = cur;
	}
	else {
		cout << "û���ҵ���Ŀ¼" << endl;
	}
}

void FileSystem::touch()
{
}

void FileSystem::rm_f()
{
}

void FileSystem::mkdir()
{
}

void FileSystem::rm_rf()
{
}

void FileSystem::needFormat(bool doFormat)
{
	this->doFormat = doFormat;
}

bool FileSystem::isFormat()
{
	return doFormat;
}
