// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "KeyHandlerEditSession.h"
#include "Sinstar3TsfBaseStructure.h"

class CLangBarItemButton;
class CCandidateListUIPresenter;
class CCompositionProcessorEngine;

const DWORD WM_CheckGlobalCompartment = WM_USER;
LRESULT CALLBACK CSinstar3Tsf_WindowProc(HWND wndHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

class CSinstar3Tsf : public CUnknown,
	public ITfTextInputProcessorEx,
    public ITfThreadMgrEventSink,
    public ITfTextEditSink,
    public ITfKeyEventSink,
    public ITfCompositionSink,
    public ITfActiveLanguageProfileNotifySink,
    public ITfThreadFocusSink,
    public ITfFunctionProvider,
    public ITfFnGetPreferredTouchKeyboardLayout
{
public:
    CSinstar3Tsf();
    ~CSinstar3Tsf();

    // ITfTextInputProcessor
    STDMETHODIMP Activate(ITfThreadMgr *pThreadMgr, TfClientId tfClientId) {
        return ActivateEx(pThreadMgr, tfClientId, 0);
    }
    // ITfTextInputProcessorEx
    STDMETHODIMP ActivateEx(ITfThreadMgr *pThreadMgr, TfClientId tfClientId, DWORD dwFlags);
    STDMETHODIMP Deactivate();

    // ITfThreadMgrEventSink
    STDMETHODIMP OnInitDocumentMgr(_In_ ITfDocumentMgr *pDocMgr);
    STDMETHODIMP OnUninitDocumentMgr(_In_ ITfDocumentMgr *pDocMgr);
    STDMETHODIMP OnSetFocus(_In_ ITfDocumentMgr *pDocMgrFocus, _In_ ITfDocumentMgr *pDocMgrPrevFocus);
    STDMETHODIMP OnPushContext(_In_ ITfContext *pContext);
    STDMETHODIMP OnPopContext(_In_ ITfContext *pContext);

    // ITfTextEditSink
    STDMETHODIMP OnEndEdit(__RPC__in_opt ITfContext *pContext, TfEditCookie ecReadOnly, __RPC__in_opt ITfEditRecord *pEditRecord);

    // ITfKeyEventSink
    STDMETHODIMP OnSetFocus(BOOL fForeground);
    STDMETHODIMP OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten);
    STDMETHODIMP OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten);
    STDMETHODIMP OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten);
    STDMETHODIMP OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pIsEaten);
    STDMETHODIMP OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pIsEaten);

    // ITfCompositionSink
    STDMETHODIMP OnCompositionTerminated(TfEditCookie ecWrite, _In_ ITfComposition *pComposition);

    // ITfActiveLanguageProfileNotifySink
    STDMETHODIMP OnActivated(_In_ REFCLSID clsid, _In_ REFGUID guidProfile, _In_ BOOL isActivated);

    // ITfThreadFocusSink
    STDMETHODIMP OnSetThreadFocus();
    STDMETHODIMP OnKillThreadFocus();

    // ITfFunctionProvider
    STDMETHODIMP GetType(__RPC__out GUID *pguid);
    STDMETHODIMP GetDescription(__RPC__deref_out_opt BSTR *pbstrDesc);
    STDMETHODIMP GetFunction(__RPC__in REFGUID rguid, __RPC__in REFIID riid, __RPC__deref_out_opt IUnknown **ppunk);

    // ITfFunction
    STDMETHODIMP GetDisplayName(_Out_ BSTR *pbstrDisplayName);

    // ITfFnGetPreferredTouchKeyboardLayout, it is the Optimized layout feature.
    STDMETHODIMP GetLayout(_Out_ TKBLayoutType *ptkblayoutType, _Out_ WORD *pwPreferredLayoutId);

    // CClassFactory factory callback
    static HRESULT CreateInstance(_In_ IUnknown *pUnkOuter, REFIID riid, _Outptr_ void **ppvObj);

    // utility function for thread manager.
    ITfThreadMgr* _GetThreadMgr() { return _pThreadMgr; }
    TfClientId _GetClientId() { return _tfClientId; }

    // functions for the composition object.
    void _SetComposition(_In_ ITfComposition *pComposition);
    void _TerminateComposition(TfEditCookie ec, _In_ ITfContext *pContext, BOOL isCalledFromDeactivate = FALSE);
    void _SaveCompositionContext(_In_ ITfContext *pContext);

    // key event handlers for composition/candidate/phrase common objects.
    HRESULT _HandleComplete(TfEditCookie ec, _In_ ITfContext *pContext);
    HRESULT _HandleCancel(TfEditCookie ec, _In_ ITfContext *pContext);

    // key event handlers for composition object.
    HRESULT _HandleCompositionInput(TfEditCookie ec, _In_ ITfContext *pContext, WCHAR wch);
    HRESULT _HandleCompositionFinalize(TfEditCookie ec, _In_ ITfContext *pContext, BOOL fCandidateList);
    HRESULT _HandleCompositionConvert(TfEditCookie ec, _In_ ITfContext *pContext, BOOL isWildcardSearch);
    HRESULT _HandleCompositionBackspace(TfEditCookie ec, _In_ ITfContext *pContext);
    HRESULT _HandleCompositionArrowKey(TfEditCookie ec, _In_ ITfContext *pContext, KEYSTROKE_FUNCTION keyFunction);

    // key event handlers for candidate object.
    HRESULT _HandleCandidateFinalize(TfEditCookie ec, _In_ ITfContext *pContext);
    HRESULT _HandleCandidateConvert(TfEditCookie ec, _In_ ITfContext *pContext);
    HRESULT _HandleCandidateArrowKey(TfEditCookie ec, _In_ ITfContext *pContext, _In_ KEYSTROKE_FUNCTION keyFunction);
    HRESULT _HandleCandidateSelectByNumber(TfEditCookie ec, _In_ ITfContext *pContext, _In_ UINT uCode);

    // key event handlers for phrase object.
    HRESULT _HandlePhraseFinalize(TfEditCookie ec, _In_ ITfContext *pContext);
    HRESULT _HandlePhraseArrowKey(TfEditCookie ec, _In_ ITfContext *pContext, _In_ KEYSTROKE_FUNCTION keyFunction);
    HRESULT _HandlePhraseSelectByNumber(TfEditCookie ec, _In_ ITfContext *pContext, _In_ UINT uCode);

    BOOL _IsSecureMode(void) { return (_dwActivateFlags & TF_TMAE_SECUREMODE) ? TRUE : FALSE; }
    BOOL _IsComLess(void) { return (_dwActivateFlags & TF_TMAE_COMLESS) ? TRUE : FALSE; }
    BOOL _IsStoreAppMode(void) { return (_dwActivateFlags & TF_TMF_IMMERSIVEMODE) ? TRUE : FALSE; };

    CCompositionProcessorEngine* GetCompositionProcessorEngine() { return (_pCompositionProcessorEngine); };

    // comless helpers
    static HRESULT CSinstar3Tsf::CreateInstance(REFCLSID rclsid, REFIID riid, _Outptr_result_maybenull_ LPVOID* ppv, _Out_opt_ HINSTANCE* phInst, BOOL isComLessMode);
    static HRESULT CSinstar3Tsf::ComLessCreateInstance(REFGUID rclsid, REFIID riid, _Outptr_result_maybenull_ void **ppv, _Out_opt_ HINSTANCE *phInst);
    static HRESULT CSinstar3Tsf::GetComModuleName(REFGUID rclsid, _Out_writes_(cchPath)WCHAR* wchPath, DWORD cchPath);

private:
    // functions for the composition object.
    HRESULT _HandleCompositionInputWorker(_In_ CCompositionProcessorEngine *pCompositionProcessorEngine, TfEditCookie ec, _In_ ITfContext *pContext);
    HRESULT _CreateAndStartCandidate(_In_ CCompositionProcessorEngine *pCompositionProcessorEngine, TfEditCookie ec, _In_ ITfContext *pContext);
    HRESULT _HandleCandidateWorker(TfEditCookie ec, _In_ ITfContext *pContext);

    void _StartComposition(_In_ ITfContext *pContext);
    void _EndComposition(_In_opt_ ITfContext *pContext);
    BOOL _IsComposing();
    BOOL _IsKeyboardDisabled();

    HRESULT _AddComposingAndChar(TfEditCookie ec, _In_ ITfContext *pContext, _In_ std::wstring &str);
	HRESULT _AddCharAndFinalize(TfEditCookie ec, ITfContext * pContext, std::wstring & str);

    BOOL _FindComposingRange(TfEditCookie ec, _In_ ITfContext *pContext, _In_ ITfRange *pSelection, _Outptr_result_maybenull_ ITfRange **ppRange);
	HRESULT _SetInputString(TfEditCookie ec, ITfContext * pContext, ITfRange * pRange, std::wstring & str, BOOL exist_composing);
    HRESULT _InsertAtSelection(TfEditCookie ec, _In_ ITfContext *pContext, _In_ std::wstring & str, _Outptr_ ITfRange **ppCompRange);

    HRESULT _RemoveDummyCompositionForComposing(TfEditCookie ec, _In_ ITfComposition *pComposition);

    // Invoke key handler edit session
    HRESULT _InvokeKeyHandler(_In_ ITfContext *pContext, UINT code, WCHAR wch, DWORD flags, _KEYSTROKE_STATE keyState);

    // function for the language property
    BOOL _SetCompositionLanguage(TfEditCookie ec, _In_ ITfContext *pContext);

    BOOL _InitThreadMgrEventSink();
    void _UninitThreadMgrEventSink();

    BOOL _InitTextEditSink(_In_ ITfDocumentMgr *pDocMgr);

    void _UpdateLanguageBarOnSetFocus(_In_ ITfDocumentMgr *pDocMgrFocus);

    BOOL _InitKeyEventSink();
    void _UninitKeyEventSink();

    BOOL _InitActiveLanguageProfileNotifySink();
    void _UninitActiveLanguageProfileNotifySink();

    BOOL _IsKeyEaten(_In_ ITfContext *pContext, UINT codeIn, _Out_ UINT *pCodeOut, _Out_writes_(1) WCHAR *pwch, _Out_opt_ _KEYSTROKE_STATE *pKeyState);

    BOOL _IsRangeCovered(TfEditCookie ec, _In_ ITfRange *pRangeTest, _In_ ITfRange *pRangeCover);
    VOID _DeleteCandidateList(BOOL fForce, _In_opt_ ITfContext *pContext);

    WCHAR ConvertVKey(UINT code);

    BOOL _InitThreadFocusSink();
    void _UninitThreadFocusSink();

    BOOL _InitFunctionProviderSink();
    void _UninitFunctionProviderSink();

    BOOL _AddTextProcessorEngine();

    BOOL VerifySinstar3TsfCLSID(_In_ REFCLSID clsid);

    friend LRESULT CALLBACK CSinstar3Tsf_WindowProc(HWND wndHandle, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    ITfThreadMgr* _pThreadMgr;
    TfClientId _tfClientId;
    DWORD _dwActivateFlags;

    // The cookie of ThreadMgrEventSink
    DWORD _threadMgrEventSinkCookie;

    ITfContext* _pTextEditSinkContext;
    DWORD _textEditSinkCookie;

    // The cookie of ActiveLanguageProfileNotifySink
    DWORD _activeLanguageProfileNotifySinkCookie;

    // The cookie of ThreadFocusSink
    DWORD _dwThreadFocusSinkCookie;

    // Composition Processor Engine object.
    CCompositionProcessorEngine* _pCompositionProcessorEngine;

    // Language bar item object.
    CLangBarItemButton* _pLangBarItem;

    // the current composition object.
    ITfComposition* _pComposition;

    // guidatom for the display attibute.
    TfGuidAtom _gaDisplayAttributeInput;
    TfGuidAtom _gaDisplayAttributeConverted;

    CANDIDATE_MODE _candidateMode;
    CCandidateListUIPresenter *_pCandidateListUIPresenter;
    BOOL _isCandidateWithWildcard : 1;

    ITfDocumentMgr* _pDocMgrLastFocused;

    ITfContext* _pContext;

    ITfCompartment* _pSIPIMEOnOffCompartment;
    DWORD _dwSIPIMEOnOffCompartmentSinkCookie;

    HWND _msgWndHandle; 

    // Support the search integration
    ITfFnSearchCandidateProvider* _pITfFnSearchCandidateProvider;

public:
	IUNKNOWN_BEGIN(ITfTextInputProcessorEx)
		IUNKNOWN_ADD_IID( ITfTextInputProcessor)
		IUNKNOWN_ADD_IID( ITfThreadMgrEventSink)
		IUNKNOWN_ADD_IID( ITfTextEditSink)
		IUNKNOWN_ADD_IID( ITfKeyEventSink)
		IUNKNOWN_ADD_IID( ITfCompositionSink)
		IUNKNOWN_ADD_IID( ITfActiveLanguageProfileNotifySink)
		IUNKNOWN_ADD_IID( ITfThreadFocusSink)
		IUNKNOWN_ADD_IID( ITfFunctionProvider)
		IUNKNOWN_ADD_IID( ITfFnGetPreferredTouchKeyboardLayout)
	IUNKNOWN_END()
};