#pragma once

#include "ImeWnd.h"
#include "SkinMananger.h"

namespace SOUI
{
	class CStatusWnd :
		public CImeWnd
	{
	public:
		CStatusWnd(void);
		~CStatusWnd(void);

	protected:
		int OnCreate(LPCREATESTRUCT lpCreateStruct);
		void OnDestroy();

		void OnRButtonUp(UINT nFlags,CPoint pt);
		
		void OnInitMenuPopup(HMENU menuPopup, UINT nIndex, BOOL bSysMenu);

		BEGIN_MSG_MAP_EX(CStatusWnd)
			MSG_WM_INITMENUPOPUP(OnInitMenuPopup)
			MSG_WM_CREATE(OnCreate)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_RBUTTONUP(OnRButtonUp)
			CHAIN_MSG_MAP(CImeWnd)
		END_MSG_MAP()

	protected:

		void OnDragStatus(EventArgs *e);
		void OnSetSkin(EventArgs *e);

		EVENT_MAP_BEGIN()
			EVENT_HANDLER(EventDragMove::EventID,OnDragStatus)
			EVENT_HANDLER(EventSetSkin::EventID,OnSetSkin)
		EVENT_MAP_END()
	protected:
		CSkinMananger  m_pSkinManager;
	};

}