#include "XX_filesystem.h"

int main() {
	bool isFormat = false; // 是否需要格式化

	// 读方式打开虚拟磁盘文件
	if ((fr = fopen(FILESYSTEMNAME, "rb")) == NULL) { 
		// 虚拟磁盘文件不存在

		// 创建虚拟磁盘文件
		if ((fw = fopen(FILESYSTEMNAME, "wb")) == NULL) { 
			// 虚拟磁盘文件创建失败
			cout << "虚拟磁盘文件创建失败..." << endl;
			return 1;
		}
		// 虚拟文件创建成功

		isFormat = true; // 创建新磁盘文件必须格式化
	}
	else { // 虚拟磁盘文件存在
		// 读取文件
		fread(buffer, Sum_Size, 1, fr);
		// 写方式打开文件
		if ((fw = fopen(FILESYSTEMNAME, "wb")) == NULL) {
			// 写方式打开文件失败
			cout << " 虚拟磁盘文件打开失败..." << endl;
			return 1;
		}
		// 写方式打开文件成功，写回文件
		fwrite(buffer, Sum_Size, 1, fw);

		// 提示用户，是否需要格式化
		char format = '\0';
		cout << "是否需要格式化? [y/n] ( 默认为 n )" << endl;
		cin >> format;
		if (format == 'y') {
			isFormat = true;
		}
		else {
			isFormat = false;
		}
	}

	// 判断是否需要格式化
	if (isFormat) { // 需要格式化
		if (!Format()) { // 磁盘文件格式化
			// 格式化失败
			cout << "格式化失败..." << endl;
			return 1;
		}
		// 格式化成功
	}
	// 不需要格式化

	//初始化文件
	Initial();
	
	return 0;
}