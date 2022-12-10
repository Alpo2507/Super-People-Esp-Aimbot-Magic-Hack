//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
HANDLE hSHThread(0);
DWORD dwSHThread(0);
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
void __fastcall TForm1::FormShow(TObject *Sender)
{
	_CCharBase = *(DWORD*)(KO_CHAR);

}
void __fastcall TForm1::chenabledClick(TObject *Sender)
{
	if(chenabled->Checked)
	{
		hSHThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SHFunction,NULL,0,&dwSHThread);
	}
	else
	{
		TerminateThread(hSHThread,0);
		CloseHandle(hSHThread);
		hSHThread = 0;
		dwSHThread = 0;
    }
}
//---------------------------------------------------------------------------

void SHFunction()
{
int iBoost = atoi(Form1->txtboost->Text.c_str());
int iSleep = atoi(Form1->txtdelay->Text.c_str());

bool bArrivedX = false, bArrivedY = false;

float fCurX,fCurY;
float fPtrX,fPtrY;
float fNewX,fNewY,fNewZ;

DWORD dwTmpVal;

char pBuf[10];	//packet

_CPacket.PutBYTE(pBuf,0x06);
_CPacket.PutWORD(pBuf+7,0x2D);
_CPacket.PutBYTE(pBuf+9,0x00);

LOOP:

if(!GetAsyncKeyState(0xA0))
	goto RETN;

fCurX = _CCharBase.GetFLOAT(KO_POSX);
fCurY = _CCharBase.GetFLOAT(KO_POSY);

fPtrX = _CCharBase.GetFLOAT(KO_PTRX);
fPtrY = _CCharBase.GetFLOAT(KO_PTRY);


if(_CMath.fDist(fCurX,fCurY,fPtrX,fPtrY) > 0.5){

	////////////error handling (test)/////////////
	dwTmpVal = *(DWORD*)(0xC26C30);
	dwTmpVal = *(DWORD*)(dwTmpVal + 0x1C);
	dwTmpVal = *(DWORD*)(dwTmpVal + 0x48);
	if(dwTmpVal){
		dwTmpVal = *(DWORD*)(dwTmpVal + 0x48);
		dwTmpVal = *(DWORD*)(dwTmpVal);
		dwTmpVal = *(DWORD*)(dwTmpVal + 0x8);
		if(!dwTmpVal)
			goto RETN;
	}
	//////////////////////////////////////////////

	if(fCurX != fPtrX){
		bArrivedX = false;
		if(fCurX > fPtrX){
			if(fCurX - fPtrX >= (float)iBoost)
				fNewX = fCurX - (float)iBoost;
			else
				fNewX = fPtrX;
		}
		else
		{
			if(fPtrX - fCurX >= (float)iBoost)
				fNewX = fCurX + (float)iBoost;
			else
				fNewX = fPtrX;
		}
	}else bArrivedX = true;

	if(fCurY != fPtrY){
		bArrivedY = false;
		if(fCurY > fPtrY){
			if(fCurY - fPtrY >= (float)iBoost)
				fNewY = fCurY - (float)iBoost;
			else
				fNewY = fPtrY;
		}
		else
		{
			if(fPtrY - fCurY >= (float)iBoost)
				fNewY = fCurY + (float)iBoost;
			else
				fNewY = fPtrY;
		}
	}else bArrivedY = true;

	fNewZ = _CTest.GetPosZ(fNewX,fNewY);
	_CTest.InitPos(fNewX,fNewY,fNewZ);

	_CPacket.PutWORD(pBuf+1,(int)(_CCharBase.GetFLOAT(KO_POSX)) * 10);
	_CPacket.PutWORD(pBuf+3,(int)(_CCharBase.GetFLOAT(KO_POSY)) * 10);
	_CPacket.PutWORD(pBuf+5,(int)(_CCharBase.GetFLOAT(KO_POSZ)) * 10);
	_CPacket.PutWORD(pBuf+7,0x2D);

	if(*(pBuf+9) == 0){
		*(pBuf+9) = 1;	//start
	}
	else if(*(pBuf+9) == 1){
		*(pBuf+9) = 3;	//moving
	}
	else if(*(pBuf+9) == 3 && bArrivedX && bArrivedY){
		*(pBuf+9) = 0;	//stop
		_CPacket.PutWORD(pBuf+7,0x00);
	}

	_CPacket.PSend(pBuf,10);

}
else{
	if(*(pBuf+9) == 3){
		*(pBuf+9) = 0;	//stop
		_CPacket.PutWORD(pBuf+7,0x00);
		_CPacket.PutWORD(pBuf+1,(int)(_CCharBase.GetFLOAT(KO_POSX)) * 10);
		_CPacket.PutWORD(pBuf+3,(int)(_CCharBase.GetFLOAT(KO_POSY)) * 10);
		_CPacket.PutWORD(pBuf+5,(int)(_CCharBase.GetFLOAT(KO_POSZ)) * 10);
		_CPacket.PSend(pBuf,10);
	}
}

RETN:
Sleep(iSleep);
goto LOOP;
}


void __fastcall TForm1::chwhClick(TObject *Sender)
{
	_CCharBase.SetDWORD(KO_AUTH,!chwh->Checked);
}
//---------------------------------------------------------------------------

