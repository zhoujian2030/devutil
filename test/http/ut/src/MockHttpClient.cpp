/*
 * MockHttpClient.cpp
 *
 *  Created on: July 8, 2016
 *      Author: z.j
 */

#include "MockHttpClient.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp> 

using boost::asio::ip::tcp;

// --------------------------------------
MockHttpClient::MockHttpClient() {

}

// --------------------------------------
MockHttpClient::~MockHttpClient() {

}

// --------------------------------------
void MockHttpClient::sendRequest(http::HttpRequest* theRequest) {
    std::string method = theRequest->getMethod();
    boost::algorithm::to_upper(method);

    // Parse URL
    std::string url = theRequest->getUrl();
    std::cout << url << std::endl;
    std::size_t pos = url.find("http://");
    if (pos == std::string::npos) {
        std::cout << "Invalid URL!" << std::endl;
        return;
    }
    url = url.substr(7);
    std::cout << url << std::endl;

    std::string address;
    std::string uri;
    pos = url.find("/");
    if (pos == std::string::npos) {
        address = url;
        uri = "/";
    } else {
        address = url.substr(0, pos);
        uri = url.substr(pos);
    }
    std::cout << "address = " << address << ", uri = " << uri << std::endl;

    std::string queryParam1, queryParam2;
    pos = address.find(":");
    if (pos == std::string::npos) {
        queryParam1 = address;
        queryParam2 = "http";
    } else {
        queryParam1 = address.substr(0, pos);
        queryParam2 = address.substr(pos+1);
    }
    std::cout << "queryParam1 = " << queryParam1 << ", queryParam2 = " << queryParam2 << std::endl;

    // Get a list of endpoints corresponding to the server name.
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(queryParam1, queryParam2);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    // Form the request.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << method << " " << uri << " HTTP/1.1\r\n";
    http::HttpRequest::HeaderMap headers = theRequest->getHeaders();
    for (http::HttpRequest::HeaderMap::iterator it = headers.begin(); it != headers.end(); ++it) {
        request_stream << it->first << ": " << it->second << " \r\n";
    }
    request_stream << "\r\n";

    // Send the request.
    boost::asio::write(socket, request);
}

// --------------------------------------
void MockHttpClient::post() {
    boost::asio::io_service io_service;

    // Get a list of endpoints corresponding to the server name.
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("127.0.0.1", "1234");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "POST /vowifi HTTP/1.1\r\n";
    request_stream << "Host: 127.0.0.1 \r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Send the request.
    boost::asio::write(socket, request);
}
