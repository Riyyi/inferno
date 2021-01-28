#include "inferno/assert.h"
#include "inferno/io/log.h"

#include <cstddef>     // size_t
#include <cstdio>      // fwrite, snprintf
#include <cstdlib>     // malloc, free
#include <cstring>     // memcpy, strlen
#include <string>      // std::string
#include <string_view> // std::string_view

namespace Inferno {

	BufferedLogStream::~BufferedLogStream()
	{
		// Free buffer memory
		if (m_capacity > sizeof(m_buffer.stack)) {
			free(m_buffer.heap);
		}
	}

	void BufferedLogStream::grow(size_t bytes) const
	{
		// Bitwise & ~ example, we use 127 as binary starts at 0
		// 0b001111111 127 ~
		// 0b100000100 260 &
		// -----------
		// 0b110000000 384
		// 0b100000100 260 &
		// -----------
		// 0b100000000 256

		// Buffer is is chunks of 128 bytes
		size_t newCapacity = (m_count + bytes + BUFFER_SIZE - 1) & ~(BUFFER_SIZE - 1);

		unsigned char* newBuffer = static_cast<unsigned char*>(malloc(newCapacity));

		// Copy the non-heap data into the new buffer
		if (m_capacity <= sizeof(m_buffer.stack)) {
			memcpy(newBuffer, m_buffer.stack, m_count);
		}
		// Copy old heap-buffer data into the new buffer, free old heap-buffer
		else if (m_buffer.heap) {
			memcpy(newBuffer, m_buffer.heap, m_count);
			free(m_buffer.heap);
		}

		m_buffer.heap = newBuffer;
		m_capacity = newCapacity;
	}

// -----------------------------------------

	DebugLogStream::~DebugLogStream()
	{
		if (empty()) {
			return;
		}

		if (m_type != Log::None) {
			const char* clear = "\033[0m";
			write(clear, strlen(clear));
		}

		if (m_newline) {
			char newline = '\n';
			write(&newline, 1);
		}

		fwrite(buffer(), 1, count(), stdout);
	}

	void DebugLogStream::color() const
	{
		const char* color = "";

		if (m_type == Log::Info) {
			color = "\x1B[34m";
		}
		else if (m_type == Log::Warn) {
			color = "\x1B[33m";
		}
		else if (m_type == Log::Danger) {
			color = "\x1B[31m";
		}
		else if (m_type == Log::Success) {
			color = "\x1B[32m";
		}
		else if (m_type == Log::Comment) {
			color = "\x1B[37m";
		}

		write(color, strlen(color));
	}

// -----------------------------------------

	const LogStream& operator<<(const LogStream& stream, const char* value)
	{
		if (value == nullptr) {
			stream.write("(null)", 6);
			return stream;
		}

		stream.write(value, strlen(value));
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, const unsigned char* value)
	{
		if (value == nullptr) {
			stream.write("(null)", 6);
			return stream;
		}

		stream.write(value, strlen((const char*)value));
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, const std::string& value)
	{
		stream.write(value.c_str(), value.length());
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, const std::string_view& value)
	{
		stream.write(value.data(), value.length());
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, char value)
	{
		stream.write(&value, 1);
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, unsigned char value)
	{
		stream.write(&value, 1);
		return stream;
	}

	const LogStream& operator<<(const LogStream& stream, int value)
	{
		// return stream << std::to_string(value);

		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%d", value);
		return stream << buffer;
	}

	const LogStream& operator<<(const LogStream& stream, long int value)
	{
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%ld", value);
		return stream << buffer;
	}

	const LogStream& operator<<(const LogStream& stream, long long int value)
	{
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%lld", value);
		return stream << buffer;
	}

	const LogStream& operator<<(const LogStream& stream, unsigned int value)
	{
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%u", value);
		return stream << buffer;
	}

	const LogStream& operator<<(const LogStream& stream, long unsigned int value)
	{
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%lu", value);
		return stream << buffer;
	}

	const LogStream& operator<<(const LogStream& stream, long long unsigned int value)
	{
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%llu", value);
		return stream << buffer;
	}

	const LogStream& operator<<(const LogStream& stream, double value)
	{
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%.4f", value);
		return stream << buffer;
	}

	const LogStream& operator<<(const LogStream& stream, float value)
	{
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%.4f", value);
		return stream << buffer;
	}

	const LogStream& operator<<(const LogStream& stream, const void* value)
	{
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%p", value);
		return stream << buffer;
	}

	const LogStream& operator<<(const LogStream& stream, bool value)
	{
		return stream << (value ? "true": "false");
	}

	const LogStream& operator<<(const LogStream& stream, Log value)
	{
		switch (value) {
		case Log::None:
			return stream << "Log";
		case Log::Info:
			return stream << "Info";
		case Log::Warn:
			return stream << "Warn";
		case Log::Danger:
			return stream << "Danger";
		case Log::Success:
			return stream << "Success";
		case Log::Comment:
			return stream << "Comment";
		default:
			ASSERT_NOT_REACHED();
			return stream;
		}
	}

// -----------------------------------------

	DebugLogStream dbg()
	{
		return {};
	}

	DebugLogStream dbg(bool newline)
	{
		return DebugLogStream(newline);
	}

	DebugLogStream dbg(Log type)
	{
		return DebugLogStream(type);
	}

	DebugLogStream dbg(Log type, bool newline)
	{
		return DebugLogStream(type, newline);
	}

// -----------------------------------------

	void dbgln(Log type, bool newline) {
		(void)type;
		dbg(newline);
	}

	void dbgln(const char* format)
	{
		dbg() << format;
	}

	void dbgln(Log type, const char* format)
	{
		dbg(type) << format;
	}

	void dbgln(Log type, bool newline, const char* format)
	{
		dbg(type, newline) << format;
	}

	void dbgln(std::string format)
	{
		dbg() << format;
	}

	void dbgln(Log type, std::string format)
	{
		dbg(type) << format;
	}

	void dbgln(Log type, bool newline, std::string format)
	{
		dbg(type, newline) << format;
	}

	void dbgln(std::string_view format)
	{
		dbg() << format;
	}

	void dbgln(Log type, std::string_view format)
	{
		dbg(type) << format;
	}

	void dbgln(Log type, bool newline, std::string_view format)
	{
		dbg(type, newline) << format;
	}

}
