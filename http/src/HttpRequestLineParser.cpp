/*
 * HttpRequestLineParser.cpp
 *
 *  Created on: July 11, 2016
 *      Author: z.j
 */

#include "HttpRequestLineParser.h"
#include "HttpRequest.h"
#include "HttpLogger.h"
#include "HttpParserException.h"

using namespace http;
using namespace std;

// -----------------------------------------------
HttpRequestLineParser::HttpRequestLineParser() {
    HttpLogger::initConsoleLog();
    m_state = IdleE;
}

// -----------------------------------------------
bool HttpRequestLineParser::parse(unsigned char theChar) {
    // LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "HttpRequestLineParser::parse(), " << theChar);

    switch (m_state)
    {
        case IdleE:
        {
            m_method.clear();
            m_url.clear();
            m_version.clear();
            if (isTextChar(theChar) && theChar != ' ')
            {
                m_method += theChar;
                m_state = ParseMethodE;
                return false;
            }
            else
            {
                // Other leading char, ignore
                return false;
            }
            break;
        }

        // ---------------------------------------------------------------------

        case ParseMethodE:
        {
            if (theChar == ' ')
            {
                // There migth be any number of SP between method and URL
                if (m_method.size() != 0)
                {
                    m_state = ParseUrlE;
                }
                return false;
            }
            else if (isTextChar(theChar))
            {
                m_method += theChar;
                //  Remain in this state
                if (m_verifyIfHttp)         // Only allow a max length of 10 char for the method.
                {                          // to be able to detect non HTTP traffic
                    if (m_method.length() > 10)
                    {
                        // not a HTTP request
                        throw HttpParserException("Not HTTP request",
                                                        theChar);
                    }
                }
                return false;
            }
            else
            {
                throw HttpParserException("Error in request line, while parsing Method",
                                        theChar);
            }
            break;
        }

        // ---------------------------------------------------------------------

        case ParseUrlE:
        {
            if (theChar == ' ')
            {
            if (m_url.size() != 0)
            {
                m_state = ParseVersionE;
            }
                return false;
            }
            else if (isTextChar(theChar))
            {
                m_url += theChar;
                // Remain in this state
                return false;
            }
            else
            {
                throw HttpParserException("Error in request line, while parsing URL",
                                        theChar);
            }
            break;
        }

        // ---------------------------------------------------------------------

        case ParseVersionE:
        {
            if (theChar == '\r')
            {
                m_state = CrParsedE;
                return false;
            }
            else if (theChar == '\n')
            {
                // The CR migth be missing, accept that only LF is there 
                m_state = IdleE;
                if (m_verifyIfHttp)
                {
                    string httpToken = "HTTP/";
                    if ((m_version.compare(0, httpToken.size(), httpToken) != 0) && (m_checkHttpProtocol))
                    {
                        // not a HTTP request
                        throw HttpParserException("Not HTTP request",
                                                        theChar);
                    }
                }
                return true;
            }
            else if (theChar == ' ')
            {
                return false;
            }
            else if (isTextChar(theChar))
            {
                m_version += theChar;

                if (m_verifyIfHttp && m_version.size() > 10)
                {
                    // not a HTTP request
                        throw HttpParserException("Not HTTP request",
                                                        theChar);
                }
                // Remain in this state
                return false;
            }
            else
            {
                throw HttpParserException("Error in request line, while parsing Version",
                                        theChar);
            }
            break;
        }

        // ---------------------------------------------------------------------

        case CrParsedE:
        {
            if (theChar == '\n')
            {
                m_state = IdleE;
                if (m_verifyIfHttp)
                {
                    string httpToken = "HTTP/";
                    if ((m_version.compare(0, httpToken.size(), httpToken) != 0) && (m_checkHttpProtocol))
                    {
                        // not a HTTP request
                        throw HttpParserException("Not HTTP request",
                                                        theChar);
                    }
                }
                return true;
            }
            else
            {
                throw HttpParserException("Error in request line, while parsing CR LF",
                                        theChar);
            }
            break;
        }
    }
    return false;
}

// -------------------------------------------
bool HttpRequestLineParser::isTextChar(int theChar) {
    if (theChar < 32 ||
        theChar > 255 ||
        theChar == 127)
    {
        return false;
    }
    else
    {
        return true;
    }
}
