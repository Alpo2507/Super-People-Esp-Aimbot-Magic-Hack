//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

void SHFunction();
extern HANDLE hSHThread;
extern DWORD dwSHThread;
extern bool bSH;
//--------------------------------------------------------------------------

class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TEdit *txtboost;
	TEdit *txtdelay;
	TCheckBox *chwh;
	TLabel *Label3;
	TLabel *Label4;
	TCheckBox *chenabled;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall chenabledClick(TObject *Sender);
	void __fastcall chwhClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
