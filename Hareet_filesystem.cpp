
/*
* 本文件系统是基于C++实现的二级文件系统。
* 运行环境：使用VS2022运行，
*           更改 项目 -> 属性 -> 配置属性 -> 高级 -> 字符集 为 使用多字节字符集
*           在 项目 -> 属性 -> 配置属性 -> C/C++ -> 预处理器 -> 预处理器定义 中添加 _CRT_SECURE_NO_WARNINGS
*/

#pragma once
#include "Hareet_filesystem.h"

int main() {
	FileSystem fs; // 实例化FileSystem
	
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
		Close(fs);
		return -1;
	}// 格式化成功

	// 安装文件系统（读入所有目录信息）
	Install(fs);

	while (1){
		if (fs.is_Login()) { // 已登录
			fs.printUserPos();
			string args = "\0"; 
			getline(cin, args);
			if (!fs.cmd(args)) { // 如果输入 q ，则返回false，退出循环
				break;
			}
		}
		else { // 未登录
			fs.Login(); // 登录
			fs.help();
		}
	}
	
	Close(fs);

	return 0;
}