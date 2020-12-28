#ifndef LOG_H
#define LOG_H

#include <cstdio>      // printf
#include <string>      // std::string
#include <string_view> // std::string_view

namespace Inferno {

	enum class Log {
		Log,
		Info,
		Warn,
		Danger,
		Success,
	};

// ----------------------------------------

	class LogStream {
	public:
		LogStream() {}
		virtual ~LogStream() {}

		virtual void write(const char* characters, int length) const = 0;
		virtual void write(const unsigned char* characters, int length) const = 0;
	};

// ----------------------------------------

	class DebugLogStream final : public LogStream{
	public:
		DebugLogStream():
			m_newline(true), m_type(Log::Log) {}
		DebugLogStream(bool newline):
			m_newline(newline), m_type(Log::Log) {}
		DebugLogStream(Log type):
			m_newline(true), m_type(type) { color(); }
		DebugLogStream(Log type, bool newline):
			m_newline(newline), m_type(type) { color(); }
		virtual ~DebugLogStream() override;

		void color() const;

		virtual void write(const char* characters, int length) const override
		{
			for (int i = 0; i < length; i++) {
				printf("%c", characters[i]);
			}
		}

		virtual void write(const unsigned char* characters, int length) const override
		{
			for (int i = 0; i < length; i++) {
				printf("%c", characters[i]);
			}
		}

	private:
		bool m_newline;
		Log m_type;
	};

// ----------------------------------------

	const LogStream& operator<<(const LogStream& stream, const char* value);
	const LogStream& operator<<(const LogStream& stream, const unsigned char* value);
	const LogStream& operator<<(const LogStream& stream, const std::string& value);
	const LogStream& operator<<(const LogStream& stream, const std::string_view& value);
	const LogStream& operator<<(const LogStream& stream, char value);
	const LogStream& operator<<(const LogStream& stream, unsigned char value);
	const LogStream& operator<<(const LogStream& stream, int value);
	const LogStream& operator<<(const LogStream& stream, long int value);
	const LogStream& operator<<(const LogStream& stream, long long int value);
	const LogStream& operator<<(const LogStream& stream, unsigned int value);
	const LogStream& operator<<(const LogStream& stream, long unsigned int value);
	const LogStream& operator<<(const LogStream& stream, long long unsigned int value);
	const LogStream& operator<<(const LogStream& stream, double value);
	const LogStream& operator<<(const LogStream& stream, float value);
	const LogStream& operator<<(const LogStream& stream, const void* value);
	const LogStream& operator<<(const LogStream& stream, bool value);
	const LogStream& operator<<(const LogStream& stream, Log value);

// ----------------------------------------

	DebugLogStream dbg();
	DebugLogStream dbg(bool newline);
	DebugLogStream dbg(Log type);
	DebugLogStream dbg(Log type, bool newline);

	void dbgln(Log type, bool newline);
	void dbgln(const char* format);
	void dbgln(Log type, const char* format);
	void dbgln(Log type, bool newline, const char* format);
	void dbgln(std::string format);
	void dbgln(Log type, std::string format);
	void dbgln(Log type, bool newline, std::string format);
	void dbgln(std::string_view format);
	void dbgln(Log type, std::string_view format);
	void dbgln(Log type, bool newline, std::string_view format);

	template<typename T, typename... P>
	void dbgln(const char* format, T value, const P&... parameters)
	{
		dbgln(Log::Log, true, format, value, parameters...);
	}

	template<typename T, typename... P>
	void dbgln(Log type, const char* format, T value, const P&... parameters)
	{
		dbgln(type, true, format, value, parameters...);
	}

	// https://en.cppreference.com/w/cpp/language/parameter_pack#Example
	template<typename T, typename... P>
	void dbgln(Log type, bool newline, const char* format, T value, const P&... parameters)
	{
		std::string_view view { format };

		uint32_t i = 0;
		while(format[i] != '\0') {

			if (format[i] == '{' && format[i + 1] == '}') {
				dbg(type, false) << view.substr(0, i) << value;
				dbgln(type, newline, format + i + 2, parameters...);
				return;
			}

			i++;
		}
	}

	template<typename T, typename... P>
	void dbgln(std::string format, T value, const P&... parameters)
	{
		dbgln(format.c_str(), value, parameters...);
	}

	template<typename T, typename... P>
	void dbgln(Log type, std::string format, T value, const P&... parameters)
	{
		dbgln(type, format.c_str(), value, parameters...);
	}

	template<typename T, typename... P>
	void dbgln(Log type, bool newline, std::string format, T value, const P&... parameters)
	{
		dbgln(type, newline, format.c_str(), value, parameters...);
	}

	template<typename T, typename... P>
	void dbgln(std::string_view format, T value, const P&... parameters)
	{
		dbgln(format.data(), value, parameters...);
	}

	template<typename T, typename... P>
	void dbgln(Log type, std::string_view format, T value, const P&... parameters)
	{
		dbgln(type, format.data(), value, parameters...);
	}

	template<typename T, typename... P>
	void dbgln(Log type, bool newline, std::string_view format, T value, const P&... parameters)
	{
		dbgln(type, newline, format.data(), value, parameters...);
	}

}

#endif // LOG_H
