//---------------------------------------------------------------------------

#ifndef MainMenuH
#define MainMenuH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Graphics.hpp>
//---------------------------------------------------------------------------
class TStartMenu : public TForm {
__published:	// IDE-managed Components
	TButton *btnStart;
	TButton *btnInfo;
	TButton *btnCombinations;
	TButton *btnExit;

	TImage *imgBack;

	void __fastcall btnStartClick(TObject *Sender);
	void __fastcall btnCombinationsClick(TObject *Sender);
	void __fastcall btnExitClick(TObject *Sender);
	void __fastcall btnInfoClick(TObject *Sender);

	private:	// User declarations
public:		// User declarations
	__fastcall TStartMenu(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TStartMenu *StartMenu;
//---------------------------------------------------------------------------
#endif
