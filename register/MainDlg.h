// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

class CMainDlg : public SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

protected:
	//soui消息
	void OnClose();
	void OnMinimize();
	void OnAgree();

	void OnInstall();
	void OnUninstall();
	void OnUpdate();
	void OnDonate();

	EVENT_MAP_BEGIN()
		EVENT_ID_COMMAND(R.id.btn_close, OnClose)
		EVENT_ID_COMMAND(R.id.btn_min, OnMinimize)
		EVENT_ID_COMMAND(R.id.chk_agree,OnAgree)
		EVENT_ID_COMMAND(R.id.btn_install,OnInstall)
		EVENT_ID_COMMAND(R.id.btn_uninstall,OnUninstall)
		EVENT_ID_COMMAND(R.id.btn_update, OnUpdate)
		EVENT_ID_COMMAND(R.id.btn_donate,OnDonate)
	EVENT_MAP_END()
		
protected:
	//HostWnd真实窗口消息处理
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
};
