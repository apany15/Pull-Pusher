//////////////////////////////////////////////////////////////////////
// IniParser.cpp
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

//#include "stdafx.h"
#include "IniParser.h"


namespace IniParser {
	// ���������� �������
	const char chWhite[] = " \n\t\r";
	
	// ������� � �������� ����� ������
	const char chSectPrefix = '[';
	const char chSectPostfix = ']';
	
	// ������� �����������
	const char chComment = ';';


	/* ���������� ��� ������ rStr1 � rStr2, ���������� true, ���� ��� �����
	 !������ eng! */
	bool CompareNoCase(const std::string &rStr1, const std::string &rStr2)
	{
		return 0 == _strcmpi(rStr1.c_str(), rStr2.c_str());
	}

	/* ������� ���������� ������� (chWhite) � ����� ������, ������, ����� */
	void TrimBoth(std::string &rString)
	{
		TrimLeft(rString);
		TrimRight(rString);
	}

	void TrimRight(std::string &rString)
	{
		rString = rString.substr(0, rString.find_last_not_of(chWhite) + 1);
	}

	void TrimLeft(std::string &rString)
	{
		int nFirst = rString.find_first_not_of(chWhite);
		if (rString.npos == nFirst) 
			return;
		rString = rString.substr(nFirst);
	}

	bool IsValidParameter(const std::string &rString)
	{
		int nEqual = rString.find('=');
		return rString.npos != nEqual && 0 != nEqual;
	}


	//
	// Class CParameter
	// ������������ ��������
	// �������� ��� ������ ���� "key=value"
	//

	// Construction / destruction
	/*
	 pchKey - ��� �����
	 pValue - ��������� �� ������ �������� 
		!��������! ���� ������ ����� ��������� � �����������!
	 bMandatory - ���� ����� true, �� ��� ������������ ��������, ����
		�� �����������, �� ������������ ����������
	*/
	CParameter::CParameter(const char *pchKey, CCommonValue *pValue, bool bMandatory)
		: m_strKey(pchKey), m_pValue(pValue), m_bMandatory(bMandatory)
	{
		//assert(pValue);
		m_bFinded = false;
	}

	CParameter::~CParameter()
	{
		//assert(m_pValue);
		delete m_pValue;
	}

	// �������� ��������� ������, � ������ ������� ����������� ����������
	void CParameter::ParseString(const std::string &rString) throw (CException *)
	{
		//assert(m_pValue);

		if (m_bFinded)
			throw CException::Create(ParameterAlreadyDefined, this);

		try {
			m_pValue->Parse(rString);
		} catch (CException *pExc) {
			pExc->m_pParameter = this;
			throw;
		}

		m_bFinded = true;
	}

	// ���������� ��������� ������������� ���������
	std::string CParameter::ToString() const throw (CException *)
	{
		try {
			//assert(m_pValue);
			return m_strKey + "=" + m_pValue->ToString();
		} catch (CException *pExc) {
			pExc->m_pParameter = this;
			throw;
		}
	}

	// ���������, ������� �� ������������ ���������, ���� ��� - ����������
	void CParameter::CheckMandatory() const throw (CException *)
	{
		if (m_bMandatory && !m_bFinded)
			throw CException::Create(MissingMandatoryParam, this);
	}

	// Operators
	/* �������� ���������: ��������� ��������� �������, ���� �� ����� ����� (��� ����� ��������) */
	bool CParameter::operator==(const CParameter &par) const
	{
		return CompareNoCase(par.GetKey(), m_strKey);
	}

	bool CParameter::operator!=(const CParameter &par) const
	{
		return !CompareNoCase(par.GetKey(), m_strKey);
	}



	//
	// Class CSection
	// ������������ ������ ini-�����
	//

	// Construction / destruction
	// pchSectionName - ��� ������ ��� ����������� � ������������ ��������
	CSection::CSection(const char *pchSectionName)
		: m_strSectionName(pchSectionName)
	{}

	CSection::~CSection()
	{
		RemoveAllParameters();
	}

	// ��������� �������� � ������ 
	void CSection::AddParameter(const char *pchKey, CCommonValue *pValue, bool bMandatory)
	{
		CParameter *pParameter = new CParameter(pchKey, pValue, bMandatory);

	#ifdef _DEBUG
		TParametersConstIt it = m_Parameters.begin();
		while (it != m_Parameters.end()) {
			CParameter *pPar = *it++;
//			assert(pPar);
			if (*pPar == *pParameter) {
				// ������ � ����� ������ ��� ����������
//				assert(0);
				break;
			}
		}
	#endif

		m_Parameters.push_back(pParameter);
	}
	
	// ���������� ��������, � ������ ������ ���������� true
	bool CSection::RemoveParameter(const char *pchKey)
	{
		TParametersIt it = m_Parameters.begin();
		while (it != m_Parameters.end()) {
			CParameter *pParameter = *it;
//			assert(pParameter);
			if (CompareNoCase(pParameter->GetKey(), std::string(pchKey))) {
				delete pParameter;
				m_Parameters.erase(it);
				return true;
			} else {
				it++;
			}
		}
		
		return false;
	}

	// ���������� ��� ��������� �� ������
	void CSection::RemoveAllParameters()
	{
		TParametersConstIt it = GetParametersBegin();
		while (it != GetParametersEnd())
			delete *it++;

		m_Parameters.empty();
	}

	// ���������� �������� �� �����
	CParameter *CSection::GetParameter(const char *pchKey) const
	{
		return GetParameter(std::string(pchKey));
	}

	// ���������� �������� �� �����
	CParameter *CSection::GetParameter(const std::string& rString) const
	{
		TParametersConstIt it = m_Parameters.begin();
		while (it != m_Parameters.end()) {
			CParameter *pParameter = *it++;
//			assert(pParameter);
			if (CompareNoCase(pParameter->GetKey(), rString))
				return pParameter;
		}
		
		return NULL;
	}

	// ���������� ���� m_bFinded �� ���� ����������
	void CSection::ClearFindedFlag()
	{
		TParametersConstIt it = m_Parameters.begin();
		while (it != m_Parameters.end()) {
			CParameter *pParameter = *it++;
//			assert(pParameter);
			pParameter->ClearFindedFlag();
		}
	}

	// ��������� ������� �� ������������ ���������, ���� ��� - ����������
	void CSection::CheckMandatory() const throw (CException *)
	{
		TParametersConstIt it_par = GetParametersBegin();
		while (it_par != GetParametersEnd()) {
			CParameter *pParameter = *it_par++;
//			assert(pParameter);
			pParameter->CheckMandatory();
		}
	}

	// �������� ��������� ������ ���� key=value, � ������ ������� ����������� ����������
	void CSection::Parse(const std::string &rString) throw (CException *)
	{
		int nEqual = rString.find('=');
		if (rString.npos == nEqual || 0 == nEqual)
			throw CException::Create(InvalidString);

		std::string strKey = rString.substr(0, nEqual);
		TrimRight(strKey);

		std::string strValue = rString.substr(nEqual + 1);
		TrimLeft(strValue);

		CParameter *pParameter = GetParameter(strKey);
		if (!pParameter)
			throw CException::Create(UnknownParameter);
		
		pParameter->ParseString(strValue);
	}

	// ��������� ��� ��������� ������ � ����� � ������� key=value
	void CSection::StoreToStream(std::ostream& file) const throw (CException *)
	{
		CParameter *pParameter = NULL;
		TParametersConstIt it = GetParametersBegin();
		while (it != GetParametersEnd()) {
			pParameter = *it++;
//			assert(pParameter);
			file << pParameter->ToString() << std::endl;
		}
	}

	// Operators
	/* �������� ���������: ������ ��������� �������, ���� �� ����� ����� (��� ����� ��������) */
	bool CSection::operator==(const CSection &sect) const
	{
		return CompareNoCase(sect.GetName(), m_strSectionName);
	}

	bool CSection::operator!=(const CSection &sect) const
	{
		return !CompareNoCase(sect.GetName(), m_strSectionName);
	}


	//
	// Class CParser
	// ������������� ���������������� �� ������ � ini-�������
	//

	// Construction/Destruction


	/*
	 bAllowInvalidStrings - ���������� ��������� � ������ ����������� �������� 
		(�� ���������� ������ ������ ��� ����������) ������
		true - ����������, false - ��������� ����������

	 bAllowUnkSections - ���������� ��������� � ������ ����������� ����������� ������:
		true - ����������, false - ��������� ����������

	 bAllowUnkKeys - ���������� ��������� � ������ ����������� ������������ �����:
		true - ����������, false - ��������� ����������
	*/
	CParser::CParser(bool bAllowInvalidStrings, bool bAllowUnkSections, bool bAllowUnkKeys)
	{
		m_bAllowInvalidStrings = bAllowInvalidStrings;
		m_bAllowUnkSections = bAllowUnkSections;
		m_bAllowUnkKeys = bAllowUnkKeys;

		m_pCurrentSection = NULL;
	}

	CParser::~CParser()
	{
		RemoveAllSections();
	}

	// ������ ������ �� ini-����� � ��������� ����������
	void CParser::ParseFile(const char *pchFilePath) throw (CException *)
	{
		// ������� ����
		std::fstream file(pchFilePath, std::fstream::in | std::fstream::binary);
		if(!file)
			throw CException::Create(FileSystemError, errno);

		ParseStream(file);
	}

	// ��������� ������ �� ��������� ���������� � ini-����
	void CParser::StoreToFile(const char *pchFilePath) const throw (CException *)
	{
		std::fstream file(pchFilePath, std::fstream::out | std::fstream::binary | std::fstream::trunc);
		if(!file)
			throw CException::Create(FileSystemError, errno);

		StoreToStream(file);
	}

	// ������ ������ �� ������ � ��������� ����������
	void CParser::ParseStream(std::istream &file) throw (CException *)
	{
		// ������� �� ���� ���������� ������� m_bFinded
		ClearFindedFlag();

		// ������ ������
		std::string str;
		int nLine = 0, nVirtLine = 0;

		while (std::getline(file, str)) {
			nLine++;
			nVirtLine++;

			// ������� ���������� ������� � ����� ������
			TrimBoth(str);

			if (str.length() < 1) {
				nVirtLine--;
				continue;
			}

			if ('[' == str[0]) {
				// ��� ������

				// ������� ��� ������
				str = str.substr(1, str.length() - 2);
				m_pCurrentSection = GetSection(str);
				if (!m_pCurrentSection) {
					if (m_bAllowUnkSections)
						continue;
					else
						throw CException::Create(UnknownSection, nLine);
				}
			} else if (chComment == str[0]) {
				// ��� �����������, ������� ��� � �����������
				m_CommentStrings[nVirtLine] = str;
				continue;
			} else{
				// ��� ��������
				try {
					if (m_pCurrentSection) {
						m_pCurrentSection->Parse(str);
					} else if (IsValidParameter(str)) {
						throw CException::Create(UnknownParameter, nLine);
					} else {
						throw CException::Create(InvalidValue, nLine);
					}
				} catch (CException *pExc) {
					pExc->m_nLineNum = nLine;

					if (pExc->GetCode() == InvalidString && m_bAllowInvalidStrings) {
						continue;
					} else if (pExc->GetCode() == UnknownParameter && m_bAllowUnkKeys) {
						continue;
					} else {
						throw;
					}

					pExc->Delete();
				}
			}
		}

		// �������� ��� �� ������������ ��������� �������
		CheckMandatory();
	}

	// ��������� ������ �� ��������� ���������� � �����
	void CParser::StoreToStream(std::ostream &rStream) const throw (CException *)
	{
		int nLine = 0;
		try {
			// ������ �� ���� �������
			TSectionsConstIt it = GetSectionsBegin();
			while (it != GetSectionsEnd()) {
				CSection *pSection = *it++;
				//assert(pSection);

				nLine++;

				TCommentStrConstIt cit = m_CommentStrings.find(nLine);
				while (cit != m_CommentStrings.end()) {
					rStream << (*cit).second << std::endl;
					nLine++;
					cit = m_CommentStrings.find(nLine);
				}

				rStream << pSection->ToString() << std::endl;

				// ������ �� ���� ���������� � ������
				CSection::TParametersConstIt pit = pSection->GetParametersBegin();
				while (pit != pSection->GetParametersEnd()) {
					nLine++;
					TCommentStrConstIt cit = m_CommentStrings.find(nLine);
					while (cit != m_CommentStrings.end()) {
						rStream << (*cit).second << std::endl;
						nLine++;
						cit = m_CommentStrings.find(nLine);
					}

					CParameter *pParameter = *pit++;
//					assert(pParameter);

					if (pParameter->IsFinded())
						rStream << pParameter->ToString() << std::endl;
					else
						nLine--;
				}

				rStream << std::endl;
			}
			
			// ������ ���������� ������������
			TCommentStrConstIt cit = m_CommentStrings.upper_bound(nLine);
			while (cit != m_CommentStrings.end())
				rStream << (*cit++).second << std::endl;
		} catch (CException *pExc) {
			pExc->m_nLineNum = nLine;
			throw;
		}
	}

	// ���������� ��������� ��� ������ � (����� CParser) ini-������
	std::ostream& operator<<(std::ostream& stream, const CParser &pars)
	{
		pars.StoreToStream(stream);
		return stream;
	}

	std::istream& operator>>(std::istream& stream, CParser &pars)
	{
		pars.ParseStream(stream);
		return stream;
	}

	// ��������� ������
	CSection *CParser::AddSection(const char *pchSectionName)
	{
		CSection *pSection = new CSection(pchSectionName);

	#ifdef _DEBUG
		TSectionsConstIt it = m_Sections.begin();
		while (it != m_Sections.end()) {
			CSection *pSect = *it++;
//			assert(pSect);
			if (*pSect == *pSection) {
				// ������ � ����� ������ ��� ����������
//				assert(0);
				break;
			}
		}
	#endif

		m_Sections.push_back(pSection);
		return pSection;
	}

	// ������� ������ �� ����� (��� ����������� � ������������ ��������), � ������ ������ ���������� true
	bool CParser::RemoveSection(const char *pchSectionName)
	{
		TSectionsIt it = m_Sections.begin();
		while (it != m_Sections.end()) {
			CSection *pSection = *it;
//			assert(pSection);
			if (CompareNoCase(pSection->GetName(), std::string(pchSectionName))) {
				delete pSection;
				m_Sections.erase(it);
				return true;
			} else {
				it++;
			}
		}

		return false;
	}

	// ������� ��� ������
	void CParser::RemoveAllSections()
	{
		TSectionsConstIt it = GetSectionsBegin();
		while (it != GetSectionsEnd())
			delete *it++;

		m_Sections.empty();
	}
	
	// ���������� ������ �� � �����
	CSection *CParser::GetSection(const char *pchSectionName) const
	{
		return GetSection(std::string(pchSectionName));
	}

	// ���������� ������ �� �����
	CSection *CParser::GetSection(const std::string &rString) const
	{
		TSectionsConstIt it = m_Sections.begin();
		while (it != m_Sections.end()) {
			CSection *pSection = *it++;
//			assert(pSection);
			if (CompareNoCase(pSection->GetName(), rString))
				return pSection;
		}

		return NULL;
	}

	// ���������� ���� m_bFinded �� ���� ����������
	void CParser::ClearFindedFlag()
	{
		TSectionsConstIt it = GetSectionsBegin();
		while (it != GetSectionsEnd()) {
			CSection *pSection = *it++;
//			assert(pSection);
			pSection->ClearFindedFlag();
		}
	}

	// ��������� ������� �� ������������ ���������, ���� ��� - ����������
	void CParser::CheckMandatory() const throw (CException *)
	{
		TSectionsConstIt it = GetSectionsBegin();
		while (it != GetSectionsEnd()) {
			CSection *pSection = *it++;
//			assert(pSection);
			pSection->CheckMandatory();
		}
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	// ������ ��������
	//
	//


	//
	// Class CStdStringValue
	// ������������ ��������� �������� (std::string)
	//

	/* 
	 pstrData - �����, �� �������� ����� ������������/�������� �������� 
	 bAllowEmpty - true - ��������� ������ ������
	*/
	CStdStringValue::CStdStringValue(std::string *pstrData, int nMaxLen, bool bAllowEmpty)
		: m_pstrData(pstrData), m_nMaxLen(nMaxLen), m_bAllowEmpty(bAllowEmpty)
	{
//		assert(m_pstrData);
	}

	// interface CCommonValue

	/* �������� ������������ ���������� ������ pchString � 
	 �������� ������������� ����, � ������ ������� ����������� ���������� */
	void CStdStringValue::Parse(const std::string &rString) throw (CException *)
	{
//		assert(m_pstrData);
		*m_pstrData = rString;
		if (!IsSatisfy())
			throw CException::Create(ValueOutOfRanges);
	}

	/* ���������� ��������� ������������� �������� */
	std::string CStdStringValue::ToString() const
	{
//		assert(m_pstrData);
		if (!IsSatisfy())
			throw CException::Create(ValueOutOfRanges);
		return *m_pstrData;
	}

	/* ��������� ��������� ������� � ���������� true, ���� �
	 ��������� ���������� ��������� ���������� (��������������� 
	 ������������) �������� */
	bool CStdStringValue::IsSatisfy() const
	{
//		assert(m_pstrData);

		if ((m_pstrData->length() > m_nMaxLen) || 
			(0 == m_pstrData->length() && !m_bAllowEmpty))
			return false;

		return true;
	}


	//
	// Class CStringValue
	// ������������ ��������� �������� (����������� C-������, � ��������� ������)
	//

	/* 
	 pchData - �����, �� �������� ����� ������������/�������� �������� 
	 nMaxLen - ������������ ����� ������
	 bAllowEmpty - true - ��������� ������ ������
	*/
	CStringValue::CStringValue(char *pchData, int nMaxLen, bool bAllowEmpty)
		: m_pchData(pchData), m_nMaxLen(nMaxLen), m_bAllowEmpty(bAllowEmpty)
	{
//		assert(m_pchData);
	}

	// interface CCommonValue
	/* �������� ������������ ���������� ������ rString � 
	 �������� ������������� ����, � ������ ������� ����������� ���������� */
	void CStringValue::Parse(const std::string &rString) throw (CException *)
	{
//		assert(m_pchData);
		if (rString.length() < m_nMaxLen - 1)
			strcpy(m_pchData, rString.c_str());
		else
			throw CException::Create(ValueOutOfRanges);

		if ((0 == strlen(m_pchData) && !m_bAllowEmpty) || !IsSatisfy())
			throw CException::Create(ValueOutOfRanges);
	}

	/* ���������� ��������� ������������� �������� */
	std::string CStringValue::ToString() const
	{
//		assert(m_pchData);
		if (!IsSatisfy())
			throw CException::Create(ValueOutOfRanges);
		return std::string(m_pchData);
	}

	/* ��������� ��������� ������� � ���������� true, ���� �
	 ��������� ���������� ��������� ���������� (��������������� 
	 ������������) �������� */
	bool CStringValue::IsSatisfy() const
	{
//		assert(m_pchData);

		if ((strlen(m_pchData) > m_nMaxLen) || 
			(0 == strlen(m_pchData) && !m_bAllowEmpty))
			return false;

		return true;
	}



//	//
//	// Class CRGBValue
//	// ������������� ���� � ��������� RGB
//	//
//
//	// Construction
//	CRGBValue::CRGBValue(COLORREF *pColor)
//		: m_pColor(pColor)
//	{
////		assert(m_pColor);
//	}
//
//	// interface CCommonValue
//	/* �������� ������������ ���������� ������ rString � 
//	 �������� ������������� ����, � ������ ������� ����������� ���������� */
//	void CRGBValue::Parse(const std::string &rString) throw (CException *)
//	{
////		assert(m_pColor);
//
//		int r, g, b;
//		char ch;
//		std::stringstream sstream(rString);
//		sstream >> std::ws >> r >> ch;
//		if (',' == ch) {
//			sstream >> std::ws >> g >> ch;
//			if (',' == ch) {
//				sstream >> std::ws >> b;
//			} else
//				throw CException::Create(InvalidValue);
//		} else
//			throw CException::Create(InvalidValue);
//
//		if (!(sstream >> std::ws).eof())
//			throw CException::Create(InvalidValue);
//
//		if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255))
//			throw CException::Create(InvalidValue);
//
//		*m_pColor = ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)));
//	}
//
//
//	/* ���������� ��������� ������������� �������� */
//	std::string CRGBValue::ToString() const throw (CException *)
//	{
////		assert(m_pColor);
//
//		int r = ((BYTE)*m_pColor);
//		int g = ((BYTE)(((WORD)(*m_pColor)) >> 8));
//		int b = ((BYTE)((*m_pColor) >> 16));
//		std::stringstream sstream;
//		sstream << r << ", " << g << ", " << b;
//		return sstream.str();
//	}
//
//	/* ��������� ��������� ������� � ���������� true, ���� �
//	 ��������� ���������� ��������� ���������� (��������������� 
//	 ������������) �������� */
//	bool CRGBValue::IsSatisfy() const
//	{
////		assert(m_pColor);
//		return true;
//	}



	//
	// Class CBoolValue
	// ������������ �������� ���������� ����
	// ����� ��������� ��������: (true, false), (yes, no), (1, 0)
	//

	// Construction
	CBoolValue::CBoolValue(bool *pbData, TBoolStyle style)
		: m_pbData(pbData), m_Style(style)
	{
//		assert(m_pbData);
	}

	// interface CCommonValue
	/* �������� ������������ ���������� ������ rString � 
	 �������� ������������� ����, � ������ ������� ����������� ���������� */
	void CBoolValue::Parse(const std::string &rString) throw (CException *)
	{
//		assert(m_pbData);

		switch (m_Style) {
		case TrueFalse:
			if (CompareNoCase("true", rString)) {
				*m_pbData = true;
				break;
			} else if (CompareNoCase("false", rString)) {
				*m_pbData = false;
				break;
			} else {
				throw CException::Create(InvalidValue);
			}

		case YesNo:
			if (CompareNoCase("yes", rString)) {
				*m_pbData = true;
				break;
			} else if (CompareNoCase("no", rString)) {
				*m_pbData = false;
				break;
			} else {
				throw CException::Create(InvalidValue);
			}
		}
	}

	/* ���������� ��������� ������������� �������� */
	std::string CBoolValue::ToString() const throw (CException *)
	{
//		assert(m_pbData);

		switch (m_Style) {
		case TrueFalse:
			return *m_pbData ? std::string("true") : std::string("false");

		case YesNo:
			return *m_pbData ? std::string("yes") : std::string("no");

		default:
//			assert(0);
			return std::string();
		};
	}

	/* ��������� ��������� ������� � ���������� true, ���� �
	 ��������� ���������� ��������� ���������� (��������������� 
	 ������������) �������� */
	bool CBoolValue::IsSatisfy() const
	{
//		assert(m_pbData);
		return true;
	}

}
