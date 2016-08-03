/*
 * HttpHeaderParser.h
 *
 *  Created on: July 11, 2016
 *      Author: z.j
 */

#ifndef HTTP_HEADER_PARSER_H
#define HTTP_HEADER_PARSER_H

#include <string>

namespace http {

    class HttpRequest;

    class HttpHeaderParser {
    public:
        HttpHeaderParser();

        void init(HttpRequest* theHttpRequest);

        bool parse(unsigned char theChar);

    private:
        void saveHeader();
        bool isTokenChar(int theChar);
        bool isChar(int theChar);

        typedef enum
        {
            IdleE,
            CrParsedE,
            FieldNameParsedE,
            FieldNameSpaceParsedE,
            FieldNameColonParsedE,
            FieldNameColonCrParsedE,
            FieldNameColonCrLfParsedE,
            FieldValueParsedE,
            FieldValueTextParsedE,
            FieldValueQuotedStringParsedE,
            FieldValueQuotedPairParsedE,
            FieldValueCrParsedE,
            FieldValueCrLfParsedE,
            FieldValueLwsParsedE,
            FieldValueQuotedStringCrParsedE,
            FieldValueQuotedStringCrLfParsedE,
            FieldValueErrorE,
            FieldValueErrorCrParsedE,
            FieldValueErrorCrLfParsedE
        } State;

      std::string m_headerName;
      std::string m_headerValue;
      HttpRequest* m_httpRequest;
      State m_state;
      unsigned int m_bytesParsed;
      unsigned int m_maxHeaderBytes;        
    };
}

#endif
