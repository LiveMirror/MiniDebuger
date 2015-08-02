#include <Windows.h>
#include "Debug.h"
#include "DealEvent.h"

_DEBUG_EVENT	DbgEvt	= {0};		
DWORD	dwRet			= DBG_EXCEPTION_NOT_HANDLED;//δ����!!!!!!!!!!!dwRet�д���ʹ�õĴ����ƺ�
STARTUPINFO			stcStartupInfo	= {sizeof(STARTUPINFO)};
PROCESS_INFORMATION stcProcInfo		= {0};//������Ϣ
extern HANDLE g_hProc;//�������ߵľ��

int StartDebug(bool bNewProcess,void *Infor);
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
			return 1;
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
				Deal_CPEV();
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
		//DBG_CONTINUE:				�쳣�Ѿ�����
		//DBG_EXCEPTION_NOT_HANDLED:�쳣��Ҫϵͳ��������
	}
	return 1;
}


