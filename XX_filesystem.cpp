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

	//fs.readDirGroup();
	//fs.ls();

	//fs.writeDirGroup();
	//fs.ls();

	/*fs.touch("abc.txt");
	fs.ls();*/

	//fs.mkdir("adgj");
	//fs.ls();

	///*fs.cd(1);
	//fs.ls();*/

	//fs.rm_f(1);
	//fs.ls();

	//fs.cd(1);
	//fs.touch("dfg.txt");
	//fs.ls();

	//fs.cd(0);
	//fs.ls();

	//fs.rm_rf(2);
	//fs.ls();

	/*Dentry dentry(0, FILE_MODE, "abcd.txt");
	dentry.setName("abd.txt");
	dentry.renewSize();*/

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

	//fs.is_Login();

	//fs.help();
	
	//string inp;
	//cin >> inp;
	//fs.cmd(inp);


	while (1){
		if (fs.is_Login()) { // �ѵ�¼
			string args = "\0";
			 //readline.h �е������뺯������shellʵ��ʱ�ȽϷ���
			 //readline(args); 
			cin.ignore();
			getline(cin, args);
			fs.cmd(args);
		}
		else { // δ��¼
			fs.Login(); // ��¼
		}
	}
	
	Close(fs);

	return 0;
}