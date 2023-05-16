#include "NewTwitchApi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <cstdlib>
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define DEFAULT_BUFLEN 512


void replaceStr(std::string& str, const std::string& oldStr, const std::string& newStr)
{
    std::string::size_type index = 0;
    do
    {
        index = str.find(oldStr, index);
        if(index == std::string::npos)
            return;

        str.replace(index, oldStr.length(), newStr);
        index += newStr.length();
    } while(true);
}

std::string quote(const std::string& s)
{
    std::string result = s;
    replaceStr(result, "\x10", "\x10""\x10");
    replaceStr(result, "\0", "\x10""0");
    replaceStr(result, "\n", "\x10""n");
    replaceStr(result, "\r", "\x10""r");
    return result;
}

std::string unquote(const std::string& s)
{
    std::string result = s;
    std::string::size_type len = result.length();
    std::string::size_type index = 0;
    while(index < len)
    {
        index = result.find("\x10", index);
        if(index = std::string::npos)
            break;

        result.erase(index, 1);
        --len;

        if(index >= len)
            break;

        switch(result[index])
        {
            case '0':
                result[index] = '\0';
                break;
            case 'n':
                result[index] = '\n';
                break;
            case 'r':
                result[index] = '\r';
                break;
        }

        ++index;
    }

    return result;
}

std::string fetch(std::string& s, const std::string& delim)
{
    std::string result;
    std::string::size_type pos = s.find(delim);
    if(pos == std::string::npos)
    {
        result = s;
        s = "";
    }
    else
    {
        result = s.substr(0, pos);
        s.erase(0, pos + delim.length());
    }
    return result;
}

bool sendStr(SOCKET sock, const std::string& s)
{
    const char* ptr = s.c_str();
    int len = s.length();

    while(len > 0)
    {
        int ret = send(sock, ptr, len, 0);
        if(ret == SOCKET_ERROR)
        {
            std::cout << "send() error: " << WSAGetLastError() << std::endl;
            return false;
        }
        ptr += ret;
        len -= ret;
    }

    return true;
}

bool sendCmd(SOCKET sock, const std::string& cmd)
{
    std::cout << "Sending: " << cmd << std::endl;
    return sendStr(sock, quote(cmd)) && sendStr(sock, "\r\n");
}

void NewTwitchApi::Start(const std::string& aOAuth, const std::string& aNickname)
{
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return;
    }

    const char* serverHostname = "irc.chat.twitch.tv";
    const char* serverPort = "6667";

    struct addrinfo hints, * result = nullptr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(serverHostname, serverPort, &hints, &result) != 0) 
    {
        std::cerr << "Failed to resolve server hostname." << std::endl;
        WSACleanup();
        return;
    }

    SOCKET socketDescriptor;
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socketDescriptor == INVALID_SOCKET) 
    {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return;
    }

    if(connect(socketDescriptor, result->ai_addr, static_cast<int>(result->ai_addrlen)) == SOCKET_ERROR) 
    {
        std::cerr << "Failed to connect to the server." << std::endl;
        closesocket(socketDescriptor);
        WSACleanup();
        return;
    }

    // Send the authentication message
    std::string authMessage = "PASS " + std::string(aOAuth) + "\r\n";
    send(socketDescriptor, authMessage.c_str(), authMessage.length(), 0);

    // Send the username
    std::string userMessage = "NICK " + std::string(aNickname) + "\r\n";
    send(socketDescriptor, userMessage.c_str(), userMessage.length(), 0);

    // Join the desired channel (replace "target_channel" with the channel you want to join)
    std::string joinMessage = "JOIN #maikatura\r\n";
    send(socketDescriptor, joinMessage.c_str(), joinMessage.length(), 0);

    const int bufferSize = 1024;
    char buffer[bufferSize];

  /*  std::string testMessage = "PRIVMSG #maikatura :lol\r\n";
    send(socketDescriptor, testMessage.c_str(), testMessage.length(), 0);*/

    while(true) 
    {
        // Receive data
        int bytesReceived = recv(socketDescriptor, buffer, bufferSize - 1, 0);
        if(bytesReceived == SOCKET_ERROR) 
        {
            std::cerr << "Error in receiving data." << std::endl;
            break;
        }

        if(bytesReceived == 0) 
        {
            // Connection closed
            std::cout << "Connection closed by the server." << std::endl;
            break;
        }

        // Null-terminate the received data
        buffer[bytesReceived] = '\0';


        // Process received data
        std::string receivedData(buffer);
        std::cout << "Received data: " << receivedData << std::endl;

        // Check if it's a PING message
        if(receivedData.substr(0, 4) == "PING") {
            // Extract the token from the PING message
            std::string token = receivedData.substr(5);

            // Send PONG response
            std::string pongMessage = "PONG :" + token + "\r\n";
            send(socketDescriptor, pongMessage.c_str(), pongMessage.length(), 0);
        }

    }

} 
