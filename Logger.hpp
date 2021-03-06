//���������� ��������� ������� ������ Logger.h
#ifndef LOGGER_HPP_
#define LOGGER_HPP_
#include <typeinfo>
#include "TConverter.cpp"

template <class T>
NS_Logger::TLog& NS_Logger::TLog::operator+=(T val)
{
	using std::cerr;
	using std::endl;
	using std::string;
	try
	{
		string tmp = NS_Converter::toStr(val);
		msg << tmp;
	}
	catch (const string& er)
	{
		cerr << er << endl;
	}
	catch (...)
	{
		const std::type_info& ti = typeid(val);
		cerr << "TLog::operator+=: ������ �������������� " << ti.name() << endl;
	}
	return *this;
}
/*
template <>
TLog& TLog::operator+=<unsigned short>(unsigned short val)
{
	using std::cerr;
	using std::endl;
	try
	{
		string tmp = NS_Converter::toStr(val, false);
		msg << tmp;
	}
	catch (const TLog& er)
	{
		cerr << er.what() << endl;
	}
	catch (...)
	{
		cerr << "TLog::operator+=: ������ �������������� ���� unsigned short!" << endl;
	}
	return *this;
}

template <>
TLog& TLog::operator+=<const char*>(const char* val)
{
	using std::cerr;
	using std::endl;
	try
	{
		string tmp = NS_Converter::toStr(val, false);
		msg << tmp;
	}
	catch (const TLog& er)
	{
		cerr << er.what() << endl;
	}
	catch (...)
	{
		cerr << "TLog::operator+=: ������ �������������� ���� const char*!" << endl;
	}
	return *this;
}

template <>
TLog& TLog::operator+=<const string&>(const string& val)
{
	using std::cerr;
	using std::endl;
	try
	{
		string tmp = NS_Converter::toStr(val, false);
		msg << tmp;
	}
	catch (const TLog& er)
	{
		cerr << er.what() << endl;
	}
	catch (...)
	{
		cerr << "TLog::operator+=: ������ �������������� ���� const string&!" << endl;
	}
	return *this;
}

template <>
TLog& TLog::operator+= <const NS_Logger::TLog&>(const NS_Logger::TLog& val)
{
	if (!val.isEmpty() and val.isGood())
		operator+= <const char*>(val.getStr().c_str());
	return *this;
}

template <>
TLog& TLog::operator+= <const NS_Logger::TLog&>(const NS_Logger::TLog& val)
{
	if (!val.isEmpty() and val.isGood())
		operator+=<const char*>(val.getStr().c_str());
	return *this;
}
/**/

/*
template <>
TLog& TLog::operator+= <const string&>(const string& val)
{
	try
	{
		msg << val;
	}
	catch (const TLog& er)
	{
		cerr << er.what() << endl;
	}
	catch (...)
	{
		const std::type_info& ti = typeid(val);
		cerr << "TLog::operator+=: ������ �������������� " << ti.name() << " �� ���������: " << val << endl;
	}
	return *this;
}

template <>
TLog& TLog::operator<<(unsigned short val)
{
	return operator+=<unsigned short>(val);
}
/*
template <>
TLog& TLog::operator<< <const string&>(const string& val)
{
	return operator+= <const string&>(val);
}

template <>
TLog& TLog::operator<< <const char*>(const char* val)
{
	return operator+= <const char*>(val);
}

template <>
TLog& TLog::operator<<(const NS_Logger::TLog& val)
{
	return operator+= <const string&>(val.getStr());
}
/**/
#endif LOGGER_HPP_