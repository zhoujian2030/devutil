/*
 * HttpHeaderParser.cpp
 *
 *  Created on: July 11, 2016
 *      Author: z.j
 */

#include "HttpHeaderParser.h"
#include "HttpRequest.h"
#include "HttpLogger.h"
#include "HttpCommon.h"
#include "HttpParserException.h"

using namespace http;
using namespace std;

// -----------------------------------------------
HttpHeaderParser::HttpHeaderParser() {
    HttpLogger::initConsoleLog();
}

// -----------------------------------------------
void HttpHeaderParser::init(HttpRequest* theHttpRequest) {
    m_httpRequest = theHttpRequest;
    m_state = IdleE;
    m_bytesParsed = 0;
    m_maxHeaderBytes = MAX_HTTP_HEADER_SIZE;
}

// -----------------------------------------------
bool HttpHeaderParser::parse(unsigned char theChar) {
    // LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "HttpHeaderParser::parse(), " << theChar);

    m_bytesParsed++;

    if (m_bytesParsed > m_maxHeaderBytes)
    {
        LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "throw exception: HttpHeaderParser - Header size limit exceeded");
        throw HttpParserException(
            "HttpHeaderParser - Header size limit exceeded",
            theChar);
    }

    switch (m_state)
    {
    case IdleE:
    {
        m_headerName.clear();
        m_headerValue.clear();
        switch (theChar)
        {
        case '\r':
    {
        m_state = CrParsedE;
        return false;
    }
        case '\n':
        {
            m_state = IdleE;
            return true; // we are done
        }
        default:
        {
            if (isTokenChar(theChar))
            {
                m_headerName += theChar;
                m_state = FieldNameParsedE;
                return false;
            }
            else
            {
                LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "throw exception: HttpHeaderParser - IdleE");
                throw HttpParserException(
                        "HttpHeaderParser - IdleE",
                        theChar);
            }
        }
        }
        break;
    }

    // ---------------------------------------------------------------------

    case CrParsedE:
    {
        if (theChar == '\n')
        {
            m_state = IdleE;
            return true; // we are done
        }
        else
        {
            LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "throw exception: HttpHeaderParser - CrParsedE");
            throw HttpParserException(
                    "HttpHeaderParser - CrParsedE",
                    theChar);
        }
    }

    // ---------------------------------------------------------------------

    case FieldNameParsedE:
    {
        if (isTokenChar(theChar))
        {
            m_headerName += theChar;
            // remain in this state
            return false;
        }
        else
            if (theChar == ':')
            {
                m_state = FieldNameColonParsedE;
            }
            else
                if (theChar == ' ' ||
                        theChar == '\t')
                {
                    m_state = FieldNameSpaceParsedE;
                    return false;
                }
                else
                {
                    LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "throw exception: HttpHeaderParser - FieldNameParsedE");
                    throw HttpParserException(
                            "HttpHeaderParser - FieldNameParsedE",
                            theChar);
                }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldNameSpaceParsedE:
    {
        if (theChar == ':')
        {
            m_state = FieldNameColonParsedE;
        }
        else
            if (theChar == ' ' ||
                    theChar == '\t')
            {
                // remain in this state
                return false;
            }
            else
            {
                LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "throw exception: HttpHeaderParser - FieldNameSpaceParsedE");
                throw HttpParserException(
                        "HttpHeaderParser - FieldNameSpaceParsedE",
                        theChar);
            }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldNameColonParsedE:
    {
        switch (theChar)
        {
        case ' ':
        case '\t':
        {
            // remain in this state
            return false;
        }
        case '\r':
        {
            m_state = FieldNameColonCrParsedE;
            return false;
        }
        case '\n':
        {
            m_state = FieldNameColonCrLfParsedE;
            return false;
        }
        case '"':
        {
            m_headerValue += theChar;
            m_state = FieldValueQuotedStringParsedE;
            return false;
        }
        default:
        {
            // HM66317
            //               if (Htag::HttpParserTools::isTextChar(theChar))
            //               {
            m_headerValue += theChar;
            m_state = FieldValueTextParsedE;
            return false;
            //               }
            //               else
            //               {
            //                  m_headerValue += theChar;
            //                  m_state = FieldValueErrorE;
            //                  return false;
            //               }
        }
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldNameColonCrParsedE:
    {
        if (theChar == '\n')
        {
            m_state = FieldNameColonCrLfParsedE;
            return false;
        }
        else
        {
            LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "throw exception: HttpHeaderParser - FieldNameColonCrParsedE");
            throw HttpParserException(
                    "HttpHeaderParser - FieldNameColonCrParsedE",
                    theChar);
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldNameColonCrLfParsedE:
    {
        switch (theChar)
        {
        case ' ':
        case '\t':
        {
            m_state = FieldNameColonParsedE;
            return false;
        }
        case '\r':
        {
            // a new header starts here - store current header
            // and reset internals
            saveHeader();
            m_state = CrParsedE;
            return false;
        }
        case '\n':
        {
            // a new header starts here - store current header
            // and reset internals
            saveHeader();
            m_state = IdleE;
            return true; // we are done
        }
        default:
        {
            if (isTokenChar(theChar))
            {
                // a new header starts here - store current header
                // and reset internals
                saveHeader();
                m_headerName += theChar;
                m_state = FieldNameParsedE;
                return false;
            }
            else
            {
                LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "throw exception: HttpHeaderParser - FieldNameColonCrLfParsedE");
                throw HttpParserException(
                        "HttpHeaderParser - FieldNameColonCrLfParsedE",
                        theChar);
            }
        }
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldValueParsedE:
    {
        switch (theChar)
        {
        case ' ':
        case '\t':
        {
            m_state = FieldValueLwsParsedE;
            return false;
        }
        case '\r':
        {
            m_state = FieldValueCrParsedE;
            return false;
        }
        case '\n':
        {
            m_state = FieldValueCrLfParsedE;
            return false;
        }
        case '"':
        {
            m_headerValue += theChar;
            m_state = FieldValueQuotedStringParsedE;
            return false;
        }
        default:
        {
            // HM66317
            //               if (Htag::HttpParserTools::isTextChar(theChar))
            //               {
            m_headerValue += theChar;
            m_state = FieldValueTextParsedE;
            return false;
            //               }
            //               else
            //               {
            //                  m_headerValue += theChar;
            //                  m_state = FieldValueErrorE;
            //                  return false;
            //               }
        }
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldValueTextParsedE:
    {
        switch (theChar)
        {
        case ' ':
        case '\t':
        {
            m_state = FieldValueLwsParsedE;
            return false;
        }
        case '"':
        {
            m_headerValue += theChar;
            m_state = FieldValueQuotedStringParsedE;
            return false;
        }
        case '\r':
        {
            m_state = FieldValueCrParsedE;
            return false;
        }
        case '\n':
        {
            m_state = FieldValueCrLfParsedE;
            return false;
        }
        default:
        {
            // HM66317
            //               if (Htag::HttpParserTools::isTextChar(theChar))
            //               {
            m_headerValue += theChar;
            // remain in this state
            return false;
            //               }
            //               else
            //               {
            //                  m_headerValue += theChar;
            //                  m_state = FieldValueErrorE;
            //                  return false;
            //               }
        }
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldValueCrParsedE:
    {
        if (theChar == '\n')
        {
            m_state = FieldValueCrLfParsedE;
            return false;
        }
        else
        {
            LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "throw exception: HttpHeaderParser - FieldValueCrParsedE");
            throw HttpParserException(
                    "HttpHeaderParser - FieldValueCrParsedE",
                    theChar);
        }
    }

    // ---------------------------------------------------------------------

    case FieldValueCrLfParsedE:
    {
        switch (theChar)
        {
        case ' ':
        case '\t':
        {
            m_state = FieldValueLwsParsedE;
            return false;
        }
        case '\r':
        {
            // a new header starts here - store current header
            // and reset internals
            saveHeader();
            m_state = CrParsedE;
            return false;
        }
        case '\n':
        {
            // a new header starts here - store current header
            // and reset internals
            saveHeader();
            m_state = IdleE;
            return true; // we are done
        }
        default:
        {
            if (isTokenChar(theChar))
            {
                // a new header starts here - store current header
                // and reset internals
                saveHeader();
                m_headerName += theChar;
                m_state = FieldNameParsedE;
                return false;
            }
            else
            {
                LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "throw exception: HttpHeaderParser - FieldValueCrLfParsedE");
                throw HttpParserException(
                        "HttpHeaderParser - FieldValueCrLfParsedE",
                        theChar);
            }
        }
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldValueLwsParsedE:
    {
        switch (theChar)
        {
        case ' ':
        case '\t':
        {
            // remain in this state
            return false;
        }
        case '\r':
        {
            m_state = FieldValueCrParsedE;
            return false;
        }
        case '\n':
        {
            m_state = FieldValueCrLfParsedE;
            return false;
        }
        case '"':
        {
            m_headerValue += ' ';
            m_headerValue += theChar;
            m_state = FieldValueQuotedStringParsedE;
            return false;
        }
        default:
        {
            m_headerValue += ' ';
            // HM66317
            //               if (Htag::HttpParserTools::isTextChar(theChar))
            //               {
            m_headerValue += theChar;
            m_state = FieldValueTextParsedE;
            return false;
            //               }
            //               else
            //               {
            //                  m_headerValue += theChar;
            //                  m_state = FieldValueErrorE;
            //                  return false;
            //               }
        }
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldValueQuotedStringParsedE:
    {
        switch (theChar)
        {
        case '"':
    {
        m_headerValue += theChar;
        m_state = FieldValueParsedE;
        return false;
    }
        case '\\':
        {
            m_headerValue += theChar;
            m_state = FieldValueQuotedPairParsedE;
            return false;
        }
        case '\r':
        {
            m_state = FieldValueQuotedStringCrParsedE;
            return false;
        }
        case '\n':
        {
            m_state = FieldValueQuotedStringCrLfParsedE;
            return false;
        }
        default:
        {
            // HM66317
            //               if (Htag::HttpParserTools::isTextChar(theChar))
            //               {
            m_headerValue += theChar;
            // remain in this state
            return false;
            //               }
            //               else
            //               {
            //                  m_headerValue += theChar;
            //                  m_state = FieldValueErrorE;
            //                  return false;
            //               }
        }
        }
        break;
    }
    // ---------------------------------------------------------------------

    case FieldValueQuotedPairParsedE:
    {
        if (isChar(theChar))
        {
            m_headerValue += theChar;
            m_state = FieldValueQuotedStringParsedE;
            return false;
        }
        else
        {
            if (theChar == '\r')
            {
                m_state = FieldValueErrorCrParsedE;
            }
            else
            {
                m_headerValue += theChar;
                m_state = FieldValueErrorE;
            }
            return false;
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldValueQuotedStringCrParsedE:
    {
        switch (theChar)
        {
        case '\n':
    {
        m_state = FieldValueQuotedStringCrLfParsedE;
        return false;
    }
        case ' ':
        case '\t':
        {
            m_state = FieldValueQuotedStringParsedE;
            return false;
        }
        case '\r':
        {
            m_state = FieldValueErrorCrParsedE;
            return false;
        }
        default:
        {
            m_headerValue += theChar;
            m_state = FieldValueErrorE;
            return false;
        }
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldValueQuotedStringCrLfParsedE:
    {
        switch (theChar)
        {
        case '\r':
    {
        saveHeader();
        m_state = CrParsedE;
        return false;
    }
        case '\n':
        {
            saveHeader();
            m_state = IdleE;
            return true;
        }
        case ' ':
        case '\t':
        {
            m_state = FieldValueQuotedStringParsedE;
            return false;
        }
        default:
        {
            if (isTokenChar(theChar))
            {
                // a new header starts here - store current header
                // and reset internals
                saveHeader();
                m_headerName += theChar;
                m_state = FieldNameParsedE;
                return false;
            }
            else
            {
                LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "throw exception: HttpHeaderParser - FieldValueQuotedStringCrLfParsedE");
                throw HttpParserException(
                        "HttpHeaderParser - FieldValueQuotedStringCrLfParsedE",
                        theChar);
            }
        }
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldValueErrorE:
    {
        switch (theChar)
        {
        case '\r':
    {
        m_state = FieldValueErrorCrParsedE;
        return false;
    }
        default:
        {
            m_headerValue += theChar;
            return false;
        }
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldValueErrorCrParsedE:
    {
        switch (theChar)
        {
        case '\n':
    {
        m_state = FieldValueErrorCrLfParsedE;
        return false;
    }
        case '\r':
        {
            m_headerValue += theChar;
            return false;
        }
        default:
        {
            m_headerValue += theChar;
            m_state = FieldValueErrorE;
            return false;
        }
        }
        break;
    }

    // ---------------------------------------------------------------------

    case FieldValueErrorCrLfParsedE:
    {
        switch (theChar)
        {
        case '\r':
    {
        saveHeader();
        m_state = CrParsedE;
        return false;
    }
        case '\n':
        {
            saveHeader();
            m_state = IdleE;
            return true;
        }
        case ' ':
        {
            m_headerValue.append("\r\n ");
            m_state = FieldValueErrorE;
            return false;
        }
        case '\t':
        {
            m_headerValue.append("\r\n\t");
            m_state = FieldValueErrorE;
            return false;
        }
        default:
        {
            if (isTokenChar(theChar))
            {
                // a new header starts here - store current header
                // and reset internals
                saveHeader();
                m_headerName += theChar;
                m_state = FieldNameParsedE;
                return false;
            }
            else
            {
                LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "throw exception: HttpHeaderParser - FieldValueErrorCrLfParsedE");
                throw HttpParserException(
                        "HttpHeaderParser - FieldValueErrorCrLfParsedE",
                        theChar);
            }
        }
        }
        break;
    }
    }
    return false;    
}

// ---------------------------------------------
void HttpHeaderParser::saveHeader() {
    m_httpRequest->setHeader(m_headerName, m_headerValue);
    m_headerName.clear();
    m_headerValue.clear();
}

// ---------------------------------------------
bool HttpHeaderParser::isTokenChar(int theChar) {
    if (theChar < 32 || theChar > 126)
    {
        return false;
    }

    switch (theChar)
    {
        case '(':
        case ')':
        case '<':
        case '>':
        case '@':
        case ',':
        case ';':
        case ':':
        case '\\':
        case '"':
        case '/':
        case '[':
        case ']':
        case '?':
        case '=':
        case '{':
        case '}':
        case ' ':
        case '\t':
        {
            return false;
        }
    }

    return true;    
}

// ---------------------------------------------
bool HttpHeaderParser::isChar(int theChar) {
    if (theChar < 0 || theChar > 127)
    {
        return false;
    }
    else
    {
        return true;
    }
}
