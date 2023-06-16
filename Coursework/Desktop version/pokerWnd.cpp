#include <vcl.h>
#pragma hdrstop

#include "pokerWnd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include <MainMenu.h>
#include <Combinations.h>
#include <string.h>
#include <Info.h>

#define SLEEP_TIME 750

TMainForm *MainForm;

void TMainForm::disableButtons() {
	btnCall->Enabled = False;
	btnBet->Enabled = False;
	btnCheck->Enabled = False;
	btnFold->Enabled = False;

	tbarBet->Enabled = False;
}

void TMainForm::clearBets() {
	for (int i = 1; i < PLAYERS; i++) {
		lblBets[i]->Caption = "";
	}
}

void TMainForm::anotherTurn(){
	int bank = 0;

	if (countPlayers(players) == 1) {
		processCardsNum();
		return;
	}

	clearBets();

	for (int i = 1; i < PLAYERS; i++) {
		if (players[i].isPlaying) {
			Application->ProcessMessages();
			Sleep(SLEEP_TIME);
		}
		 updateComputerInfo(i, chooseStrategy);
		 bank += players[i].bet;
	}

	lblTblBank->Caption = "Bank: ";
	lblTblBank->Caption += table.atStake;

	Application->ProcessMessages();
	Sleep(SLEEP_TIME);

	clearBets();

	if (bank != table.call * countPlayers(players)) {
		for (int i = 1; i < PLAYERS; i++) {
			if (players[i].isPlaying) {
				Application->ProcessMessages();
				Sleep(SLEEP_TIME);
			}
			updateComputerInfo(i, callAfterBets);
		  }
	}

	Application->ProcessMessages();
	Sleep(SLEEP_TIME);

	lblTblBank->Caption = "Bank: ";
	lblTblBank->Caption += table.atStake;

	processCardsNum();
}

void TMainForm::resetDeck(){
	for (int i = 0; i < DECK_SIZE; i++) {
			deck[i].value = i % VALUES;
			deck[i].suit = i / VALUES;
	}
}

void TMainForm::paintCard(Card card, TImage* img){
	int ind;

	Application->ProcessMessages();
	Sleep(SLEEP_TIME);

	img->Picture->Bitmap = NULL;
	ind = card.suit * VALUES + card.value;
	imgList->GetBitmap(ind, img->Picture->Bitmap);
}

 void TMainForm::processCardsNum(){
	int winnerId;

	switch(table.cardsNum){
	case PREFLOP_CARDS:
		for (int i = 0; i < FLOP_CARDS; i++) {
			paintCard(table.layout[i], imgTableCards[i]);
		}
		table.cardsNum = FLOP_CARDS;
		lblGamePhase->Caption = "Flop";
		break;
	case FLOP_CARDS:
		paintCard(table.layout[TURN_CARD], imgTableCards[TURN_CARD]);
		table.cardsNum = TURN_CARDS;
		lblGamePhase->Caption = "Turn";
		break;
	case TURN_CARDS:
		paintCard(table.layout[RIVER_CARD], imgTableCards[RIVER_CARD]);
		table.cardsNum = RIVER_CARDS;
		lblGamePhase->Caption = "River";
		break;
	case RIVER_CARDS:
		for (int i = 1; i < PLAYERS; i++) {
			paintCard(players[i].layout[FIRST_CARD], imgPlayerCards[i * 2]);
			paintCard(players[i].layout[SECOND_CARD], imgPlayerCards[i * 2 + 1]);
		}
		table.cardsNum = RIVER_CARDS + 1;

		Application->ProcessMessages();
		Sleep(SLEEP_TIME);

		winnerId = findWinner(players, table, PLAYERS);

		if (winnerId == DRAW) {
			lblHidden->Caption = "Draw with combination of ";
			lblHidden->Caption += combinations[findCombinations(table.layout, &players[winnerId], RIVER_CARDS)];
			ShowMessage(lblHidden->Caption);
		} else {
			lblHidden->Caption = winnerNames[winnerId];
			lblHidden->Caption += " won with combination of ";
			lblHidden->Caption += combinations[findCombinations(table.layout, &players[winnerId], RIVER_CARDS)];
			ShowMessage(lblHidden->Caption);
			players[winnerId].balance += table.atStake;
		}
		break;
	default:
		break;
	}
 }

void TMainForm::updateComputerInfo(int ind, int (*strat)(Player*, Table*, Card*, int)) {
	int strategy, tmp;

	strategy = strat(&players[ind], &table, deck, table.cardsNum);

	switch (strategy) {
	case OUT_OF_GAME:
		lblBets[ind]->Caption = "IS OUT";
		break;
	case FOLD:
		fold(&players[ind]);
		lblBets[ind]->Caption = "FOLD";
		break;
	 case CALL:
		tmp = table.call - players[ind].bet;
		if (players[ind].balance + players[ind].bet <= table.call) {
			allIn(&players[ind], &table);
			lblBets[ind]->Caption = "ALL IN";
		} else if (tmp) {
		  call(&players[ind], &table);
		  lblBets[ind]->Caption = "CALL ";
		  lblBets[ind]->Caption += tmp;
		} else {
			lblBets[ind]->Caption = "CHECK";
		}
		break;
	 case CHECK:
		if (table.call - players[ind].bet) {
			fold(&players[ind]);
			lblBets[ind]->Caption = "FOLD";
		} else {
			lblBets[ind]->Caption = "CHECK";
		}
		break;
	  case BET:
		if (players[ind].balance + players[ind].bet <= table.call) {
			allIn(&players[ind], &table);
			lblBets[ind]->Caption = "ALL IN";
		} else if (players[ind].bet + players[ind].balance != table.call) {
			tmp = bet(&players[ind], &table);
			lblBets[ind]->Caption = "BET ";
			lblBets[ind]->Caption += tmp;
		} else {
			lblBets[ind]->Caption = "CHECK";
		}
		break;
	  case ALL_IN:
		lblBets[ind]->Caption = "ALL IN";
		break;
	default:
		break;
	}
	lblBalances[ind]->Caption = "Balance: ";
	lblBalances[ind]->Caption += players[ind].balance;
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::btnBetClick(TObject *Sender) {
	disableButtons();

	int stake = tbarBet->Position;

	clearBets();

	btnBet->Caption = "Bet";

	playersTurn(&players[PLAYER], &table, table.cardsNum, stake);

	MainForm->Caption = "Poker Balance: ";
	MainForm->Caption += players[PLAYER].balance;
	MainForm->Caption += " Best score: ";
	MainForm->Caption += score;

	lblPlrBalance->Caption = "Your balance: ";
	lblPlrBalance->Caption += players[PLAYER].balance;

	lblBets[PLAYER]->Caption = "Your bet: ";
	lblBets[PLAYER]->Caption += stake;
	lblBet->Caption = tbarBet->Position;

	tbarBet->Position = 0;
	tbarBet->Max = players[PLAYER].balance;

	lblBet->Caption = tbarBet->Position;

	for (int i = 1; i < PLAYERS; i++) {
		if (players[i].isPlaying) {
			Application->ProcessMessages ();
			Sleep(SLEEP_TIME);
		}
		updateComputerInfo(i, chooseStrategy);
	}

	lblTblBank->Caption = "Bank: ";
	lblTblBank->Caption += table.atStake;

	if (table.call > players[PLAYER].bet) {
		if (table.call <= players[PLAYER].bet + players[PLAYER].balance) {
			btnCall->Caption = "Call ";
			btnCall->Caption += table.call - players[PLAYER].bet;
			btnCall->Enabled = True;
			btnFold->Enabled = True;
		}
	} else {
		processCardsNum();

		lblPlrCmb->Caption = "Your combination: ";
		lblPlrCmb->Caption += combinations[findCombinations(table.layout, &players[PLAYER], table.cardsNum)];

		tbarBet->Enabled = True;
		btnBet->Enabled = True;
		btnCheck->Enabled = True;
		btnFold->Enabled = True;
	}

	if (players[PLAYER].isAllIn || countPlayers(players) == 1) {
		disableButtons();
		while (table.cardsNum <= RIVER_CARDS){
			anotherTurn();
		}
	}

	if (table.cardsNum > RIVER_CARDS) {
		FormCreate(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::btnFoldClick(TObject *Sender) {
	disableButtons();

	tbarBet->Position = 0;
	lblBet->Caption = tbarBet->Position;
	lblPlrCmb->Caption = "You are out";

	players[PLAYER].isPlaying = FALSE;

	while (table.cardsNum <= RIVER_CARDS){
		anotherTurn();
	}

	FormCreate(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::btnCallClick(TObject *Sender) {
	disableButtons();

	tbarBet->Position = 0;
	lblBet->Caption = tbarBet->Position;

	int stake = table.call - players[PLAYER].bet;

	clearBets();

	btnCall->Caption = "Call";

	playersTurn(&players[PLAYER], &table, table.cardsNum, stake);

	MainForm->Caption = "Poker Balance: ";
	MainForm->Caption += players[PLAYER].balance;
	MainForm->Caption += " Best score: ";
	MainForm->Caption += score;

	lblPlrBalance->Caption = "Your balance: ";
	lblPlrBalance->Caption += players[PLAYER].balance;

	lblBets[PLAYER]->Caption = "Your bet: ";
	lblBets[PLAYER]->Caption += stake;

	tbarBet->Max = players[PLAYER].balance;

	for (int i = 1; i < PLAYERS; i++) {
		Application->ProcessMessages();
		Sleep(SLEEP_TIME);
		updateComputerInfo(i, callAfterBets);
	}

	lblTblBank->Caption = "Bank: ";
	lblTblBank->Caption += table.atStake;

	processCardsNum();

	lblPlrCmb->Caption = "Your combination: ";
	lblPlrCmb->Caption += combinations[findCombinations(table.layout, &players[PLAYER], table.cardsNum)];

	tbarBet->Enabled = True;
	btnCall->Enabled = False;
	btnBet->Enabled = True;
	btnCheck->Enabled = True;

	if (table.cardsNum > RIVER_CARDS) {
		FormCreate(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::btnCheckClick(TObject *Sender) {
	disableButtons();

	tbarBet->Position = 0;
	lblBet->Caption = tbarBet->Position;

	lblBet->Caption = tbarBet->Position;

	btnBetClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender) {
	int ind;

	if (players[PLAYER].balance == 0) {
		ShowMessage("LOSE");
		Close();
	}

	lblGamePhase->Caption = "Preflop";

	MainForm->Caption = "Poker Balance: ";
	MainForm->Caption += players[PLAYER].balance;
	MainForm->Caption += " Best score: ";
	MainForm->Caption += score;

	btnCall->Caption = "Call";
	btnBet->Caption = "Bet";

	resetDeck();

	tbarBet->Enabled = True;

	btnBet->Enabled = True;
	btnCheck->Enabled = True;
	btnFold->Enabled = True;
	btnCall->Enabled = False;

	tbarBet->Max = players[PLAYER].balance;
	tbarBet->Position = 0;

	for (int i = 1; i < PLAYERS; i++) {
		if (!players[i].balance) {
			lblHidden->Caption = winnerNames[i];
			lblHidden->Caption += " rebuys";
			ShowMessage(lblHidden->Caption);
			players[i].balance = START_BALANCE;
		}
	}

	lblBet->Caption = "0";

	initializePlayers(players);

	cardsDealing(players, deck, PLAYERS);
	clearTable(&table);

	for (int i = 0; i < TABLE_CARDS; i++) {
		dealCard(table.layout, deck, i);
	}

	for (int i = 0; i < TABLE_CARDS; i++) {
		imgTableCards[i]->Picture->Bitmap = NULL;
	}

	for (int i = 0; i < PLAYERS * 2; i++) {
		imgPlayerCards[i]->Picture->Bitmap = NULL;
	}

	for (int i = 0; i < TABLE_CARDS; i++) {
		imgList->GetBitmap(CARD_BACK, imgTableCards[i]->Picture->Bitmap);
	}

	ind = players[PLAYER].layout[FIRST_CARD].suit * VALUES + players[PLAYER].layout[FIRST_CARD].value;
	imgList->GetBitmap(ind, imgPlayerCards[FIRST_CARD]->Picture->Bitmap);

	ind = players[PLAYER].layout[SECOND_CARD].suit * VALUES + players[PLAYER].layout[SECOND_CARD].value;
	imgList->GetBitmap(ind, imgPlayerCards[SECOND_CARD]->Picture->Bitmap);

	for (int i = 2; i < PLAYERS * 2; i++) {
		imgList->GetBitmap(CARD_BACK, imgPlayerCards[i]->Picture->Bitmap);
	}

	lblBalances[PLAYER]->Caption = "Your balance: ";
	lblBalances[PLAYER]->Caption += players[PLAYER].balance;

	for (int i = 1; i < PLAYERS; i++) {
		lblBalances[i]->Caption = "Balance: ";
		lblBalances[i]->Caption += players[i].balance;
		lblBets[i]->Caption = "";
	}

	lblBets[PLAYER]->Caption = "Your bet: 0";

	lblTblBank->Caption = "Bank: 0";

	lblPlrCmb->Caption = "Your combination: ";
	lblPlrCmb->Caption += combinations[findCombinations(table.layout, &players[PLAYER], table.cardsNum)];
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::tbarBetTracking(TObject *Sender) {
	lblBet->Caption = tbarBet->Position;

	btnBet->Caption = "Bet ";
	btnBet->Caption += tbarBet->Position;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::imgPlrCrd_2MouseEnter(TObject *Sender) {
	imgPlrCrd_2->Top -= PICTURE_SHIFT;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::imgPlrCrd_2MouseLeave(TObject *Sender) {
	imgPlrCrd_2->Top += PICTURE_SHIFT;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::imgPlrCrd_1MouseEnter(TObject *Sender) {
	imgPlrCrd_1->Top -= PICTURE_SHIFT;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::imgPlrCrd_1MouseLeave(TObject *Sender) {
	imgPlrCrd_1->Top += PICTURE_SHIFT;
}
//---------------------------------------------------------------------------

__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner) {
	FILE *file = fopen("score.txt", "r");

	srand(time(NULL));
	initBalance(players);

	if (file) {
		fscanf(file, "%d", &score);
	} else {
		score = players[PLAYER].balance;
	}

	if (score < players[PLAYER].balance) {
		score = players[PLAYER].balance;
	}

	fclose(file);

	lblBalances[PLAYER] = lblPlrBalance;
	lblBalances[1] = lblComp_1_Balance;
	lblBalances[2] = lblComp_2_Balance;
	lblBalances[3] = lblComp_3_Balance;
	lblBalances[4] = lblComp_4_Balance;

	lblBets[PLAYER] = lblPlrBet;
	lblBets[1] = lblComp_1_Bet;
	lblBets[2] = lblComp_2_Bet;
	lblBets[3] = lblComp_3_Bet;
	lblBets[4] = lblComp_4_Bet;

	imgTableCards[0] = imgTblCrd_1;
	imgTableCards[1] = imgTblCrd_2;
	imgTableCards[2] = imgTblCrd_3;
	imgTableCards[3] = imgTblCrd_4;
	imgTableCards[4] = imgTblCrd_5;

	imgPlayerCards[0] = imgPlrCrd_1;
	imgPlayerCards[1] = imgPlrCrd_2;
	imgPlayerCards[2] = imgComp_1_Crd_1;
	imgPlayerCards[3] = imgComp_1_Crd_2;
	imgPlayerCards[4] = imgComp_2_Crd_1;
	imgPlayerCards[5] = imgComp_2_Crd_2;
	imgPlayerCards[6] = imgComp_3_Crd_1;
	imgPlayerCards[7] = imgComp_3_Crd_2;
	imgPlayerCards[8] = imgComp_4_Crd_1;
	imgPlayerCards[9] = imgComp_4_Crd_2;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::lblPlrCmbClick(TObject *Sender) {
	CombinationsForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action) {
	FILE *file = fopen("score.txt", "w");
	if (players[PLAYER].balance > score) {
		fprintf(file, "%d", players[PLAYER].balance);
	} else {
		fprintf(file, "%d", score);
    }
	fclose(file);

	StartMenu->Visible = True;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CombinationsClick(TObject *Sender) {
	CombinationsForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::InfoClick(TObject *Sender) {
	InfoForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LeaveClick(TObject *Sender) {
	Close();
}
//---------------------------------------------------------------------------
