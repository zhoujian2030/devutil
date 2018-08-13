/*
 * HttpParserException.h
 *
 *  Created on: July 11, 2016
 *      Author: z.j
 */

#ifndef HTTP_PARSER_EXCEPTION_H
#define HTTP_PARSER_EXCEPTION_H

#include <string>

namespace http {
    
    class HttpParserException {
    public:
        HttpParserException(std::string description, int theExceptionChar);

    private:
        std::string m_description;
    };

    // ---------------------------------------------------------
    inline HttpParserException::HttpParserException(std::string description, int theExceptionChar) 
    : m_description(description)
    {

    }
}

#endif
