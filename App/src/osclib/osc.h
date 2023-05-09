#ifndef OSC_H
#define OSC_H

#include <cstdint>
#include <functional>
#include <string>

namespace osc {

class Value {
	friend class Receiver;
public:
	enum class Type {
		Void,
		Bool,
		Int,
		Float,
		String,
	};
private:
	Type type_ = Type::Void;
	union {
		bool b;
		int32_t i;
		float f;
	} v;
	std::string v_s;
public:
	Type type() const
	{
		return type_;
	}
	bool GetBoolValue() const
	{
		return v.b;
	}

	int32_t GetIntValue() const
	{
		return v.i;
	}

	float GetFloatValue() const
	{
		return v.f;
	}

	std::string GetStringValue() const
	{
		return v_s;
	}
};

struct Listener {
	std::function<void(char const *, int)> received;
	std::function<void(std::string const &addr, Value const &)> value;
};

class Transmitter {
private:
	struct Private;
	Private *m;
public:
	Transmitter();
	~Transmitter();

	void Open(char const *hostname);
	void Close();

	void SendBool(const std::string &addr, bool val);
	void SendInt(const std::string &addr, int32_t val);
	void SendFloat(const std::string &addr, float val);
	void SendString(const std::string& addr, const std::string val);
	void SendChatboxMessage(const std::string& addr, const std::string val);
};

class Receiver {
private:
	struct Private;
	Private *m;

	void Run();
	bool IsInterruptionRequested() const;
	void Start();
	void Stop();
public:
	Receiver();
	~Receiver();
	Listener* SetListener(Listener* listener);
	void Open(char const *hostname);
	void Close();
};

} // namespace osc

#endif // OSC_H
