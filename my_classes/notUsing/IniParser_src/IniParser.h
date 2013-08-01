//////////////////////////////////////////////////////////////////////
// IniParser.h
//
//
//	���������:	IniParser
//	����������: ������ � ini-�������
//
//	������:		1.0
//	����:		20.09.2003
//	�����:		����� ���� �������
//	e-mail:		Yuri32@nm.ru
//
//
//

#pragma once
//#pragma warning (disable : 4786)
//#pragma warning (disable : 4290)
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <map>
//#include "windows.h"

namespace IniParser {

	// ������� � �������� ����� ������
	extern const char chSectPrefix;
	extern const char chSectPostfix;

	/* ���������� ��� ������ rStr1 � rStr2, ���������� true, ���� ��� �����
	 !������ eng! */
	bool CompareNoCase(const std::string &rStr1, const std::string &rStr2);

	/* ������� ���������� ������� (chWhite) � ����� ������, ������, ����� */
	void TrimBoth(std::string &rString);
	void TrimRight(std::string &rString);
	void TrimLeft(std::string &rString);


	// ��������������� ����������
	class CParameter;
	class CSection;
	class CParser;

	// ���� ������
	enum TErrorCodes {
		FileSystemError,			/* ������ ��������  �������. 
									  CException::m_nLineNum ����� ��������� ��������� ��� ������ */
		UnknownSection,				/* ����������� ������ */
		UnknownParameter,			/* ����������� �������� */
		InvalidString,				/* �������� ������ */
		InvalidValue,				/* �������� �������� (������ ��������������) */
		ValueOutOfRanges,			/* �������� ������� �� �������� ����� */
		ParameterAlreadyDefined,	/* �������� ��������� ��������� ��� */
		MissingMandatoryParam		/* ���������� ������������ �������� */
	};


	//
	// Class CException
	// ������������ ����������, ������� ���������� ��� ������ � ini-������
	//

	class CException : public std::exception {
	// Construction / destruction
	protected:
		CException(TErrorCodes code)
			: m_Cause(code), m_pParameter(NULL)
		{}

		CException(TErrorCodes code, const CParameter *pParameter)
			: m_Cause(code), m_pParameter(pParameter), m_nLineNum(-1)
		{}

		CException(TErrorCodes code, int nLine)
			: m_Cause(code), m_pParameter(NULL), m_nLineNum(nLine)
		{}

	// Creation
	public:
		static CException *Create(TErrorCodes code) {return new CException(code);}
		static CException *Create(TErrorCodes code, const CParameter *pParameter) {return new CException(code, pParameter);}
		static CException *Create(TErrorCodes code, int nLine) {return new CException(code, nLine);}
		void Delete() {delete this;}

	// ����������� ����������� � �������� ������������
	private:
		CException(const CException&) {}		// no implementation
		CException& operator=(const CException&) {}	// no implementation

	public:
		int GetLineNum() const {return m_nLineNum;}
		TErrorCodes GetCode() const {return m_Cause;}
		const CParameter *GetParameter() const {return m_pParameter;}

	protected:
		/* ����� ������, ��� ������� ������� ��������� ����������,
		 ���� � �������� ������������ TErrorCodes ��� ������ ������ ��
		 ������� ����� ������ ���� ���������� */
		int m_nLineNum;
		// ��� ������
		TErrorCodes m_Cause;
		// ��������� �� �������� ��� ������� �������� ��������� ����������
		const CParameter *m_pParameter;

	friend CParser;
	friend CSection;
	friend CParameter;
	};


	//
	// Class CCommonValue
	// ������������ ������������ ��� ������. ��� ��������� ���� ������ ����������� �� ����
	//

	class CCommonValue {
	public:
		virtual ~CCommonValue() {}

	public:
		/* �������� ������������ ���������� ������ rString � 
		 �������� ������������� ����, � ������ ������� ����������� ���������� */
		virtual void Parse(const std::string &rString) throw (CException *) = 0;
		// ���������� ��������� ������������� ��������
		virtual std::string ToString() const throw (CException *) = 0;
		/* ��������� ��������� ������� � ���������� true, ���� �
		 ��������� ���������� ��������� ���������� (��������������� 
		 ������������) �������� */
		virtual bool IsSatisfy() const = 0;
	};


	//
	// Class CParameter
	// ������������ ��������
	// �������� ��� ������ ���� "key=value"
	//

	class CParameter {
	// Construction / destruction
	protected:
		/*
		 pchKey - ��� �����
		 pValue - ��������� �� ������ ��������
			!��������! ���� ������ ����� ��������� � �����������!
		 bMandatory - ���� ����� true, �� ��� ������������ �������� (����
			�� �����������, �� ������������ ����������)
		*/
		CParameter(const char *pchKey, CCommonValue *pValue, bool bMandatory = false);
		virtual ~CParameter();

	// ����������� ����������� � �������� ������������
	private:
		CParameter(const CParameter&) {}			// no implementation
		CParameter& operator=(const CParameter&) {}	// no implementation

	public:
		// ���������� ����
		const std::string &GetKey() const {return m_strKey;}
		// ���������� const ��������� �� ������ ��������
		const CCommonValue *GetValue() const {return m_pValue;}
		// ���������� ��������� ������������� ���������
		std::string ToString() const throw (CException *);

	protected:
		// �������� ��������� ������, � ������ ������� ����������� ����������
		void ParseString(const std::string &rString) throw (CException *);
		
		/* ���� ��� ������������ ��������, �� ��������� ������ �� ��, 
		 ���� ���, �� ������������ ���������� */
		void CheckMandatory() const throw (CException *);
		// ���������� ���� m_bFinded
		void ClearFindedFlag() {m_bFinded = false;}
		bool IsFinded() const {return m_bFinded;}

	// Operators
	protected:
		/* �������� ���������: ��������� ��������� �������, ���� �� ����� ����� (��� ����� ��������) */
		bool operator==(const CParameter &par) const;
		bool operator!=(const CParameter &par) const;

	protected:
		// ���� (key)
		std::string m_strKey;
		// ��������� �� ������ ��������
		CCommonValue *m_pValue;
		// ==true, �� ��� ������������ ��������
		bool m_bMandatory;
		
		// ����� Parse ���������� ��� � true, ���� �������� ��������� � ������� ���������
		bool m_bFinded;

	friend CSection;
	friend CParser;
	};



	//
	// Class CSection
	// ������������ ������ ini-�����
	//

	class CSection {
	// Construction / destruction
	protected:
		// pchSectionName - ��� ������ ��� ����������� � ������������ ��������
		CSection(const char *pchSectionName);
		virtual ~CSection();

	// ����������� ����������� � �������� ������������
	private:
		CSection(const CSection&) {}			// no implementation
		CSection& operator=(const CSection&) {}	// no implementation

	// ������ � �������
	public:
		// ��������� �������� � ������
		void AddParameter(const char *pchKey, CCommonValue *pValue, bool bMandatory = false);
		// ���������� ��������, � ������ ������ ���������� true
		bool RemoveParameter(const char *pchKey);
		// ���������� ��� ��������� �� ������
		void RemoveAllParameters();
		
		// ���������� �������� �� �����
		CParameter *GetParameter(const char *pchKey) const;
		// ���������� �������� �� �����
		CParameter *GetParameter(const std::string& rString) const;

		// ���������� ��� ������ (��� �������� � ���������)
		const std::string &GetName() const {return m_strSectionName;}
		// ���������� ��� ������ (� ���������� � ����������� ���������)
		std::string ToString() const {return std::string(chSectPrefix + m_strSectionName + chSectPostfix);}

	protected:
		// ���������� ���� m_bFinded �� ���� ����������
		void ClearFindedFlag();
		// ��������� ������� �� ������������ ���������, ���� ��� - ����������
		void CheckMandatory() const throw (CException *);

		// �������� ��������� ������ ���� key=value, � ������ ������� ����������� ����������
		void Parse(const std::string &rString) throw (CException *);
		// ��������� ��� ��������� ������ � ����� � ������� key=value
		void StoreToStream(std::ostream& file) const throw (CException *);

		// Operators
	protected:
		/* �������� ���������: ������ ��������� �������, ���� �� ����� ����� (��� ����� ��������) */
		bool operator==(const CSection &sect) const;
		bool operator!=(const CSection &sect) const;

	// ����������� ����������� ����� ������
	public:
		typedef std::list<CParameter *> TParameters;
		typedef TParameters::iterator TParametersIt;
		typedef TParameters::const_iterator TParametersConstIt;

	// ��������� �� ���������� ������
	public:
		TParametersConstIt GetParametersBegin() const {return m_Parameters.begin();}
		TParametersConstIt GetParametersEnd() const {return m_Parameters.end();}

	protected:
		// ��� ������
		std::string m_strSectionName;
		// ������ ����������
		TParameters m_Parameters;

	friend CParser;
	};



	//
	// Class CParser
	// ������������� �������� ���������������� �� ������ � ini-�������
	//

	class CParser {
	// Construction / destruction
	public:
		/*
		 bAllowInvalidStrings - ���������� ��������� � ������ ����������� �������� 
			(�� ���������� ������ ������ ��� ����������) ������
			true - ����������, false - ��������� ����������

		 bAllowUnkSections - ���������� ��������� � ������ ����������� ����������� ������:
			true - ����������, false - ��������� ����������

		 bAllowUnkKeys - ���������� ��������� � ������ ����������� ������������ �����:
			true - ����������, false - ��������� ����������
		*/
		CParser(bool bAllowInvalidStrings = false, bool bAllowUnkSections = true, bool bAllowUnkKeys = true);
		virtual ~CParser();

	public:
		// ������ ������ �� ini-����� � ��������� ����������
		void ParseFile(const char *pchFilePath) throw (CException *);
		// ��������� ������ �� ��������� ���������� � ini-����
		void StoreToFile(const char *pchFilePath) const throw (CException *);

		// ������ ������ �� ������ � ��������� ����������
		void ParseStream(std::istream &rStream) throw (CException *);
		// ��������� ������ �� ��������� ���������� � �����
		void StoreToStream(std::ostream &rStream) const throw (CException *);

		// ���������� ��������� ��� ������ � ini-������
		friend std::ostream& operator<<(std::ostream& stream, const CParser &pars);
		friend std::istream& operator>>(std::istream& stream, CParser &pars);

	public:
		// ��������� ������. ���������� ��������� �� ����������� ������ 
		CSection *AddSection(const char *pchSectionName);
		// ������� ������ �� ����� (��� ����������� � ������������ ��������), � ������ ������ ���������� true
		bool RemoveSection(const char *pchSectionName);
		// ������� ��� ������
		void RemoveAllSections();
		// ���������� ������ �� � �����
		CSection *GetSection(const char *pchSectionName) const;
		// ���������� ������ �� � �����
		CSection *GetSection(const std::string &rString) const;

	protected:
		// ���������� ���� m_bFinded �� ���� ����������
		void ClearFindedFlag();
		// ��������� ������� �� ������������ ���������, ���� ��� - ����������
		void CheckMandatory() const throw (CException *);

	// ����������� ����������� ����� ������
	public:
		typedef std::list<CSection *> TSections;
		typedef TSections::iterator TSectionsIt;
		typedef TSections::const_iterator TSectionsConstIt;
		
		typedef std::map<int, std::string> TCommentStr;
		typedef TCommentStr::const_iterator TCommentStrConstIt;
		typedef TCommentStr::value_type TCommentStrPair;

	// ��������� �� �������
	public:
		TSectionsConstIt GetSectionsBegin() const {return m_Sections.begin();}
		TSectionsConstIt GetSectionsEnd() const {return m_Sections.end();}

	protected:
		// ������ ������
		TSections m_Sections;
		// ��������� �������
		bool m_bAllowInvalidStrings, m_bAllowUnkSections, m_bAllowUnkKeys;

		// ������ � ������� ���� ������
		CSection *m_pCurrentSection;

		// ������ �����������
		TCommentStr m_CommentStrings;
	};




	////////////////////////////////////////////////////////////////////////////////
	//
	//   ������ ��������
	//
	//


	//
	// ������ CRangeLimits
	// ������������ ����������� �� ��������
	//

	template <typename T>
	class CRangeLimits {
	// ���������� ����������� ����� ������
	public:
		/* ����������� �� �������� ���������� */
		enum ELimits {
			Min,	// ����������� ����� (min)
			Max,	// ����������� ������ (max)
			MinMax	// ����������� ����� � ������
		};

	// Construction
	public:
		CRangeLimits(ELimits lim, T min, T max)
			: m_min(min), m_max(max), m_limits(lim)
		{}
	
	public:
		/* ���������� true, ���� �������� �� ������ 
		 pData ������������� ������������ */
		bool IsSatisfy(const T *pData) const
		{
			if (!pData)
				return false;

			switch (m_limits) {
			case Min:
				return *pData >= m_min;

			case Max:
				return *pData <= m_max;

			case MinMax:
				return m_min <= *pData && *pData <= m_max;

			default:
//				assert(0);
				return false;
			}
		}

	protected:
		T m_min;
		T m_max;
		ELimits m_limits;
	};



	//
	// ������ CNumericTypes
	// ������������ �������� �������� ����� (int, float, double...)
	//

	template <typename T>
	class CNumericType : public CCommonValue {
	// ���������� ����������� ����� ������
	public:
		enum EFlags {
			// integer
			dec = 0x1,
			hex = 0x2,
			oct = 0x4,

			// floating
			scientific = 0x8,
			fixed = 0x10
		};

	
	// Construction / destruction
	public:
		/* pnData - �����, �� �������� ����� ������������/�������� ��������,
		   pLimits - ��������� �� ������ ������-������������ ��������
				(� ���-�� ��������� ������� CRangeLimits �.�. ������ ��� �� ���, 
				 ��� � � ������ CNumericType).
				���� ����������� �� �������� ���, ���� �������� �.�. ����� NULL
				!��������! ������ ����� ��������� � ������������ 
		   flags - ����� ��������������
		   nPrecision - ��������
		*/
		CNumericType(T *pData, CRangeLimits<T> *pLimits, EFlags flags = dec, int nPrecision = 6)
			: m_pData(pData), m_pLimits(pLimits), m_StreamFlags(flags), m_nPrecision(nPrecision)
		{}

		~CNumericType()
		{
			delete m_pLimits;
		}

	// interface ICommonValue
	public:
		/* �������� ������������ ���������� ������ pchString � 
		 �������� ������������� ����, � ������ ������� ����������� ���������� */
		virtual void Parse(const std::string &rString) throw (CException *);
		/* ���������� ��������� ������������� �������� */
		virtual std::string ToString() const throw (CException *);
		/* ��������� ��������� ������� � ���������� true, ���� �
		 ��������� ���������� ��������� ���������� (��������������� 
		 ������������) �������� */
		virtual bool IsSatisfy() const;

	protected:
		// �������������� ����� � ������ (������������� �����)
		void PrepareStream(std::ostream& stream) const;

	protected:
		T *m_pData;
		CRangeLimits<T> *m_pLimits;
		// ����� ���������������
		EFlags m_StreamFlags;
		// ��������
		int m_nPrecision;
	};

	/* �������� ������������ ���������� ������ pchString � 
	 �������� ������������� ����, � ������ ������� ����������� ���������� */
	template <typename T>
	void CNumericType<T>::Parse(const std::string &rString) throw (CException *)
	{
//		assert(m_pData);
		std::stringstream sstream(rString);
		PrepareStream(sstream);

		if (!(sstream >> *m_pData) || !(sstream >> std::ws).eof())
			throw CException::Create(InvalidValue);

		if (!IsSatisfy())
			throw CException::Create(ValueOutOfRanges);
	}

	/* ���������� ��������� ������������� �������� */
	template <typename T>
	std::string CNumericType<T>::ToString() const throw (CException *)
	{
//		assert(m_pData);
		if (!IsSatisfy())
			throw CException::Create(ValueOutOfRanges);

		std::stringstream sstream;
		PrepareStream(sstream);
		sstream << *m_pData;
		return sstream.str();
	}

	/* ��������� ��������� ������� � ���������� true, ���� �
	 ��������� ���������� ��������� ���������� (��������������� 
	 ������������) �������� */
	template <typename T>
	bool CNumericType<T>::IsSatisfy() const
	{
//		assert(m_pData);
		if (!m_pLimits)
			return true;
		
		return m_pLimits->IsSatisfy(m_pData);
	}

	// �������������� ����� � ������ (������������� �����)
	template <typename T>
	void CNumericType<T>::PrepareStream(std::ostream& rStream) const
	{
		rStream.setf(std::ios::showbase);
		rStream.precision(m_nPrecision);

		if (dec == (dec & m_StreamFlags)) {
			rStream.setf(std::ios::dec, std::ios_base::basefield);
		} else if (hex == (hex & m_StreamFlags)) {
			rStream.setf(std::ios::hex, std::ios_base::basefield);
		} else if (oct == (oct & m_StreamFlags)) {
			rStream.setf(std::ios::oct, std::ios_base::basefield);
		} else if (scientific == (scientific & m_StreamFlags)) {
			rStream.setf(std::ios::scientific, std::ios_base::floatfield);
		} else if (fixed == (fixed & m_StreamFlags)) {
			rStream.setf(std::ios::fixed, std::ios_base::floatfield);
		} else {
//			assert(0);
		}
	}


	//
	// Class CStdStringValue
	// ������������ ��������� �������� (std::string)
	//

	class CStdStringValue : public CCommonValue {
	// Construction
	public:
		/* 
		 pstrData - �����, �� �������� ����� ������������/�������� �������� 
		 nMaxLen - ������������ �����, ���� == -1, �� ����� �� ����������
		 bAllowEmpty - true - ��������� ������ ������
		*/
		CStdStringValue(std::string *pstrData, int nMaxLen, bool bAllowEmpty);

	// interface CCommonValue
	public:
		/* �������� ������������ ���������� ������ rString � 
		 �������� ������������� ����, � ������ ������� ����������� ���������� */
		virtual void Parse(const std::string &rString) throw (CException *);
		/* ���������� ��������� ������������� �������� */
		virtual std::string ToString() const throw (CException *);
		/* ��������� ��������� ������� � ���������� true, ���� �
		 ��������� ���������� ��������� ���������� (��������������� 
		 ������������) �������� */
		virtual bool IsSatisfy() const;

	protected:
		std::string *m_pstrData;
		int m_nMaxLen;
		bool m_bAllowEmpty;
	};


	//
	// Class CStringValue
	// ������������ ��������� �������� (����������� C-������ � ��������� ������)
	//

	class CStringValue : public CCommonValue {
	// Construction
	public:
		/* 
		 pchData - �����, �� �������� ����� ������������/�������� �������� 
		 nMaxLen - ������������ ����� ������
		 bAllowEmpty - true - ��������� ������ ������
		*/
		CStringValue(char *pchData, int nMaxLen, bool bAllowEmpty);

	// interface CCommonValue
	public:
		/* �������� ������������ ���������� ������ rString � 
		 �������� ������������� ����, � ������ ������� ����������� ���������� */
		virtual void Parse(const std::string &rString) throw (CException *);
		/* ���������� ��������� ������������� �������� */
		virtual std::string ToString() const throw (CException *);
		/* ��������� ��������� ������� � ���������� true, ���� �
		 ��������� ���������� ��������� ���������� (��������������� 
		 ������������) �������� */
		virtual bool IsSatisfy() const;

	protected:
		char *m_pchData;
		int m_nMaxLen;
		bool m_bAllowEmpty;
	};



	////
	//// Class CRGBValue
	//// ������������ ���� � ��������� RGB
	////

	///*
	// ����������
	// <part> ::= <digit>[<digit>][<digit>]
	// <sep> ::= <,><space>*
	// <space> ::= < >
	// <color> ::= <part><sep><part><sep><part>
	//*/

	//class CRGBValue : public CCommonValue {
	//// Construction
	//public:
	//	CRGBValue(COLORREF *pColor);

	//// interface CCommonValue
	//public:
	//	/* �������� ������������ ���������� ������ rString � 
	//	 �������� ������������� ����, � ������ ������� ����������� ���������� */
	//	virtual void Parse(const std::string &rString) throw (CException *);
	//	/* ���������� ��������� ������������� �������� */
	//	virtual std::string ToString() const throw (CException *);
	//	/* ��������� ��������� ������� � ���������� true, ���� �
	//	 ��������� ���������� ��������� ���������� (��������������� 
	//	 ������������) �������� */
	//	virtual bool IsSatisfy() const;

	//protected:
	//	COLORREF *m_pColor;
	//};



	//
	// Class CBoolValue
	// ������������ �������� ���������� ����
	// ����� ��������� ��������: (true, false); (yes, no)
	//

	class CBoolValue : public CCommonValue {
	public:
		enum TBoolStyle {TrueFalse, YesNo};

	// Construction
	public:
		CBoolValue(bool *pbData, TBoolStyle style = TrueFalse);

	// interface CCommonValue
	public:
		/* �������� ������������ ���������� ������ rString � 
		 �������� ������������� ����, � ������ ������� ����������� ���������� */
		virtual void Parse(const std::string &rString) throw (CException *);
		/* ���������� ��������� ������������� �������� */
		virtual std::string ToString() const throw (CException *);
		/* ��������� ��������� ������� � ���������� true, ���� �
		 ��������� ���������� ��������� ���������� (��������������� 
		 ������������) �������� */
		virtual bool IsSatisfy() const;

	protected:
		bool *m_pbData;
		TBoolStyle m_Style;
	};

};
