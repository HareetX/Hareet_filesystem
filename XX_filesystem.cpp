#pragma once
#include "XX_filesystem.h"



int main() {
	FileSystem fs; // 实例化FileSystem，并初始化文件系统
	
	// 打开磁盘文件
	if (!Open(fs)) {
		// 打开磁盘文件失败
		cout << "打开磁盘文件失败 ..." << endl;
		return -1;
	}// 成功打开文件
	
	// 检查文件系统格式化需求，并按需格式化
 	if (!Format(fs)) {
		// 格式化失败
		cout << "格式化失败..." << endl;
		return -1;
	}// 格式化成功

	fs.readDirGroup();

	while (1){
		if (fs.is_Login()) { // 已登录
			string args = "\0"; 
			
			getline(cin, args);
			if (!fs.cmd(args)) { // 如果输入 q ，则返回false，退出循环
				break;
			}
		}
		else { // 未登录
			fs.Login(); // 登录
			cin.ignore();
		}
	}
	
	Close(fs);

	return 0;
}