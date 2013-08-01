//////////////////////////////////////////////////////////////////////
// IniParser.cpp
//
//
//	компонент:	IniParser
//	назначение: работа с ini-файлами
//
//	версия:		1.0
//	дата:		20.09.2003
//	автор:		Попов Юрий Юрьевич
//	e-mail:		Yuri32@nm.ru
//
//
//

//#include "stdafx.h"
#include "IniParser.h"


namespace IniParser {
	// Пробельные символы
	const char chWhite[] = " \n\t\r";
	
	// Префикс и постфикс имени секции
	const char chSectPrefix = '[';
	const char chSectPostfix = ']';
	
	// Признак комментария
	const char chComment = ';';


	/* Сравнивает две строки rStr1 и rStr2, возвращает true, если они равны
	 !только eng! */
	bool CompareNoCase(const std::string &rStr1, const std::string &rStr2)
	{
		return 0 == _strcmpi(rStr1.c_str(), rStr2.c_str());
	}

	/* Усекает пробельные символы (chWhite) с обеих сторон, справа, слева */
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
	// Представляет параметр
	// Параметр это строка вида "key=value"
	//

	// Construction / destruction
	/*
	 pchKey - имя ключа
	 pValue - указатель на объект значения 
		!ВНИМАНИЕ! этот объект будет уничтожен в деструкторе!
	 bMandatory - если равен true, то это обязательный параметр, если
		он отсутствует, то возбуждается исключение
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

	// Пытается разобрать строку, в случае неудачи выбрасывает исключение
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

	// Возвращает строковое представление параметра
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

	// Проверяет, считаны ли обязательные параметры, если нет - исключение
	void CParameter::CheckMandatory() const throw (CException *)
	{
		if (m_bMandatory && !m_bFinded)
			throw CException::Create(MissingMandatoryParam, this);
	}

	// Operators
	/* Критерий равенства: параметры считаются равными, если их ключи равны (без учета регистра) */
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
	// Представляет секцию ini-файла
	//

	// Construction / destruction
	// pchSectionName - имя секции без префиксного и постфиксного символов
	CSection::CSection(const char *pchSectionName)
		: m_strSectionName(pchSectionName)
	{}

	CSection::~CSection()
	{
		RemoveAllParameters();
	}

	// Добавляет параметр в секцию 
	void CSection::AddParameter(const char *pchKey, CCommonValue *pValue, bool bMandatory)
	{
		CParameter *pParameter = new CParameter(pchKey, pValue, bMandatory);

	#ifdef _DEBUG
		TParametersConstIt it = m_Parameters.begin();
		while (it != m_Parameters.end()) {
			CParameter *pPar = *it++;
//			assert(pPar);
			if (*pPar == *pParameter) {
				// Секция с таким именем уже существует
//				assert(0);
				break;
			}
		}
	#endif

		m_Parameters.push_back(pParameter);
	}
	
	// Уничтожает параметр, в случае успеха возвращает true
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

	// Уничтожает все параметры из секции
	void CSection::RemoveAllParameters()
	{
		TParametersConstIt it = GetParametersBegin();
		while (it != GetParametersEnd())
			delete *it++;

		m_Parameters.empty();
	}

	// Возвращает параметр по ключу
	CParameter *CSection::GetParameter(const char *pchKey) const
	{
		return GetParameter(std::string(pchKey));
	}

	// Возвращает параметр по ключу
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

	// Сбрасывает флаг m_bFinded во всех параметрах
	void CSection::ClearFindedFlag()
	{
		TParametersConstIt it = m_Parameters.begin();
		while (it != m_Parameters.end()) {
			CParameter *pParameter = *it++;
//			assert(pParameter);
			pParameter->ClearFindedFlag();
		}
	}

	// Проверяет считаны ли обязательные параметры, если нет - исключение
	void CSection::CheckMandatory() const throw (CException *)
	{
		TParametersConstIt it_par = GetParametersBegin();
		while (it_par != GetParametersEnd()) {
			CParameter *pParameter = *it_par++;
//			assert(pParameter);
			pParameter->CheckMandatory();
		}
	}

	// Пытается разобрать строку вида key=value, в случае неудачи выбрасывает исключение
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

	// Сохраняет все параметры секции в файле в формате key=value
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
	/* Критерий равенства: секции считаются равными, если их ключи равны (без учета регистра) */
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
	// Предоставляет функциональность по работе с ini-файлами
	//

	// Construction/Destruction


	/*
	 bAllowInvalidStrings - определяет поведение в случае обнаружения неверной 
		(не являющейся именем секции или параметром) строки
		true - пропустить, false - возбудить исключение

	 bAllowUnkSections - определяет поведение в случае обнаружения неизвестной секции:
		true - пропустить, false - возбудить исключение

	 bAllowUnkKeys - определяет поведение в случае обнаружения неизвестного ключа:
		true - пропустить, false - возбудить исключение
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

	// Читает данные из ini-файла в связанные переменные
	void CParser::ParseFile(const char *pchFilePath) throw (CException *)
	{
		// Открыть файл
		std::fstream file(pchFilePath, std::fstream::in | std::fstream::binary);
		if(!file)
			throw CException::Create(FileSystemError, errno);

		ParseStream(file);
	}

	// Сохраняет данные из связанных переменных в ini-файл
	void CParser::StoreToFile(const char *pchFilePath) const throw (CException *)
	{
		std::fstream file(pchFilePath, std::fstream::out | std::fstream::binary | std::fstream::trunc);
		if(!file)
			throw CException::Create(FileSystemError, errno);

		StoreToStream(file);
	}

	// Читает данные из потока в связанные переменные
	void CParser::ParseStream(std::istream &file) throw (CException *)
	{
		// Сбросим во всех параметрах признак m_bFinded
		ClearFindedFlag();

		// Разбор потока
		std::string str;
		int nLine = 0, nVirtLine = 0;

		while (std::getline(file, str)) {
			nLine++;
			nVirtLine++;

			// Убираем пробельные символы с обеих сторон
			TrimBoth(str);

			if (str.length() < 1) {
				nVirtLine--;
				continue;
			}

			if ('[' == str[0]) {
				// Это секция

				// Выделим имя секции
				str = str.substr(1, str.length() - 2);
				m_pCurrentSection = GetSection(str);
				if (!m_pCurrentSection) {
					if (m_bAllowUnkSections)
						continue;
					else
						throw CException::Create(UnknownSection, nLine);
				}
			} else if (chComment == str[0]) {
				// Это комментарий, добавим его в отображение
				m_CommentStrings[nVirtLine] = str;
				continue;
			} else{
				// Это параметр
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

		// Проверим все ли обязательные параметры найдены
		CheckMandatory();
	}

	// Сохраняет данные из связанных переменных в поток
	void CParser::StoreToStream(std::ostream &rStream) const throw (CException *)
	{
		int nLine = 0;
		try {
			// Проход по всем секциям
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

				// Проход по всем параметрам в секции
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
			
			// Запись оставшихся комментариев
			TCommentStrConstIt cit = m_CommentStrings.upper_bound(nLine);
			while (cit != m_CommentStrings.end())
				rStream << (*cit++).second << std::endl;
		} catch (CException *pExc) {
			pExc->m_nLineNum = nLine;
			throw;
		}
	}

	// Глобальные операторы для работы с (класс CParser) ini-файлом
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

	// Добавляет секцию
	CSection *CParser::AddSection(const char *pchSectionName)
	{
		CSection *pSection = new CSection(pchSectionName);

	#ifdef _DEBUG
		TSectionsConstIt it = m_Sections.begin();
		while (it != m_Sections.end()) {
			CSection *pSect = *it++;
//			assert(pSect);
			if (*pSect == *pSection) {
				// Секция с таким именем уже существует
//				assert(0);
				break;
			}
		}
	#endif

		m_Sections.push_back(pSection);
		return pSection;
	}

	// Удаляет секцию по имени (без префиксного и постфиксного символов), в случае успеха возвращает true
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

	// Удаляет все секции
	void CParser::RemoveAllSections()
	{
		TSectionsConstIt it = GetSectionsBegin();
		while (it != GetSectionsEnd())
			delete *it++;

		m_Sections.empty();
	}
	
	// Возвращает секцию по её имени
	CSection *CParser::GetSection(const char *pchSectionName) const
	{
		return GetSection(std::string(pchSectionName));
	}

	// Возвращает секцию по имени
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

	// Сбрасывает флаг m_bFinded во всех параметрах
	void CParser::ClearFindedFlag()
	{
		TSectionsConstIt it = GetSectionsBegin();
		while (it != GetSectionsEnd()) {
			CSection *pSection = *it++;
//			assert(pSection);
			pSection->ClearFindedFlag();
		}
	}

	// Проверяет считаны ли обязательные параметры, если нет - исключение
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
	// Классы значений
	//
	//


	//
	// Class CStdStringValue
	// Представляет строковое значение (std::string)
	//

	/* 
	 pstrData - адрес, по которому будет записываться/читаться значение 
	 bAllowEmpty - true - разрешить пустые строки
	*/
	CStdStringValue::CStdStringValue(std::string *pstrData, int nMaxLen, bool bAllowEmpty)
		: m_pstrData(pstrData), m_nMaxLen(nMaxLen), m_bAllowEmpty(bAllowEmpty)
	{
//		assert(m_pstrData);
	}

	// interface CCommonValue

	/* Пытается пребразовать содержимое строки pchString в 
	 значение определенного типа, в случае неудачи выбрасывает исключение */
	void CStdStringValue::Parse(const std::string &rString) throw (CException *)
	{
//		assert(m_pstrData);
		*m_pstrData = rString;
		if (!IsSatisfy())
			throw CException::Create(ValueOutOfRanges);
	}

	/* Возвращает строковое представление значения */
	std::string CStdStringValue::ToString() const
	{
//		assert(m_pstrData);
		if (!IsSatisfy())
			throw CException::Create(ValueOutOfRanges);
		return *m_pstrData;
	}

	/* Проверяет состояние объекта и возвращает true, если в
	 связанной переменной находится корректное (удовлетвлряющее 
	 ограничениям) значение */
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
	// Представляет строковое значение (стандартная C-строка, с известной длиной)
	//

	/* 
	 pchData - адрес, по которому будет записываться/читаться значение 
	 nMaxLen - максимальная длина строки
	 bAllowEmpty - true - разрешить пустые строки
	*/
	CStringValue::CStringValue(char *pchData, int nMaxLen, bool bAllowEmpty)
		: m_pchData(pchData), m_nMaxLen(nMaxLen), m_bAllowEmpty(bAllowEmpty)
	{
//		assert(m_pchData);
	}

	// interface CCommonValue
	/* Пытается пребразовать содержимое строки rString в 
	 значение определенного типа, в случае неудачи выбрасывает исключение */
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

	/* Возвращает строковое представление значения */
	std::string CStringValue::ToString() const
	{
//		assert(m_pchData);
		if (!IsSatisfy())
			throw CException::Create(ValueOutOfRanges);
		return std::string(m_pchData);
	}

	/* Проверяет состояние объекта и возвращает true, если в
	 связанной переменной находится корректное (удовлетвлряющее 
	 ограничениям) значение */
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
//	// Проедставляет цвет в кодировке RGB
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
//	/* Пытается пребразовать содержимое строки rString в 
//	 значение определенного типа, в случае неудачи выбрасывает исключение */
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
//	/* Возвращает строковое представление значения */
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
//	/* Проверяет состояние объекта и возвращает true, если в
//	 связанной переменной находится корректное (удовлетвлряющее 
//	 ограничениям) значение */
//	bool CRGBValue::IsSatisfy() const
//	{
////		assert(m_pColor);
//		return true;
//	}



	//
	// Class CBoolValue
	// Представляет значения булевского типа
	// Может принимать значения: (true, false), (yes, no), (1, 0)
	//

	// Construction
	CBoolValue::CBoolValue(bool *pbData, TBoolStyle style)
		: m_pbData(pbData), m_Style(style)
	{
//		assert(m_pbData);
	}

	// interface CCommonValue
	/* Пытается пребразовать содержимое строки rString в 
	 значение определенного типа, в случае неудачи выбрасывает исключение */
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

	/* Возвращает строковое представление значения */
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

	/* Проверяет состояние объекта и возвращает true, если в
	 связанной переменной находится корректное (удовлетвлряющее 
	 ограничениям) значение */
	bool CBoolValue::IsSatisfy() const
	{
//		assert(m_pbData);
		return true;
	}

}
