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
		cout << "�Ƿ���Ҫ��ʽ��? [y/n] ( Ĭ��Ϊ n )" << endl;
		cin >> format;
		if (format == 'y') {
			fs.needFormat(true);
		}
		else {
			fs.needFormat(false);
		}
	}
	return fs.formatSysFile();
}

void Close(FileSystem& fs)
{
	fs.writeSysFile();
	fs.closeSysFile();
}





