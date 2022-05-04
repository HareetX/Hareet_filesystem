#pragma once
#include "XX_filesystem.h"

int main() {
	FileSystem fs; // ʵ����FileSystem������ʼ���ļ�ϵͳ

	// �򿪴����ļ�
	if (!Open(fs)) {
		// �򿪴����ļ�ʧ��
		cout << "�򿪴����ļ�ʧ�� ..." << endl;
		return -1;
	}// �ɹ����ļ�
	
	// ����ļ�ϵͳ��ʽ�����󣬲������ʽ��
 	if (!Format(fs)) {
		// ��ʽ��ʧ��
		cout << "��ʽ��ʧ��..." << endl;
		return -1;
	}// ��ʽ���ɹ�

	Dentry dentry(0, FILE_MODE, "abcd.txt");
	dentry.setName("abd.txt");
	dentry.renewSize();

	////Inode_Label il;
	////Disk d;
	//FileSystem f;
	//FILE* fpw = fopen("a.sys", "wb");
	//FILE* fpr = fopen("a.sys", "rb");

	//// ֱ�ӵ���C�Ŀ⺯����֤����дû����

	///*fseek(fpw, 0, SEEK_SET);
	//fwrite(&il, sizeof(Inode_Label), 1, fpw);*/
	///*fseek(fpr, 0, SEEK_SET);
	//fread(&il, sizeof(Inode_Label), 1, fpr);*/

	//// ���ð�����FileSystem���е�Disk���е�inode���д������֤����дû����

	////d.inode_write(fpw);
	////d.inode_read(fpr);

	//// ����FileSystem��Ķ�дdisk������֤����дû����
	//// �����������ļ���дָ�봫��fs�������
	//f.f_read(fpr);
	//f.f_write(fpw);

	////f.writeSysFile();
	//f.readSysFile();

	//fclose(fpw);
	//fclose(fpr);

	//while (1){
	//	if (isLogin) { // �ѵ�¼
	//		string args = "\0";
	//		// readline.h �е������뺯������shellʵ��ʱ�ȽϷ���
	//		//readline(args); 
	//		getline(cin, args);
	//		cmd(args);
	//	}
	//	else { // δ��¼
	//		Login(); // ��¼
	//	}
	//}
	
	Close(fs);

	return 0;
}