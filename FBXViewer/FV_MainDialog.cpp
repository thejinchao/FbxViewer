/***************************************************

    FBX Viewer

    (C) Copyright  JinChao. 2014
***************************************************/

#include "StdAfx.h"
#include "FV_MainDialog.h"
#include "FV_MainView.h"

namespace FbxViewer
{

	//--------------------------------------------------------------------------------------------
	CMainDialog::CMainDialog()
	{
	}

	//--------------------------------------------------------------------------------------------
	CMainDialog::~CMainDialog()
	{
	}

	//--------------------------------------------------------------------------------------------
	LRESULT CMainDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//move to default position
		SetWindowPos(0, 100, 100, 0, 0, SWP_NOSIZE);
		//Add menu
		SetMenu(LoadMenu(0, MAKEINTRESOURCE(IDR_MENU_MAIN)));

		return (LRESULT)TRUE;
	}

	//--------------------------------------------------------------------------------------------
	LRESULT CMainDialog::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		return 0;
	}

	//--------------------------------------------------------------------------------------------
	LRESULT CMainDialog::OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CFileDialog fileDlg(TRUE, _T("FBX file"), 0, OFN_HIDEREADONLY,
			_T("FBX file(*.fbx)\0*.fbx\0All files(*.*)\0*.*\0\0"), m_hWnd);
		if (IDOK != fileDlg.DoModal()) return 0;

		FV_MainView_LoadFBX(fileDlg.m_szFileName);
		return 0;
	}

	//--------------------------------------------------------------------------------------------
	LRESULT CMainDialog::OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		return 0;
	}

}
