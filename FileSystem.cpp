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

void FileSystem::needFormat(bool doFormat)
{
	this->doFormat = doFormat;
}

bool FileSystem::isFormat()
{
	return doFormat;
}
