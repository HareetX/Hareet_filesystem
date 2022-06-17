#pragma once
#include "XX_filesystem.h"

bool Open(FileSystem& fs)
{
	bool isopen = fs.openSysFile();
	return isopen;
}

bool Format(FileSystem& fs)
{
	if (!(fs.isFormat())) {
		// ��ʾ�û����Ƿ���Ҫ��ʽ��
		char format = '\0';
		cout << "�Ƿ���Ҫ��ʽ�� [y/n] ( Ĭ��Ϊ n )" << endl;
		cin >> format;
		if (format == 'y') {
			fs.needFormat(true);
		}
		else {
			fs.needFormat(false);
		}
	}
	system("pause");
	system("cls");
	return fs.formatSysFile();
}

void Install(FileSystem& fs)
{
	fs.readDirGroup();
}

void Close(FileSystem& fs)
{
	fs.writeDirGroup();
	fs.writeSysFile();
	fs.closeSysFile();
}





