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
                result[index] : = '\n';
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
    WSADATA wsa;
    int ret = WSAStartup(MAKEWORD(2, 0), &wsa);
    if(ret != 0)
    {
        std::cout << "Winsock init error: " << ret << std::endl;
        goto done;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == INVALID_SOCKET)
    {
        std::cout << "socket() error: " << WSAGetLastError() << std::endl;
        goto done;
    }

    SOCKADDR_IN addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("52.25.27.117"); //the ip
    addr.sin_port = htons(6667);

    if(connect(sock, (SOCKADDR*)&addr, sizeof(addr)) != 0)
    {
        std::cout << "connect() error: " << WSAGetLastError() << std::endl;
        goto cleanup:
    }

    std::cout << "connected" << std::endl;

    std::string oauth = ...;
    std::string nick = ...;
    std::string user = ...;
    std::string channel = ...;

    sendCmd("PASS " + oauth);
    sendCmd("NICK " + nick);
    sendCmd("USER " + user);
    sendCmd("JOIN " + channel);

    char buf[1024];
    std::string LineBuffer;
    std::string::size_type StartIdx = 0;

    do
    {
        int ret = recv(sock, buf, sizeof(buf), 0);
        if(ret == SOCKET_ERROR)
        {
            std::cout << "recv() error: " << WSAGetLastError() << std::endl;
            goto cleanup;
        }

        if(ret == 0)
        {
            std::cout << "Server disconnected" << std::endl;
            break;
        }

        LineBuffer.append(buf, ret);

        do
        {
            std::string::size_type pos = LineBuffer.find('\n', StartIdx);
            if(pos == std::string::npos)
                break;

            std::string::size_type len = pos;
            if((pos > 0) && (LineBuffer[pos - 1] == '\r'))
                --len;

            std::string msg = unquote(LineBuffer.substr(0, len));
            LineBuffer.erase(0, pos + 1);
            StartIdx = 0;

            std::string senderNick;
            std::string senderHost;

            if(!msg.empty() && (msg[0] == ':'))
            {
                std::string tmp = fetch(msg, " ");
                tmp.erase(0, 1); // remove ':'
                senderNick = fetch(tmp, "!");
                senderHost = tmp;
            }

            std::cout << "Received: " << msg << std::endl;

            if(msg == "PING")
                sendCmd("PONG :" + hostname);
        } while(true);
    } while(true);
}
