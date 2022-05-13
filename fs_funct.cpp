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
		// 提示用户，是否需要格式化
		char format = '\0';
		cout << "是否需要格式化? [y/n] ( 默认为 n )" << endl;
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





