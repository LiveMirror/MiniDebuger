#pragma once


bool Deal_CPEV();//���̴����¼�
bool InvalidInt3(DWORD dwIndex);//ʹint3�ϵ�ʧЧ
bool DeleteInt3(DWORD dwIndex);
bool ClearInt3(DWORD dwIndex);
int SerarchInt3(LPVOID lpAddress);
bool WriteInt3(LPVOID	lpAddress);

DWORD OnExceptionDebugEvent(LPEXCEPTION_DEBUG_INFO pDbgInfo);