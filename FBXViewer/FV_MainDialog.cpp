/***************************************************

    FBX Viewer

    (C) Copyright  JinChao. 2014
***************************************************/

#include "StdAfx.h"
#include "FV_MainDialog.h"
#include "FV_MainView.h"
#include "FV_SceneContext.h"

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

		//Set anim buddy
		SendMessage((HWND)GetDlgItem(IDC_SLIDER_ANIM), TBM_SETBUDDY, (WPARAM)TRUE, (LPARAM)(HWND)GetDlgItem(IDC_STATIC_ANIM_LEFT));
		SendMessage((HWND)GetDlgItem(IDC_SLIDER_ANIM), TBM_SETBUDDY, (WPARAM)FALSE, (LPARAM)(HWND)GetDlgItem(IDC_STATIC_ANIM_RIGHT));

		//set auto play
		CheckDlgButton(IDC_CHECK_PLAY, BST_CHECKED);

		DlgResize_Init(false);

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

		//refresh gui
		_refreshGUIInfo();
		return 0;
	}

	//--------------------------------------------------------------------------------------------
	LRESULT CMainDialog::OnAppExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		::PostQuitMessage(0);
		return 0;
	}

	//--------------------------------------------------------------------------------------------
	LRESULT CMainDialog::OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		return 0;
	}

	//--------------------------------------------------------------------------------------------
	void CMainDialog::_refreshGUIInfo(void)
	{
		SceneContext* pSceneContext = FV_GetSceneContext();
		const FbxScene* pFbxScene = pSceneContext->GetScene();

		//------------------
		//Refresh Animation info
		//------------------
		HWND hAnimCombo = GetDlgItem(IDC_COMBO_ANIM_LIST);
		while (ComboBox_GetCount(hAnimCombo) > 0) ComboBox_DeleteString(hAnimCombo, 0);
		for (int i = 0; i < pFbxScene->GetSrcObjectCount<FbxAnimStack>(); i++)
		{
			FbxAnimStack* lAnimStack = pFbxScene->GetSrcObject<FbxAnimStack>(i);

			int index = ComboBox_AddString(hAnimCombo, lAnimStack->GetName());
		}

		if (ComboBox_GetCount(hAnimCombo)>0)
		{
			ComboBox_SetCurSel(hAnimCombo, 0);
			_onAnimationSelectChange(0);
		}
	}

	//--------------------------------------------------------------------------------------------
	unsigned int _getCurveKeyCounts(const FbxAnimCurve *pCurve)
	{
		int lKeyCount = pCurve->KeyGetCount();
		if (lKeyCount <= 0) return 0;

		FbxTime lKeyTime = pCurve->KeyGetTime(lKeyCount-1);
		return (unsigned int)lKeyTime.GetFrameCount();
	}

	//--------------------------------------------------------------------------------------------
	unsigned int _getFrameCounts(FbxAnimLayer* pAnimLayer, FbxNode* pNode)
	{
		const char* CHANNEL_NAMES[] = {
			FBXSDK_CURVENODE_COMPONENT_X, FBXSDK_CURVENODE_COMPONENT_Y, FBXSDK_CURVENODE_COMPONENT_Z,
		};

		for (int i = 0; i < 3; i++)
		{
			FbxAnimCurve* lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, CHANNEL_NAMES[i]);
			if (lAnimCurve &&  lAnimCurve->KeyGetCount() > 0) return _getCurveKeyCounts(lAnimCurve);

			lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, CHANNEL_NAMES[i]);
			if (lAnimCurve &&  lAnimCurve->KeyGetCount() > 0) return _getCurveKeyCounts(lAnimCurve);

			lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, CHANNEL_NAMES[i]);
			if (lAnimCurve &&  lAnimCurve->KeyGetCount() > 0) return _getCurveKeyCounts(lAnimCurve);
		}

		for (int lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
		{
			int counts = _getFrameCounts(pAnimLayer, pNode->GetChild(lModelCount));
			if (counts) return counts;
		}

		return 0;
	}

	//--------------------------------------------------------------------------------------------
	void CMainDialog::_onAnimationSelectChange(int index)
	{
		SceneContext* pSceneContext = FV_GetSceneContext();
		const FbxScene* pFbxScene = pSceneContext->GetScene();

		FbxAnimStack* lAnimStack = pFbxScene->GetSrcObject<FbxAnimStack>(index);
		if (lAnimStack == 0) return;

		FbxAnimLayer* lAnimLayer = lAnimStack->GetMember<FbxAnimLayer>(0);
		if (lAnimLayer == 0) return;

		int frameCounts = _getFrameCounts(lAnimLayer, pFbxScene->GetRootNode());

		HWND hAnimSlider = (HWND)GetDlgItem(IDC_SLIDER_ANIM);
		SendMessage(hAnimSlider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, frameCounts));

		SetDlgItemTextA(IDC_STATIC_ANIM_LEFT, "0");
		char szTemp[32]; _snprintf_s(szTemp, 32, "%d", frameCounts);
		SetDlgItemTextA(IDC_STATIC_ANIM_RIGHT, szTemp);
		SetDlgItemTextA(IDC_EDIT_FRAME, "0");

		//Set anim to view
		FV_GetSceneContext()->SetCurrentAnimStack(index);
	}

	//--------------------------------------------------------------------------------------------
	LRESULT CMainDialog::OnCheckPlay(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bool isPlay = (IsDlgButtonChecked(IDC_CHECK_PLAY) == BST_CHECKED);
		FV_GetSceneContext()->SetPause(!isPlay);

		::EnableWindow((HWND)GetDlgItem(IDC_SLIDER_ANIM), !isPlay);
		::EnableWindow((HWND)GetDlgItem(IDC_EDIT_FRAME), !isPlay);

		return 0;
	}

	//--------------------------------------------------------------------------------------------
	LRESULT CMainDialog::OnAnimationComboCommand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		switch (wNotifyCode)
		{
		case CBN_SELCHANGE:
			HWND hAnimCombo = GetDlgItem(IDC_COMBO_ANIM_LIST);
			int index = ComboBox_GetCurSel(hAnimCombo);
			_onAnimationSelectChange(index);
			break;
		}

		return 0;
	}

	//--------------------------------------------------------------------------------------------
	LRESULT CMainDialog::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HWND hAnimSlider = (HWND)GetDlgItem(IDC_SLIDER_ANIM);
		if ((HWND)lParam == hAnimSlider)
		{
			switch (LOWORD(wParam))
			{
			case SB_THUMBTRACK:
				DWORD dwPos = (DWORD)SendMessage(hAnimSlider, TBM_GETPOS, 0, 0);
				SetDlgItemInt(IDC_EDIT_FRAME, dwPos);
				FV_GetSceneContext()->setAnimFrame(dwPos);
				break;
			}
		}
		return 0;
	}
}
