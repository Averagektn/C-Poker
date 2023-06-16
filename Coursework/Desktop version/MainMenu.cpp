//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <Combinations.h>
#include <pokerWnd.h>
#include "MainMenu.h"
#include <Info.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStartMenu *StartMenu;
//---------------------------------------------------------------------------
__fastcall TStartMenu::TStartMenu(TComponent* Owner) : TForm(Owner) {
}
//---------------------------------------------------------------------------
void __fastcall TStartMenu::btnStartClick(TObject *Sender) {
	StartMenu->Visible = False;
	MainForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TStartMenu::btnCombinationsClick(TObject *Sender) {
	CombinationsForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TStartMenu::btnExitClick(TObject *Sender) {
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TStartMenu::btnInfoClick(TObject *Sender) {
	InfoForm->Show();
}
//---------------------------------------------------------------------------

