#ifndef LOG_H
#define LOG_H

#define BUFFER_SIZE 128

#include <cstddef>     // size_t
#include <cstring>     // memcpy
#include <string>      // std::string
#include <string_view> // std::string_view
#include <utility>     // std::forward

namespace Inferno {

	enum class Log {
		None,
		Info,
		Warn,
		Danger,
		Success,
		Comment,
	};

// -----------------------------------------

	class LogStream {
	public:
		LogStream() {}
		virtual ~LogStream() {}

		virtual void write(const char* characters, int length) const = 0;
		virtual void write(const unsigned char* characters, int length) const = 0;
	};

// -----------------------------------------

	class BufferedLogStream : public LogStream {
	public:
		BufferedLogStream() {}
		virtual ~BufferedLogStream();

		inline virtual void write(const char* characters, int length) const override
		{
			write(reinterpret_cast<const unsigned char*>(characters), length);
		}

		inline virtual void write(const unsigned char* characters, int length) const override
		{
			size_t newSize = m_count + length;

			if (newSize > m_capacity) {
				grow(length);
			}

			// Append to buffer
			memcpy(buffer() + m_count, characters, length);

			m_count = newSize;
		}

	protected:
		inline unsigned char* buffer() const
		{
			if (m_capacity <= sizeof(m_buffer.stack)) {
				return m_buffer.stack;
			}

			return m_buffer.heap;
		}

		inline bool empty() const { return m_count == 0; }
		inline size_t count() const { return m_count; }

	private:
		void grow(size_t bytes) const;

		mutable size_t m_count { 0 };
		mutable size_t m_capacity { BUFFER_SIZE };
		union {
			mutable unsigned char* heap { nullptr };
			mutable unsigned char stack[BUFFER_SIZE];
		} m_buffer;
	};

// -----------------------------------------

	class DebugLogStream final : public BufferedLogStream {
	public:
		DebugLogStream()
			: m_newline(true), m_type(Log::None) {}
		DebugLogStream(bool newline)
			: m_newline(newline), m_type(Log::None) {}
		DebugLogStream(Log type)
			: m_newline(true), m_type(type) { color(); }
		DebugLogStream(Log type, bool newline)
			: m_newline(newline), m_type(type) { color(); }
		virtual ~DebugLogStream() override;

		void color() const;

	private:
		bool m_newline;
		Log m_type;
	};

// -----------------------------------------

	class StringLogStream final : public BufferedLogStream {
	public:
		StringLogStream(std::string* fill)
			: m_fill(fill) {}
		virtual ~StringLogStream() override;

	private:
		std::string* m_fill;
	};

// -----------------------------------------

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

// -----------------------------------------

	DebugLogStream dbg();
	DebugLogStream info();
	DebugLogStream warn();
	DebugLogStream danger();
	DebugLogStream success();
	DebugLogStream comment();

	DebugLogStream dbg(bool newline);
	DebugLogStream info(bool newline);
	DebugLogStream warn(bool newline);
	DebugLogStream danger(bool newline);
	DebugLogStream success(bool newline);
	DebugLogStream comment(bool newline);

// -----------------------------------------

	template<typename... P> void dbgln(const char* format, P&&... parameters) { return dbgln(Log::None, true, format, std::forward<P>(parameters)...); }
	template<typename... P> void infoln(const char* format, P&&... parameters) { return dbgln(Log::Info, true, format, std::forward<P>(parameters)...); }
	template<typename... P> void warnln(const char* format, P&&... parameters) { return dbgln(Log::Warn, true, format, std::forward<P>(parameters)...); }
	template<typename... P> void dangerln(const char* format, P&&... parameters) { return dbgln(Log::Danger, true, format, std::forward<P>(parameters)...); }
	template<typename... P> void successln(const char* format, P&&... parameters) { return dbgln(Log::Success, true, format, std::forward<P>(parameters)...); }
	template<typename... P> void commentln(const char* format, P&&... parameters) { return dbgln(Log::Comment, true, format, std::forward<P>(parameters)...); }

	template<typename... P> void dbgln(const std::string& format, P&&... parameters) { return dbgln(Log::None, true, format.c_str(), std::forward<P>(parameters)...); }
	template<typename... P> void infoln(const std::string& format, P&&... parameters) { return dbgln(Log::Info, true, format.c_str(), std::forward<P>(parameters)...); }
	template<typename... P> void warnln(const std::string& format, P&&... parameters) { return dbgln(Log::Warn, true, format.c_str(), std::forward<P>(parameters)...); }
	template<typename... P> void dangerln(const std::string& format, P&&... parameters) { return dbgln(Log::Danger, true, format.c_str(), std::forward<P>(parameters)...); }
	template<typename... P> void successln(const std::string& format, P&&... parameters) { return dbgln(Log::Success, true, format.c_str(), std::forward<P>(parameters)...); }
	template<typename... P> void commentln(const std::string& format, P&&... parameters) { return dbgln(Log::Comment, true, format.c_str(), std::forward<P>(parameters)...); }

	template<typename... P> void dbgln(const std::string_view& format, P&&... parameters) { return dbgln(Log::None, true, format.data(), std::forward<P>(parameters)...); }
	template<typename... P> void infoln(const std::string_view& format, P&&... parameters) { return dbgln(Log::Info, true, format.data(), std::forward<P>(parameters)...); }
	template<typename... P> void warnln(const std::string_view& format, P&&... parameters) { return dbgln(Log::Warn, true, format.data(), std::forward<P>(parameters)...); }
	template<typename... P> void dangerln(const std::string_view& format, P&&... parameters) { return dbgln(Log::Danger, true, format.data(), std::forward<P>(parameters)...); }
	template<typename... P> void successln(const std::string_view& format, P&&... parameters) { return dbgln(Log::Success, true, format.data(), std::forward<P>(parameters)...); }
	template<typename... P> void commentln(const std::string_view& format, P&&... parameters) { return dbgln(Log::Comment, true, format.data(), std::forward<P>(parameters)...); }

	template<typename... P> void dbgln(bool newline, const char* format, P&&... parameters) { return dbgln(Log::None, newline, format, std::forward<P>(parameters)...); }
	template<typename... P> void infoln(bool newline, const char* format, P&&... parameters) { return dbgln(Log::Info, newline, format, std::forward<P>(parameters)...); }
	template<typename... P> void warnln(bool newline, const char* format, P&&... parameters) { return dbgln(Log::Warn, newline, format, std::forward<P>(parameters)...); }
	template<typename... P> void dangerln(bool newline, const char* format, P&&... parameters) { return dbgln(Log::Danger, newline, format, std::forward<P>(parameters)...); }
	template<typename... P> void successln(bool newline, const char* format, P&&... parameters) { return dbgln(Log::Success, newline, format, std::forward<P>(parameters)...); }
	template<typename... P> void commentln(bool newline, const char* format, P&&... parameters) { return dbgln(Log::Comment, newline, format, std::forward<P>(parameters)...); }

	template<typename... P> void dbgln(bool newline, const std::string& format, P&&... parameters) { return dbgln(Log::None, newline, format.c_str(), std::forward<P>(parameters)...); }
	template<typename... P> void infoln(bool newline, const std::string& format, P&&... parameters) { return dbgln(Log::Info, newline, format.c_str(), std::forward<P>(parameters)...); }
	template<typename... P> void warnln(bool newline, const std::string& format, P&&... parameters) { return dbgln(Log::Warn, newline, format.c_str(), std::forward<P>(parameters)...); }
	template<typename... P> void dangerln(bool newline, const std::string& format, P&&... parameters) { return dbgln(Log::Danger, newline, format.c_str(), std::forward<P>(parameters)...); }
	template<typename... P> void successln(bool newline, const std::string& format, P&&... parameters) { return dbgln(Log::Success, newline, format.c_str(), std::forward<P>(parameters)...); }
	template<typename... P> void commentln(bool newline, const std::string& format, P&&... parameters) { return dbgln(Log::Comment, newline, format.c_str(), std::forward<P>(parameters)...); }

	template<typename... P> void dbgln(bool newline, const std::string_view& format, P&&... parameters) { return dbgln(Log::None, newline, format.data(), std::forward<P>(parameters)...); }
	template<typename... P> void infoln(bool newline, const std::string_view& format, P&&... parameters) { return dbgln(Log::Info, newline, format.data(), std::forward<P>(parameters)...); }
	template<typename... P> void warnln(bool newline, const std::string_view& format, P&&... parameters) { return dbgln(Log::Warn, newline, format.data(), std::forward<P>(parameters)...); }
	template<typename... P> void dangerln(bool newline, const std::string_view& format, P&&... parameters) { return dbgln(Log::Danger, newline, format.data(), std::forward<P>(parameters)...); }
	template<typename... P> void successln(bool newline, const std::string_view& format, P&&... parameters) { return dbgln(Log::Success, newline, format.data(), std::forward<P>(parameters)...); }
	template<typename... P> void commentln(bool newline, const std::string_view& format, P&&... parameters) { return dbgln(Log::Comment, newline, format.data(), std::forward<P>(parameters)...); }

// -----------------------------------------

	void dbgln(Log type, bool newline);
	void dbgln(Log type, bool newline, const char* format);

	// https://en.cppreference.com/w/cpp/language/parameter_pack#Example
	template<typename T, typename... P>
	void dbgln(Log type, bool newline, const char* format, T value, P&&... parameters)
	{
		std::string_view view { format };

		uint32_t i = 0;
		while(format[i] != '\0') {

			if (format[i] == '{' && format[i + 1] == '}') {
				DebugLogStream(type, false) << view.substr(0, i) << value;
				dbgln(type, newline, format + i + 2, parameters...);
				return;
			}

			i++;
		}
	}

// -----------------------------------------

	StringLogStream str(std::string* fill);

}

#endif // LOG_H
