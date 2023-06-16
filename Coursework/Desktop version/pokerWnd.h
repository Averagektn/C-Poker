//---------------------------------------------------------------------------

#ifndef pokerWndH
#define pokerWndH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Auxiliary.h>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm {
__published:	// IDE-managed Components
	TImage *imgBackground;

	TImage *imgPlrCrd_1;
	TImage *imgPlrCrd_2;

	TImage *imgComp_1_Crd_1;
	TImage *imgComp_1_Crd_2;

	TImage *imgComp_2_Crd_1;
	TImage *imgComp_2_Crd_2;

	TImage *imgComp_3_Crd_1;
	TImage *imgComp_3_Crd_2;

	TImage *imgComp_4_Crd_1;
	TImage *imgComp_4_Crd_2;

	TImage *imgTblCrd_1;
	TImage *imgTblCrd_2;
	TImage *imgTblCrd_3;
	TImage *imgTblCrd_4;
	TImage *imgTblCrd_5;

	TTrackBar *tbarBet;

	TButton *btnCall;
	TButton *btnCheck;
	TButton *btnFold;
	TButton *btnBet;

	TLabel *lblBet;

	TLabel *lblComp_3;
	TLabel *lblComp_4;
	TLabel *lblComp_2;
	TLabel *lblComp_1;

	TLabel *lblComp_2_Bet;
	TLabel *lblComp_1_Bet;
	TLabel *lblComp_3_Bet;
	TLabel *lblComp_4_Bet;

	TLabel *lblComp_1_Balance;
	TLabel *lblComp_2_Balance;
	TLabel *lblComp_3_Balance;
	TLabel *lblComp_4_Balance;

	TLabel *lblPlrCmb;
	TLabel *lblPlrBalance;
	TLabel *lblPlrBet;

	TLabel *lblTblBank;
	TLabel *lblGamePhase;

	TImageList *imgList;

	TLabel *lblHidden;
	TMainMenu *mmPoker;
	TMenuItem *Combinations;
	TMenuItem *Info;
	TMenuItem *Leave;

	void __fastcall btnBetClick(TObject *Sender);
    void __fastcall btnFoldClick(TObject *Sender);
	void __fastcall btnCallClick(TObject *Sender);
	void __fastcall btnCheckClick(TObject *Sender);

	void __fastcall tbarBetTracking(TObject *Sender);

	void __fastcall imgPlrCrd_1MouseEnter(TObject *Sender);
	void __fastcall imgPlrCrd_2MouseEnter(TObject *Sender);

	void __fastcall imgPlrCrd_1MouseLeave(TObject *Sender);
	void __fastcall imgPlrCrd_2MouseLeave(TObject *Sender);

	void __fastcall lblPlrCmbClick(TObject *Sender);

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CombinationsClick(TObject *Sender);
	void __fastcall InfoClick(TObject *Sender);
	void __fastcall LeaveClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	  TLabel* lblBets[PLAYERS];
	  TLabel* lblBalances[PLAYERS];

	  TImage* imgTableCards[TABLE_CARDS];
	  TImage* imgPlayerCards[PLAYERS * 2];

	  Player players[PLAYERS];
	  Table table;
	  Card deck[DECK_SIZE];

	  int score;

	  const int PICTURE_SHIFT = 10;

	  const char* combinations[COMBINATIONS] =
	 {"error", "high card", "pair", "two pairs", "three of a kind", "straight",
	  "flush", "full house", "four of a kind", "straight flush", "royal flush"};

	  const char* winnerNames[PLAYERS] =
	  {"You", "Dablo Kescobar", "Tequila Sunset", "Paquette", "Raphael Costeau"};

	  void paintCard(Card card, TImage* img);
	  void disableButtons();
	  void clearBets();
	  void updateComputerInfo(int ind, int (*strat)(Player*, Table*, Card*, int));
	  void resetDeck();
	  void processCardsNum();
	  void anotherTurn();

	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

//Change card images
