#include "XX_filesystem.h"

int main() {
	bool isFormat = false; // �Ƿ���Ҫ��ʽ��

	// ����ʽ����������ļ�
	if ((fr = fopen(FILESYSTEMNAME, "rb")) == NULL) { 
		// ��������ļ�������

		// ������������ļ�
		if ((fw = fopen(FILESYSTEMNAME, "wb")) == NULL) { 
			// ��������ļ�����ʧ��
			cout << "��������ļ�����ʧ��..." << endl;
			return 1;
		}
		// �����ļ������ɹ�

		isFormat = true; // �����´����ļ������ʽ��
	}
	else { // ��������ļ�����
		// ��ȡ�ļ�
		fread(buffer, Sum_Size, 1, fr);
		// д��ʽ���ļ�
		if ((fw = fopen(FILESYSTEMNAME, "wb")) == NULL) {
			// д��ʽ���ļ�ʧ��
			cout << " ��������ļ���ʧ��..." << endl;
			return 1;
		}
		// д��ʽ���ļ��ɹ���д���ļ�
		fwrite(buffer, Sum_Size, 1, fw);

		// ��ʾ�û����Ƿ���Ҫ��ʽ��
		char format = '\0';
		cout << "�Ƿ���Ҫ��ʽ��? [y/n] ( Ĭ��Ϊ n )" << endl;
		cin >> format;
		if (format == 'y') {
			isFormat = true;
		}
		else {
			isFormat = false;
		}
	}

	// �ж��Ƿ���Ҫ��ʽ��
	if (isFormat) { // ��Ҫ��ʽ��
		if (!Format()) { // �����ļ���ʽ��
			// ��ʽ��ʧ��
			cout << "��ʽ��ʧ��..." << endl;
			return 1;
		}
		// ��ʽ���ɹ�
	}
	// ����Ҫ��ʽ��

	//��ʼ���ļ�
	Initial();
	
	return 0;
}