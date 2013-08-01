//////////////////////////////////////////////////////////////////////
// IniParser.h
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

	// Префикс и постфикс имени секции
	extern const char chSectPrefix;
	extern const char chSectPostfix;

	/* Сравнивает две строки rStr1 и rStr2, возвращает true, если они равны
	 !только eng! */
	bool CompareNoCase(const std::string &rStr1, const std::string &rStr2);

	/* Усекает пробельные символы (chWhite) с обеих сторон, справа, слева */
	void TrimBoth(std::string &rString);
	void TrimRight(std::string &rString);
	void TrimLeft(std::string &rString);


	// Предварительное объявление
	class CParameter;
	class CSection;
	class CParser;

	// Коды ошибок
	enum TErrorCodes {
		FileSystemError,			/* Ошибка файловой  системы. 
									  CException::m_nLineNum будет содержать системный код ошибки */
		UnknownSection,				/* Неизвестная секция */
		UnknownParameter,			/* Неизвестный параметр */
		InvalidString,				/* Неверная строка */
		InvalidValue,				/* Неверное значение (ошибка преобразования) */
		ValueOutOfRanges,			/* Значение выходит за заданные рамки */
		ParameterAlreadyDefined,	/* Параметр определен несколько раз */
		MissingMandatoryParam		/* Отсуствует обязательный параметр */
	};


	//
	// Class CException
	// Представляет исключение, могущее возникнуть при работе с ini-файлом
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

	// Конструктор копирования и оператор присваивания
	private:
		CException(const CException&) {}		// no implementation
		CException& operator=(const CException&) {}	// no implementation

	public:
		int GetLineNum() const {return m_nLineNum;}
		TErrorCodes GetCode() const {return m_Cause;}
		const CParameter *GetParameter() const {return m_pParameter;}

	protected:
		/* Номер строки, при разборе которой произошло исключение,
		 если в описании перечисления TErrorCodes для данной ошибки не
		 указано иного смысла этой переменной */
		int m_nLineNum;
		// Код ошибки
		TErrorCodes m_Cause;
		// Указатель на параметр при разборе которого произошло исключение
		const CParameter *m_pParameter;

	friend CParser;
	friend CSection;
	friend CParameter;
	};


	//
	// Class CCommonValue
	// Представляет произвольный тип данных. Все остальные типы данных наследуются от него
	//

	class CCommonValue {
	public:
		virtual ~CCommonValue() {}

	public:
		/* Пытается пребразовать содержимое строки rString в 
		 значение определенного типа, в случае неудачи выбрасывает исключение */
		virtual void Parse(const std::string &rString) throw (CException *) = 0;
		// Возвращает строковое представление значения
		virtual std::string ToString() const throw (CException *) = 0;
		/* Проверяет состояние объекта и возвращает true, если в
		 связанной переменной находится корректное (удовлетвлряющее 
		 ограничениям) значение */
		virtual bool IsSatisfy() const = 0;
	};


	//
	// Class CParameter
	// Представляет параметр
	// Параметр это строка вида "key=value"
	//

	class CParameter {
	// Construction / destruction
	protected:
		/*
		 pchKey - имя ключа
		 pValue - указатель на объект значения
			!ВНИМАНИЕ! этот объект будет уничтожен в деструкторе!
		 bMandatory - если равен true, то это обязательный параметр (если
			он отсутствует, то возбуждается исключение)
		*/
		CParameter(const char *pchKey, CCommonValue *pValue, bool bMandatory = false);
		virtual ~CParameter();

	// Конструктор копирования и оператор присваивания
	private:
		CParameter(const CParameter&) {}			// no implementation
		CParameter& operator=(const CParameter&) {}	// no implementation

	public:
		// Возвращает ключ
		const std::string &GetKey() const {return m_strKey;}
		// Возвращает const указатель на объект значения
		const CCommonValue *GetValue() const {return m_pValue;}
		// Возвращает строковое представление параметра
		std::string ToString() const throw (CException *);

	protected:
		// Пытается разобрать строку, в случае неудачи выбрасывает исключение
		void ParseString(const std::string &rString) throw (CException *);
		
		/* Если это обязательный параметр, то проверяет считан ли он, 
		 если нет, то возбуждается исключение */
		void CheckMandatory() const throw (CException *);
		// Сбрасывает флаг m_bFinded
		void ClearFindedFlag() {m_bFinded = false;}
		bool IsFinded() const {return m_bFinded;}

	// Operators
	protected:
		/* Критерий равенства: параметры считаются равными, если их ключи равны (без учета регистра) */
		bool operator==(const CParameter &par) const;
		bool operator!=(const CParameter &par) const;

	protected:
		// ключ (key)
		std::string m_strKey;
		// указатель на объект значения
		CCommonValue *m_pValue;
		// ==true, то это обязательный параметр
		bool m_bMandatory;
		
		// Метод Parse выставляет его в true, если параметр обнаружен и успешно распознан
		bool m_bFinded;

	friend CSection;
	friend CParser;
	};



	//
	// Class CSection
	// Представляет секцию ini-файла
	//

	class CSection {
	// Construction / destruction
	protected:
		// pchSectionName - имя секции без префиксного и постфиксного символов
		CSection(const char *pchSectionName);
		virtual ~CSection();

	// Конструктор копирования и оператор присваивания
	private:
		CSection(const CSection&) {}			// no implementation
		CSection& operator=(const CSection&) {}	// no implementation

	// Работа с секцией
	public:
		// Добавляет параметр в секцию
		void AddParameter(const char *pchKey, CCommonValue *pValue, bool bMandatory = false);
		// Уничтожает параметр, в случае успеха возвращает true
		bool RemoveParameter(const char *pchKey);
		// Уничтожает все параметры из секции
		void RemoveAllParameters();
		
		// Возвращает параметр по ключу
		CParameter *GetParameter(const char *pchKey) const;
		// Возвращает параметр по ключу
		CParameter *GetParameter(const std::string& rString) const;

		// Вовзращает имя секции (без префикса и постфикса)
		const std::string &GetName() const {return m_strSectionName;}
		// Вовзращает имя секции (с префиксным и постфиксным символами)
		std::string ToString() const {return std::string(chSectPrefix + m_strSectionName + chSectPostfix);}

	protected:
		// Сбрасывает флаг m_bFinded во всех параметрах
		void ClearFindedFlag();
		// Проверяет считаны ли обязательные параметры, если нет - исключение
		void CheckMandatory() const throw (CException *);

		// Пытается разобрать строку вида key=value, в случае неудачи выбрасывает исключение
		void Parse(const std::string &rString) throw (CException *);
		// Сохраняет все параметры секции в поток в формате key=value
		void StoreToStream(std::ostream& file) const throw (CException *);

		// Operators
	protected:
		/* Критерий равенства: секции считаются равными, если их ключи равны (без учета регистра) */
		bool operator==(const CSection &sect) const;
		bool operator!=(const CSection &sect) const;

	// Определения необходимых типов данных
	public:
		typedef std::list<CParameter *> TParameters;
		typedef TParameters::iterator TParametersIt;
		typedef TParameters::const_iterator TParametersConstIt;

	// Навигация по параметрам секции
	public:
		TParametersConstIt GetParametersBegin() const {return m_Parameters.begin();}
		TParametersConstIt GetParametersEnd() const {return m_Parameters.end();}

	protected:
		// Имя секции
		std::string m_strSectionName;
		// Список параметров
		TParameters m_Parameters;

	friend CParser;
	};



	//
	// Class CParser
	// Предоставляет основную функциональность по работе с ini-файлами
	//

	class CParser {
	// Construction / destruction
	public:
		/*
		 bAllowInvalidStrings - определяет поведение в случае обнаружения неверной 
			(не являющейся именем секции или параметром) строки
			true - пропустить, false - возбудить исключение

		 bAllowUnkSections - определяет поведение в случае обнаружения неизвестной секции:
			true - пропустить, false - возбудить исключение

		 bAllowUnkKeys - определяет поведение в случае обнаружения неизвестного ключа:
			true - пропустить, false - возбудить исключение
		*/
		CParser(bool bAllowInvalidStrings = false, bool bAllowUnkSections = true, bool bAllowUnkKeys = true);
		virtual ~CParser();

	public:
		// Читает данные из ini-файла в связанные переменные
		void ParseFile(const char *pchFilePath) throw (CException *);
		// Сохраняет данные из связанных переменных в ini-файл
		void StoreToFile(const char *pchFilePath) const throw (CException *);

		// Читает данные из потока в связанные переменные
		void ParseStream(std::istream &rStream) throw (CException *);
		// Сохраняет данные из связанных переменных в поток
		void StoreToStream(std::ostream &rStream) const throw (CException *);

		// Глобальные операторы для работы с ini-файлом
		friend std::ostream& operator<<(std::ostream& stream, const CParser &pars);
		friend std::istream& operator>>(std::istream& stream, CParser &pars);

	public:
		// Добавляет секцию. Вовзращает указатель на добавленную секцию 
		CSection *AddSection(const char *pchSectionName);
		// Удаляет секцию по имени (без префиксного и постфиксного символов), в случае успеха возвращает true
		bool RemoveSection(const char *pchSectionName);
		// Удаляет все секции
		void RemoveAllSections();
		// Возвращает секцию по её имени
		CSection *GetSection(const char *pchSectionName) const;
		// Возвращает секцию по её имени
		CSection *GetSection(const std::string &rString) const;

	protected:
		// Сбрасывает флаг m_bFinded во всех параметрах
		void ClearFindedFlag();
		// Проверяет считаны ли обязательные параметры, если нет - исключение
		void CheckMandatory() const throw (CException *);

	// Определения необходимых типов данных
	public:
		typedef std::list<CSection *> TSections;
		typedef TSections::iterator TSectionsIt;
		typedef TSections::const_iterator TSectionsConstIt;
		
		typedef std::map<int, std::string> TCommentStr;
		typedef TCommentStr::const_iterator TCommentStrConstIt;
		typedef TCommentStr::value_type TCommentStrPair;

	// Навигация по секциям
	public:
		TSectionsConstIt GetSectionsBegin() const {return m_Sections.begin();}
		TSectionsConstIt GetSectionsEnd() const {return m_Sections.end();}

	protected:
		// Список секций
		TSections m_Sections;
		// Параметры парсера
		bool m_bAllowInvalidStrings, m_bAllowUnkSections, m_bAllowUnkKeys;

		// Секция в которой идет работа
		CSection *m_pCurrentSection;

		// Строки комментария
		TCommentStr m_CommentStrings;
	};




	////////////////////////////////////////////////////////////////////////////////
	//
	//   Классы значений
	//
	//


	//
	// Шаблон CRangeLimits
	// Представляет ограничения на значение
	//

	template <typename T>
	class CRangeLimits {
	// Объявления необходимых типов данных
	public:
		/* Ограничения на значения параметров */
		enum ELimits {
			Min,	// Ограничение снизу (min)
			Max,	// Ограничение сверху (max)
			MinMax	// Ограничение снизу и сверху
		};

	// Construction
	public:
		CRangeLimits(ELimits lim, T min, T max)
			: m_min(min), m_max(max), m_limits(lim)
		{}
	
	public:
		/* Возвращает true, если значение по адресу 
		 pData удовлетворяет ограничениям */
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
	// Шаблон CNumericTypes
	// Представляет значения числовых типов (int, float, double...)
	//

	template <typename T>
	class CNumericType : public CCommonValue {
	// Объявления необходимых типов данных
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
		/* pnData - адрес, по которому будет записываться/читаться значение,
		   pLimits - указатель на объект класса-ограничителя значений
				(в кач-ве параметра шаблона CRangeLimits д.б. указан тот же тип, 
				 что и у класса CNumericType).
				Если ограничений на значение нет, этот параметр д.б. равен NULL
				!ВНИМАНИЕ! объект будет уничтожен в деситрукторе 
		   flags - флаги форматирования
		   nPrecision - точность
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
		/* Пытается пребразовать содержимое строки pchString в 
		 значение определенного типа, в случае неудачи выбрасывает исключение */
		virtual void Parse(const std::string &rString) throw (CException *);
		/* Возвращает строковое представление значения */
		virtual std::string ToString() const throw (CException *);
		/* Проверяет состояние объекта и возвращает true, если в
		 связанной переменной находится корректное (удовлетворяющее 
		 ограничениям) значение */
		virtual bool IsSatisfy() const;

	protected:
		// Подготавливает поток к работе (устанавливает флаги)
		void PrepareStream(std::ostream& stream) const;

	protected:
		T *m_pData;
		CRangeLimits<T> *m_pLimits;
		// Флаги форматироования
		EFlags m_StreamFlags;
		// Точность
		int m_nPrecision;
	};

	/* Пытается пребразовать содержимое строки pchString в 
	 значение определенного типа, в случае неудачи выбрасывает исключение */
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

	/* Возвращает строковое представление значения */
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

	/* Проверяет состояние объекта и возвращает true, если в
	 связанной переменной находится корректное (удовлетворяющее 
	 ограничениям) значение */
	template <typename T>
	bool CNumericType<T>::IsSatisfy() const
	{
//		assert(m_pData);
		if (!m_pLimits)
			return true;
		
		return m_pLimits->IsSatisfy(m_pData);
	}

	// Подготавливает поток к работе (устанавливает флаги)
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
	// Представляет строковое значение (std::string)
	//

	class CStdStringValue : public CCommonValue {
	// Construction
	public:
		/* 
		 pstrData - адрес, по которому будет записываться/читаться значение 
		 nMaxLen - максимальная длина, если == -1, то длина не ограничена
		 bAllowEmpty - true - разрешить пустые строки
		*/
		CStdStringValue(std::string *pstrData, int nMaxLen, bool bAllowEmpty);

	// interface CCommonValue
	public:
		/* Пытается пребразовать содержимое строки rString в 
		 значение определенного типа, в случае неудачи выбрасывает исключение */
		virtual void Parse(const std::string &rString) throw (CException *);
		/* Возвращает строковое представление значения */
		virtual std::string ToString() const throw (CException *);
		/* Проверяет состояние объекта и возвращает true, если в
		 связанной переменной находится корректное (удовлетвлряющее 
		 ограничениям) значение */
		virtual bool IsSatisfy() const;

	protected:
		std::string *m_pstrData;
		int m_nMaxLen;
		bool m_bAllowEmpty;
	};


	//
	// Class CStringValue
	// Представляет строковое значение (стандартная C-строка с известной длиной)
	//

	class CStringValue : public CCommonValue {
	// Construction
	public:
		/* 
		 pchData - адрес, по которому будет записываться/читаться значение 
		 nMaxLen - максимальная длина строки
		 bAllowEmpty - true - разрешить пустые строки
		*/
		CStringValue(char *pchData, int nMaxLen, bool bAllowEmpty);

	// interface CCommonValue
	public:
		/* Пытается пребразовать содержимое строки rString в 
		 значение определенного типа, в случае неудачи выбрасывает исключение */
		virtual void Parse(const std::string &rString) throw (CException *);
		/* Возвращает строковое представление значения */
		virtual std::string ToString() const throw (CException *);
		/* Проверяет состояние объекта и возвращает true, если в
		 связанной переменной находится корректное (удовлетвлряющее 
		 ограничениям) значение */
		virtual bool IsSatisfy() const;

	protected:
		char *m_pchData;
		int m_nMaxLen;
		bool m_bAllowEmpty;
	};



	////
	//// Class CRGBValue
	//// Представляет цвет в кодировке RGB
	////

	///*
	// Грамматика
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
	//	/* Пытается пребразовать содержимое строки rString в 
	//	 значение определенного типа, в случае неудачи выбрасывает исключение */
	//	virtual void Parse(const std::string &rString) throw (CException *);
	//	/* Возвращает строковое представление значения */
	//	virtual std::string ToString() const throw (CException *);
	//	/* Проверяет состояние объекта и возвращает true, если в
	//	 связанной переменной находится корректное (удовлетвлряющее 
	//	 ограничениям) значение */
	//	virtual bool IsSatisfy() const;

	//protected:
	//	COLORREF *m_pColor;
	//};



	//
	// Class CBoolValue
	// Представляет значения булевского типа
	// Может принимать значения: (true, false); (yes, no)
	//

	class CBoolValue : public CCommonValue {
	public:
		enum TBoolStyle {TrueFalse, YesNo};

	// Construction
	public:
		CBoolValue(bool *pbData, TBoolStyle style = TrueFalse);

	// interface CCommonValue
	public:
		/* Пытается пребразовать содержимое строки rString в 
		 значение определенного типа, в случае неудачи выбрасывает исключение */
		virtual void Parse(const std::string &rString) throw (CException *);
		/* Возвращает строковое представление значения */
		virtual std::string ToString() const throw (CException *);
		/* Проверяет состояние объекта и возвращает true, если в
		 связанной переменной находится корректное (удовлетвлряющее 
		 ограничениям) значение */
		virtual bool IsSatisfy() const;

	protected:
		bool *m_pbData;
		TBoolStyle m_Style;
	};

};
