//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("pokerWnd.cpp", MainForm);
USEFORM("Combinations.cpp", CombinationsForm);
USEFORM("MainMenu.cpp", StartMenu);
USEFORM("Info.cpp", InfoForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TStartMenu), &StartMenu);
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TCombinationsForm), &CombinationsForm);
		Application->CreateForm(__classid(TInfoForm), &InfoForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
