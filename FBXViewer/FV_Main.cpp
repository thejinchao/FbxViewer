/***************************************************

    FBX Viewer

    (C) Copyright  JinChao. 2014
***************************************************/

#include "StdAfx.h"
#include "FV_MainView.h"
#include "FV_MainDialog.h"
CAppModule _AppModule;

//WinMain Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int nCmdShow)
{
	srand(::GetTickCount());

	// Init WTL app module
	::InitCommonControls();
	_AppModule.Init(NULL, hInstance);

	//create main dialog
	FbxViewer::CMainDialog dlg;
	dlg.Create(::GetDesktopWindow());
	dlg.ShowWindow(SW_SHOW);

	//show main view
	FbxViewer::FV_MainView_Enter();

	// Close WTL app module
	_AppModule.Term();
	return 0;
}

