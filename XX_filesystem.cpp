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

	fs.readDirGroup();

	while (1){
		if (fs.is_Login()) { // �ѵ�¼
			fs.printUserPos();
			string args = "\0"; 
			getline(cin, args);
			if (!fs.cmd(args)) { // ������� q ���򷵻�false���˳�ѭ��
				break;
			}
		}
		else { // δ��¼
			fs.Login(); // ��¼
			fs.help();
		}
	}
	
	Close(fs);

	return 0;
}