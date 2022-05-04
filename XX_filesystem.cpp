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

	Dentry dentry(0, FILE_MODE, "abcd.txt");
	dentry.setName("abd.txt");
	dentry.renewSize();

	////Inode_Label il;
	////Disk d;
	//FileSystem f;
	//FILE* fpw = fopen("a.sys", "wb");
	//FILE* fpr = fopen("a.sys", "rb");

	//// 直接调用C的库函数验证：读写没问题

	///*fseek(fpw, 0, SEEK_SET);
	//fwrite(&il, sizeof(Inode_Label), 1, fpw);*/
	///*fseek(fpr, 0, SEEK_SET);
	//fread(&il, sizeof(Inode_Label), 1, fpr);*/

	//// 调用包含在FileSystem类中的Disk类中的inode表读写函数验证：读写没问题

	////d.inode_write(fpw);
	////d.inode_read(fpr);

	//// 调用FileSystem类的读写disk函数验证：读写没问题
	//// 把主函数的文件读写指针传进fs方便调试
	//f.f_read(fpr);
	//f.f_write(fpw);

	////f.writeSysFile();
	//f.readSysFile();

	//fclose(fpw);
	//fclose(fpr);

	//while (1){
	//	if (isLogin) { // 已登录
	//		string args = "\0";
	//		// readline.h 中的行输入函数，在shell实现时比较方便
	//		//readline(args); 
	//		getline(cin, args);
	//		cmd(args);
	//	}
	//	else { // 未登录
	//		Login(); // 登录
	//	}
	//}
	
	Close(fs);

	return 0;
}