#pragma once
#include "resource.h"
#include "WTLTabViewCtrl.h"

namespace FbxViewer
{
	class CMainDialog : public CDialogImpl< CMainDialog >
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
			COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnHelpAbout)
			
			REFLECT_NOTIFICATIONS()
		END_MSG_MAP()

		LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnHelpAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

		/*************************************************************************
		Implementation Methods
		*************************************************************************/
	private:
		CWTLTabViewCtrl m_tabMain;

		/*************************************************************************
		Implementation Data
		*************************************************************************/
	private:

		/*************************************************************************
		Construction and Destruction
		*************************************************************************/
	public:
		CMainDialog();
		~CMainDialog();
	};

}