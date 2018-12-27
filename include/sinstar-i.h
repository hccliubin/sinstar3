#pragma once

#include "TextService-i.h"


interface ISinstar : public IUnknown
{
	virtual void OnIMESelect(BOOL bSelect)=NULL;
	virtual void OnCompositionStarted()=NULL;
	virtual void OnCompositionChanged()=NULL;
	virtual void OnCompositionTerminated(bool bClearCtx)=NULL;
	virtual void OnSetCaretPosition(POINT pt,int nHei)=NULL;
	virtual void OnSetFocusSegmentPosition(POINT pt,int nHei)=NULL;
	virtual void ProcessKeyStoke(LPVOID lpImeContext,UINT vkCode,LPARAM lParam,BOOL bKeyDown,BOOL *pbEaten)=NULL;
	virtual void TranslateKey(LPVOID lpImeContext,UINT vkCode,UINT uScanCode,BOOL bKeyDown,BOOL *pbEaten)=NULL;
	virtual void OnSetFocus(BOOL bFocus)=NULL;
	virtual int  GetCompositionSegments()=NULL;
	virtual int  GetCompositionSegmentEnd(int iSeg)=NULL;
	virtual int	 GetCompositionSegmentAttr(int iSeg)=NULL;
	virtual void OnOpenStatusChanged(BOOL bOpen)=NULL;
	virtual void OnConversionModeChanged(EInputMethod uMode)=NULL;
	virtual void ShowHelp()=NULL;
	virtual EInputMethod GetDefInputMode() = NULL;
};

