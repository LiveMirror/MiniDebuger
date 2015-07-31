#include <Windows.h>
#include "Debug.h"
#include "AntiAsm.h"

_DEBUG_EVENT	DbgEvt	= {0};		
DWORD	dwRet			= DBG_EXCEPTION_NOT_HANDLED;//δ����!!!!!!!!!!!dwRet�д���ʹ�õĴ����ƺ�
STARTUPINFO			stcStartupInfo	= {sizeof(STARTUPINFO)};
PROCESS_INFORMATION stcProcInfo		= {0};//������Ϣ
HANDLE g_hProc;//�������ߵľ��

int StartDebug(bool bNewProcess,void *Infor);
DWORD OnExceptionDebugEvent(LPEXCEPTION_DEBUG_INFO pDbgInfo);
bool GetEvent();




//////////////////////////////////////////////////////////////////////////
//bool bNewProcess	:�Ե��Եķ�ʽ����һ������(1),���ӵ�һ������(0)
//void *Infor		:ָ��һ��wchar_t�ַ�������DWORDֵ,��ȡ����bNewProcess.
int StartDebug(bool bNewProcess,void *Infor)
{
	if (bNewProcess)
	{
		if(!CreateProcess((LPCWSTR)Infor,
			NULL,				//������
			NULL,				//��ȫ������
			NULL,				//�߳������Ƿ�ɼ̳�
			FALSE,				//�Ƿ�ӵ��ý��̳��̳��˾��
			DEBUG_PROCESS,		//�����DEBUG_ONLY_THIS_PROCESS,ֻ�������һ�����̶����������ӽ���һ��
			NULL,				//�½��̵Ļ�����
			NULL,				//�½��̵ĵ�ǰ����·��(��ǰĿ¼)
			&stcStartupInfo,	//ָ���½��̵�����������
			&stcProcInfo		//�����½��̵�ʶ����Ϣ
			))
		{
			OutputDebug(L"�������Խ���ʧ��");
			return 0;
		}
			
			
			
			;
		//DEBUG_PROCESS
		//DEBUG_ONLY_THIS_PROCESS
	}
	else
	{
		DebugActiveProcess(*((DWORD*)Infor));//����DWORDֵ,�����Ǵ���DWORDָ��.
	}

	GetEvent();

	return 1;
}

bool GetEvent()
{
	while(1==WaitForDebugEvent(&DbgEvt,INFINITE))
	{
		switch (DbgEvt.dwDebugEventCode)
		{		
			//�쳣����
		case EXCEPTION_DEBUG_EVENT:
			{
				dwRet= OnExceptionDebugEvent(&DbgEvt.u.Exception);
				break;
			}	
			//���̴���
		case CREATE_PROCESS_DEBUG_EVENT:
			{

				OutputDebug(L"���̴����¼�");

				LPVOID	lpIntAddress	= DbgEvt.u.CreateProcessInfo.lpStartAddress;
				DWORD	dwRet	= 0;
				BYTE	byBack	= 0;
				BYTE	byInt3  = 0xCC;
						g_hProc   = OpenProcess(PROCESS_ALL_ACCESS,0,DbgEvt.dwProcessId);
				if (NULL==g_hProc)
				{
					OutputDebug(L"�򿪽���ʧ��,����Ϊ:%p,FILE: %s, LINE: %d��",DbgEvt.dwProcessId,__FILE__,__LINE__);
					break;
				}
				//����ԭָ��
				if(!ReadProcessMemory(g_hProc,lpIntAddress,&byBack,1,&dwRet))
				{
					OutputDebug(L"���ڴ��ַʧ��,��ַΪ:%p,FILE: %s, LINE: %d��",lpIntAddress,__FILE__,__LINE__);
					break;
				}
				//д��int3�ϵ�
				if(!WriteProcessMemory(g_hProc,lpIntAddress,&byInt3,1,&dwRet))
				{	
					OutputDebug(L"д�ڴ��ַʧ��,��ַΪ:%p,FILE: %s, LINE: %d��",lpIntAddress,__FILE__,__LINE__);
					break;
				}

				wchar_t szOPCode[64]	= {0};
				wchar_t szASM[64]		= {0};
				wchar_t szComment[64]	= {0};
				DWORD	dwTempAddr		= (DWORD)lpIntAddress;//��ʼ������ַ
				DWORD   dwOPCodeLen		= 0;

				for (;dwOPCodeLen!=-1;)
				{
				dwOPCodeLen = DBG_Disasm((LPVOID)dwTempAddr,szOPCode,szASM,szComment);
				dwTempAddr+=dwOPCodeLen;
				OutputDebug(L"Addr:0x%p %-12s %-16s %s\n",dwTempAddr,szOPCode,szASM,szComment);
				}

				break;
			}
			//�����˳�
		case EXIT_PROCESS_DEBUG_EVENT:
			{
				break;
			}
			//�̴߳���
		case CREATE_THREAD_DEBUG_EVENT:
			{
				break;
			}
			//�߳��˳�
		case EXIT_THREAD_DEBUG_EVENT:
			{
				break;
			}
			//ӳ��DLL
		case LOAD_DLL_DEBUG_EVENT:
			{
				break;
			}
			//ж��DLL
		case UNLOAD_DLL_DEBUG_EVENT:
			{
				break;
			}
			//�����ַ������
		case OUTPUT_DEBUG_STRING_EVENT:
			{
				break;
			}
			//RIP(�ڲ�����)
		case RIP_EVENT:
			{
				break;
			}

		default:
			{
				break;
			}
		}
		ContinueDebugEvent(
			DbgEvt.dwProcessId,
			DbgEvt.dwThreadId,
			dwRet);
		//dwRet��ֵ:
		//DBG_CONTINUE//�쳣�Ѿ�����
		//DBG_EXCEPTION_NOT_HANDLED//��Ҫϵͳ��������
	}
	return 1;
}


DWORD OnExceptionDebugEvent(LPEXCEPTION_DEBUG_INFO pDbgInfo)
{
	EXCEPTION_RECORD* pstcExp = &pDbgInfo->ExceptionRecord;

	DWORD	dwExpCode	= pstcExp->ExceptionCode;	//�쳣����
	PVOID	lpExpAddr	= pstcExp->ExceptionAddress;//�쳣��ַ

	switch (dwExpCode)
	{
		//�Ƿ������쳣
	case EXCEPTION_ACCESS_VIOLATION:
		{
			break;
		}
		//�ϵ��쳣
	case EXCEPTION_BREAKPOINT:
		{
			OutputDebug(L"Addr:0x%p",lpExpAddr);
			break;
		}
		//�ڴ�����쳣
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		{
			break;
		}
		//��Чָ��
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		{
			break;
		}
		//�������
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		{
			break;
		}
		//ָ�֧�ֵ�ǰģʽ
	case EXCEPTION_PRIV_INSTRUCTION:
		{
			break;
		}
		//������Ӳ���ϵ��쳣
	case EXCEPTION_SINGLE_STEP:
		{
			break;
		}

	default:
		break;
	}
	return DBG_EXCEPTION_NOT_HANDLED;////DBG_CONTINUE//�쳣�Ѿ�����

}