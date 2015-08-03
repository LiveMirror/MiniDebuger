#include <Windows.h>
#include "Debug.h"
#include "DealEvent.h"
#include "..\Interface.h"

_DEBUG_EVENT	DbgEvt	= {0};		
DWORD	dwRet			= DBG_EXCEPTION_NOT_HANDLED;//δ����!!!!!!!!!!!dwRet�д���ʹ�õĴ����ƺ�
STARTUPINFO			stcStartupInfo	= {sizeof(STARTUPINFO)};
PROCESS_INFORMATION stcProcInfo		= {0};//������Ϣ
extern HANDLE g_hProc;//�������ߵľ��

int StartDebug(bool bNewProcess,void *Infor);
bool GetEvent();
//����Ƿ����ڵ���״̬
bool Debuging=false;



//////////////////////////////////////////////////////////////////////////
//bool bNewProcess	:�Ե��Եķ�ʽ����һ������(1),���ӵ�һ������(0)
//void *Infor		:ָ��һ��wchar_t�ַ�������DWORDֵ,��ȡ����bNewProcess.
int StartDebug(bool bNewProcess,void *Infor)
{
	//�жϵ�ǰ����״̬
	if (true==Debuging)
	{
		return -1;
	}

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

	Debuging=true;

	GetEvent();

	return 1;
}

bool GetEvent()
{
	while(1==WaitForDebugEvent(&DbgEvt,INFINITE))
	{
		//����Ƿ������Ҫ���õĺ���,�������������ṩ
		AutoAnalysisCommandParsing();

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
		//DBG_EXCEPTION_NOT_HANDLED:�쳣��Ҫϵͳ��������(���ǵ�Ĭ��)
	}
	return false;
}


bool Detach()
{
	if (0!=DbgEvt.dwProcessId)
	{
		DebugActiveProcessStop(DbgEvt.dwProcessId);
		Debuging=false;
		return true;
	}
	
	return false;
	
}


//bLove==true   �����߱���,�������߲����˳�
//bLove==false  �෴.Ĭ�����.
//����:false��ʾʧ��.
bool LoveIt(bool bLove)
{
	if (0==DebugSetProcessKillOnExit(!bLove))
	{
		return false;
	}

	return true;
}


