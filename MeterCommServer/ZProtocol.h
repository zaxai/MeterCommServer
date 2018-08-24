#pragma once
class ZProtocol
{
public:
	ZProtocol(void);
	virtual ~ZProtocol(void);
	virtual int DataEncode(CString & strData,CString & strError)=0;
	virtual bool DataDecode(CString strData,CString & strValue)=0;
	virtual CString GetLength(void)=0;
};

