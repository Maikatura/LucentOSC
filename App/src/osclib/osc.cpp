
#include "osc.h"
#include <cstring>
#include <iostream>
#include <thread>

#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>

#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <unistd.h>
#define closesocket ::Close
#define ioctlsocket ::ioctl
#define IN_ADDR in_addr
#endif

#include "OscOutboundPacketStream.h"

namespace
{

	bool get_host_by_name(const char* name, IN_ADDR* out)
	{
		struct hostent* he = nullptr;
	#if defined(_WIN32) || defined(__APPLE__)
		he = gethostbyname(name);
	#else
		int err = 0;
		char buf[2048];
		struct hostent tmp;
		gethostbyname_r(name, &tmp, buf, sizeof(buf), &he, &err);
	#endif
		if(he)
		{
			memcpy(&out->s_addr, he->h_addr, sizeof(out->s_addr));
			return true;
		}
		return false;
	}

} // namespace

//// Transmitter ////

struct osc::Transmitter::Private
{
	struct sockaddr_in addr;
	int sock;
};

osc::Transmitter::Transmitter()
	: m(new Private)
{ }

osc::Transmitter::~Transmitter()
{
	Close();
	delete m;
}

void osc::Transmitter::Open(char const* hostname)
{
	Close();

	m->sock = socket(AF_INET, SOCK_DGRAM, 0);

	m->addr.sin_family = AF_INET;
	m->addr.sin_port = htons(9000);
	get_host_by_name(hostname, &m->addr.sin_addr);
}

void osc::Transmitter::Close()
{
	if(m->sock != -1)
	{
		closesocket(m->sock);
		m->sock = -1;
	}
}

void osc::Transmitter::SendString(const std::string& addr, const std::string val)
{
	size_t n;

	char tmp[100];
	osc::OutboundPacketStream packetStream(tmp, 100);
	packetStream << osc::BeginMessage(addr.c_str()) << val.c_str() << osc::EndMessage;
	sendto(m->sock, tmp, packetStream.Size(), 0, (struct sockaddr*)&m->addr, sizeof(m->addr));
}

void osc::Transmitter::SendBool(const std::string& addr, bool val)
{
	size_t n;

	char tmp[100];

	size_t i = addr.size();
	memcpy(tmp, addr.c_str(), i);
	tmp[i++] = 0;
	for(int j = 0; j < 3 && (i & 3); j++) tmp[i++] = 0;

	tmp[i++] = ',';
	tmp[i++] = val ? 'T' : 'F';
	tmp[i++] = 0;
	tmp[i++] = 0;

	sendto(m->sock, tmp, i, 0, (struct sockaddr*)&m->addr, sizeof(m->addr));
}

void osc::Transmitter::SendInt(const std::string& addr, int32_t val)
{
	size_t n;

	char tmp[100];

	size_t i = addr.size();
	memcpy(tmp, addr.c_str(), i);
	tmp[i++] = 0;
	for(int j = 0; j < 3 && (i & 3); j++) tmp[i++] = 0;

	tmp[i++] = ',';
	tmp[i++] = 'i';
	tmp[i++] = 0;
	tmp[i++] = 0;

	tmp[i++] = uint8_t(val >> 24);
	tmp[i++] = uint8_t(val >> 16);
	tmp[i++] = uint8_t(val >> 8);
	tmp[i++] = uint8_t(val);

	sendto(m->sock, tmp, i, 0, (struct sockaddr*)&m->addr, sizeof(m->addr));
}

void osc::Transmitter::SendFloat(const std::string& addr, float val)
{
	size_t n;

	char tmp[100];
	osc::OutboundPacketStream packetStream(tmp, 100);
	packetStream << osc::BeginMessage(addr.c_str()) << val << osc::EndMessage;
	sendto(m->sock, tmp, packetStream.Size(), 0, (struct sockaddr*)&m->addr, sizeof(m->addr));
}



void osc::Transmitter::SendChatboxMessage(const std::string& addr, const std::string val)
{
	size_t n;

	char tmp[100];
	osc::OutboundPacketStream packetStream(tmp, 100);
	packetStream << osc::BeginMessage(addr.c_str()) << val.c_str() << true << true << osc::EndMessage;
	sendto(m->sock, tmp, packetStream.Size(), 0, (struct sockaddr*)&m->addr, sizeof(m->addr));
}

//// Receiver ////

struct osc::Receiver::Private
{
	struct sockaddr_in addr;
	int sock = -1;
	std::thread thread;
	bool interrupted = false;

	osc::Listener* listener = nullptr;
};

osc::Receiver::Receiver()
	: m(new Private)
{ }

osc::Receiver::~Receiver()
{
	Stop();
	Close();
	delete m;
}

osc::Listener* osc::Receiver::SetListener(Listener* listener)
{
	auto old = m->listener;
	m->listener = listener;
	return old;
}

bool osc::Receiver::IsInterruptionRequested() const
{
	return m->interrupted;
}

void osc::Receiver::Run()
{
	while(1)
	{
		char buffer[2048];
		if(IsInterruptionRequested()) break;
		int n = recv(m->sock, buffer, sizeof(buffer), 0);
		if(n < 1)
		{
			if(errno == EAGAIN)
			{
				// not yet
			}
			else
			{
				if(IsInterruptionRequested()) break;
			}
		}
		else
		{
			if(m->listener && m->listener->received)
			{
				m->listener->received(buffer, n);
			}

			Value value;

			int pos = 0;
			int end = sizeof(buffer);
			while(pos < end && buffer[pos]) pos++;
			std::string addr((char const*)buffer, pos);
			pos = pos + 4;
			pos -= pos & 3;
			if(pos + 2 < end && buffer[pos] == ',')
			{
				pos++;
				char c = buffer[pos];
				pos += 3;
				switch(c)
				{
					case 'T':
						value.type_ = Value::Type::Bool;
						value.v.b = true;
						break;
					case 'F':
						value.type_ = Value::Type::Bool;
						value.v.b = false;
						break;
					case 'i':
						value.type_ = Value::Type::Int;
						if(pos + sizeof(int32_t) <= end)
						{
							uint8_t const* s = (uint8_t*)buffer + pos;
							uint8_t* d = (uint8_t*)&value.v.i;
							for(int i = 0; i < sizeof(int32_t); i++)
							{
								d[i] = s[sizeof(int32_t) - 1 - i];
							}
						}
						break;
					case 'f':
						value.type_ = Value::Type::Float;
						if(pos + sizeof(float) <= end)
						{
							uint8_t const* s = (uint8_t*)buffer + pos;
							uint8_t* d = (uint8_t*)&value.v.f;
							for(int i = 0; i < sizeof(float); i++)
							{
								d[i] = s[sizeof(float) - 1 - i];
							}
						}
						break;
					case 's':
						value.type_ = Value::Type::String;
						{
							int len;
							for(len = 0; pos + len < sizeof(buffer); len++)
							{
								if(!buffer[pos + len]) break;
							}
							value.v_s.assign(buffer + pos, len);
						}
						break;
				}

				if(m->listener && m->listener->value)
				{
					m->listener->value(addr, value);
				}
			}
		}
	}
}

void osc::Receiver::Open(char const* hostname)
{
	Close();



	m->sock = socket(AF_INET, SOCK_DGRAM, 0);

	m->addr.sin_family = AF_INET;
	m->addr.sin_port = htons(9001);
	get_host_by_name(hostname, &m->addr.sin_addr);

	Start();
}

void osc::Receiver::Close()
{
	if(m->sock != -1)
	{
		closesocket(m->sock);
		m->sock = -1;
	}
}

void osc::Receiver::Start()
{


	bind(m->sock, (struct sockaddr*)&m->addr, sizeof(m->addr));
	u_long val = 1;
	ioctlsocket(m->sock, FIONBIO, &val);

	
	m->thread = std::thread([&]()
	{
		Run();
	});
}

void osc::Receiver::Stop()
{
	m->interrupted = true;
	if(m->thread.joinable())
	{
		m->thread.join();
	}
	m->thread = {};
}

//