#include "StartDebug.h"
#include <Windows.h>

int main(int argc,char* argv[])
{
	char cPePath[MAX_PATH];

	if (NULL!=argv[1])
	{
		strcpy_s(cPePath,argv[1]);

	}

	//���ֽ��ַ�תΪ���ַ�

	//char st[20] = {"���ֽ��ַ���!"};

	DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, cPePath, -1, NULL, 0);
	wchar_t *pwText=new wchar_t[dwNum];
	if(!pwText)
	{
		delete []pwText;
	}
	MultiByteToWideChar (CP_ACP, 0, cPePath, -1, pwText, dwNum);

	StartDebug(true,pwText);

	delete []pwText;



	

	return 0;
}