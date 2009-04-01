//
// LineEndingConverter.cpp
//
// $Id: //poco/1.3/Foundation/src/LineEndingConverter.cpp#1 $
//
// Library: Foundation
// Package: Streams
// Module:  LineEndingConverter
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/LineEndingConverter.h"


namespace Poco {


#if defined(_WIN32)
#define POCO_DEFAULT_NEWLINE_CHARS "\r\n"
#else
#define POCO_DEFAULT_NEWLINE_CHARS "\n"
#endif


const std::string LineEnding::NEWLINE_DEFAULT(POCO_DEFAULT_NEWLINE_CHARS);
const std::string LineEnding::NEWLINE_CR("\r");
const std::string LineEnding::NEWLINE_CRLF("\r\n");
const std::string LineEnding::NEWLINE_LF("\n");


LineEndingConverterStreamBuf::LineEndingConverterStreamBuf(std::istream& istr): 
	_pIstr(&istr), 
	_pOstr(0), 
	_newLine(LineEnding::NEWLINE_DEFAULT),
	_lastChar(0)
{
	_it = _newLine.end();
}


LineEndingConverterStreamBuf::LineEndingConverterStreamBuf(std::ostream& ostr): 
	_pIstr(0), 
	_pOstr(&ostr), 
	_newLine(LineEnding::NEWLINE_DEFAULT),
	_lastChar(0)
{
	_it = _newLine.end();
}


LineEndingConverterStreamBuf::~LineEndingConverterStreamBuf()
{
}


void LineEndingConverterStreamBuf::setNewLine(const std::string& newLineCharacters)
{
	_newLine = newLineCharacters;
	_it      = _newLine.end();
}


const std::string& LineEndingConverterStreamBuf::getNewLine() const
{
	return _newLine;
}


int LineEndingConverterStreamBuf::readFromDevice()
{
	poco_assert_dbg (_pIstr);

	while (_it == _newLine.end())
	{
		int c = _pIstr->get();
		if (c == '\r')
		{
			if (_pIstr->peek() == '\n') _pIstr->get();
			_it = _newLine.begin();
		}
		else if (c == '\n')
		{
			_it = _newLine.begin();
		}
		else return c;
	}
	return *_it++;
}


int LineEndingConverterStreamBuf::writeToDevice(char c)
{
	poco_assert_dbg (_pOstr);

	if (c == '\r' || (c == '\n' && _lastChar != '\r'))
		_pOstr->write(_newLine.data(), (std::streamsize) _newLine.length());
	if (c != '\n' && c != '\r')
		_pOstr->put(c);
	_lastChar = c;
	return charToInt(c);
}


LineEndingConverterIOS::LineEndingConverterIOS(std::istream& istr): _buf(istr)
{
	poco_ios_init(&_buf);
}


LineEndingConverterIOS::LineEndingConverterIOS(std::ostream& ostr): _buf(ostr)
{
	poco_ios_init(&_buf);
}


LineEndingConverterIOS::~LineEndingConverterIOS()
{
}


void LineEndingConverterIOS::setNewLine(const std::string& newLineCharacters)
{
	_buf.setNewLine(newLineCharacters);
}


const std::string& LineEndingConverterIOS::getNewLine() const
{
	return _buf.getNewLine();
}


LineEndingConverterStreamBuf* LineEndingConverterIOS::rdbuf()
{
	return &_buf;
}


InputLineEndingConverter::InputLineEndingConverter(std::istream& istr): 
	LineEndingConverterIOS(istr), 
	std::istream(&_buf)
{
}


InputLineEndingConverter::InputLineEndingConverter(std::istream& istr, const std::string& newLineCharacters): 
	LineEndingConverterIOS(istr), 
	std::istream(&_buf)
{
	setNewLine(newLineCharacters);
}


InputLineEndingConverter::~InputLineEndingConverter()
{
}


OutputLineEndingConverter::OutputLineEndingConverter(std::ostream& ostr): 
	LineEndingConverterIOS(ostr), 
	std::ostream(&_buf)
{
}


OutputLineEndingConverter::OutputLineEndingConverter(std::ostream& ostr, const std::string& newLineCharacters): 
	LineEndingConverterIOS(ostr), 
	std::ostream(&_buf)
{
	setNewLine(newLineCharacters);
}


OutputLineEndingConverter::~OutputLineEndingConverter()
{
}


} // namespace Poco
