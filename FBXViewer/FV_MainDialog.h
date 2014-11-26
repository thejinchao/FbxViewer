#pragma once
#include "resource.h"
#include "WTLTabViewCtrl.h"

namespace FbxViewer
{
	class CMainDialog : public CAxDialogImpl< CMainDialog >, public CDialogResize< CMainDialog >
	{
		/*************************************************************************
		Public Methods
		*************************************************************************/
	public:

		/*************************************************************************
		Inherit Methods
		*************************************************************************/
	public:
		enum { IDD = IDD_DIALOG_MAIN };

		BEGIN_MSG_MAP(CMainDialog)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
			COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
			COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
			COMMAND_ID_HANDLER(ID_APP_EXIT, OnAppExit)
			COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnHelpAbout)
			COMMAND_ID_HANDLER(IDC_CHECK_PLAY, OnCheckPlay)
			COMMAND_ID_HANDLER(IDC_COMBO_ANIM_LIST, OnAnimationComboCommand)
			MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
			CHAIN_MSG_MAP(CDialogResize< CMainDialog >)
			REFLECT_NOTIFICATIONS()
		END_MSG_MAP()

		BEGIN_DLGRESIZE_MAP(CMainDialog)
			DLGRESIZE_CONTROL(IDC_STATIC_ANIM_GROUP, DLSZ_SIZE_X)
			DLGRESIZE_CONTROL(IDC_SLIDER_ANIM, DLSZ_SIZE_X)
			DLGRESIZE_CONTROL(IDC_EDIT_FRAME, DLSZ_MOVE_X)
		END_DLGRESIZE_MAP()

		LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnAppExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnCheckPlay(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnAnimationComboCommand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual BOOL TranslateMessage(MSG* pMsg) { return ::IsDialogMessage(m_hWnd, pMsg); }
		/*************************************************************************
		Implementation Methods
		*************************************************************************/
	private:
		void _refreshGUIInfo(void);
		void _onAnimationSelectChange(int index);

		/*************************************************************************
		Implementation Data
		*************************************************************************/
	private:
		CWTLTabViewCtrl m_tabMain;

		/*************************************************************************
		Construction and Destruction
		*************************************************************************/
	public:
		CMainDialog();
		~CMainDialog();
	};

}