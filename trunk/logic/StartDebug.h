#pragma once


//bool bNewProcess	:�Ե��Եķ�ʽ����һ������(1),���ӵ�һ������(0)
//void *Infor		:ָ��һ���ַ�������DWORDֵ,��ȡ����bNewProcess.
int StartDebug(bool bNewProcess,void *Infor);

//return:
//false.����δ��ʼ��.
bool Detach();


//bLove==true   �����߱���,�������߲����˳�
//bLove==false  �෴.Ĭ�����.
//����:
//falseʧ��
bool LoveIt(bool bLove);