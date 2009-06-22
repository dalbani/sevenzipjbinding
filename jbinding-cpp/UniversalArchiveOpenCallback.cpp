#include "SevenZipJBinding.h"

#include "JNITools.h"
#include "UniversalArchiveOpenCallback.h"

void UniversalArchiveOpencallback::Init(NativeMethodContext * nativeMethodContext, JNIEnv * initEnv,
		jobject archiveOpenCallbackImpl, CPPToJavaInStream * lastVolume)
{
    TRACE_OBJECT_CALL("Init")

    CMyComPtr<IArchiveOpenCallback> archiveOpenCallbackComPtr = new CPPToJavaArchiveOpenCallback(nativeMethodContext, initEnv, archiveOpenCallbackImpl);
    _archiveOpenCallback = archiveOpenCallbackComPtr.Detach();

    _archiveOpenVolumeCallback = NULL;
    _cryptoGetTextPassword = NULL;

    jclass cryptoGetTextPasswordClass = initEnv->FindClass(CRYPTOGETTEXTPASSWORD_CLASS);
    FATALIF(cryptoGetTextPasswordClass == NULL,
            "Can't find class " CRYPTOGETTEXTPASSWORD_CLASS);

    jclass archiveOpenVolumeCallbackClass = initEnv->FindClass(ARCHIVEOPENVOLUMECALLBACK_CLASS);
    FATALIF(cryptoGetTextPasswordClass == NULL,
            "Can't find class " ARCHIVEOPENVOLUMECALLBACK_CLASS);

    if (initEnv->IsInstanceOf(archiveOpenCallbackImpl, cryptoGetTextPasswordClass))
    {
    	TRACE("implements ICryptoGetTextPassword")
        CMyComPtr<ICryptoGetTextPassword> cryptoGetTextPasswordComPtr =
            new CPPToJavaCryptoGetTextPassword(nativeMethodContext, initEnv, archiveOpenCallbackImpl);
        _cryptoGetTextPassword = cryptoGetTextPasswordComPtr.Detach();
    }

    if (initEnv->IsInstanceOf(archiveOpenCallbackImpl, archiveOpenVolumeCallbackClass))
    {
    	TRACE("implements IArchiveOpenVolumeCallback")
        CMyComPtr<IArchiveOpenVolumeCallback> archiveOpenVolumeCallbackComPtr =
            new CPPToJavaArchiveOpenVolumeCallback(nativeMethodContext, initEnv, archiveOpenCallbackImpl, lastVolume);
        _archiveOpenVolumeCallback = archiveOpenVolumeCallbackComPtr.Detach();
    }
}

STDMETHODIMP(UniversalArchiveOpencallback::QueryInterface)(REFGUID iid, void **outObject)
{
    TRACE_OBJECT_CALL("QueryInterface")
//    TRACE1("UniversalArchiveOpencallback::QueryInterface(%i)", iid)
//    TRACE2("UniversalArchiveOpencallback::QueryInterface(%x,%x)", (int)iid.Data4[3], (int)iid.Data4[5])
//    TRACE1("_archiveOpenVolumeCallback=0x%08X", (size_t)_archiveOpenVolumeCallback)

    if (iid == IID_IArchiveOpenCallback)
    {
        *outObject = (void *)(IArchiveOpenCallback *)this;
        AddRef();
        return S_OK;
    }

    if (memcmp(&iid, &IID_IArchiveOpenVolumeCallback, sizeof(GUID)) == 0 && _archiveOpenVolumeCallback)
    {
//    	TRACE("OpenVolume")
        *outObject = (void *)(IArchiveOpenVolumeCallback *)this;
        AddRef();
        return S_OK;
    }

    if (iid == IID_ICryptoGetTextPassword && _cryptoGetTextPassword)
    {
//    	TRACE("CryptoGetTextPassword")
        *outObject = (void *)(ICryptoGetTextPassword *)this;
        AddRef();
        return S_OK;
    }

    TRACE("Nothing found. Returning E_NOINTERFACE")
    return E_NOINTERFACE;
}