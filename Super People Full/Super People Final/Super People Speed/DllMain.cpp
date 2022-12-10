//---------------------------------------------------------------------------

#include <windows.h>
#include "MainForm.h"
#pragma hdrstop
#pragma argsused

long WINAPI FormThread(long lParam)
{
Form1=new TForm1(NULL);
Application->Initialize();
Application->CreateForm(__classid(TForm1), &Form1);
Application->Run();
Application->ProcessMessages();
Form1->Show();



	return true;
}
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
switch (reason)
{
case DLL_PROCESS_ATTACH:
CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)FormThread,NULL,0,0);
break;
case DLL_PROCESS_DETACH:
ExitProcess(1) ;
break;
}
		return 1;
}
//---------------------------------------------------------------------------

