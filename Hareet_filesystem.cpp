
/*
* ���ļ�ϵͳ�ǻ���C++ʵ�ֵĶ����ļ�ϵͳ��
* ���л�����ʹ��VS2022���У�
*           ���� ��Ŀ -> ���� -> �������� -> �߼� -> �ַ��� Ϊ ʹ�ö��ֽ��ַ���
*           �� ��Ŀ -> ���� -> �������� -> C/C++ -> Ԥ������ -> Ԥ���������� ����� _CRT_SECURE_NO_WARNINGS
*/

#pragma once
#include "Hareet_filesystem.h"

int main() {
	FileSystem fs; // ʵ����FileSystem
	
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
		Close(fs);
		return -1;
	}// ��ʽ���ɹ�

	// ��װ�ļ�ϵͳ����������Ŀ¼��Ϣ��
	Install(fs);

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