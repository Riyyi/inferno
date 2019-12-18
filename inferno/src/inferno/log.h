#ifndef LOG_H
#define LOG_H

#include <cstdio>  // printf, sprintf
#include <cstring> // strlen
#include <memory>  // std::shared_ptr
#include <string>  // std::string

namespace Inferno {

	class Logger;

	class Log {
	public:
		static void initialize();

// -----------------------------------------

		inline static std::shared_ptr<Logger> &getCoreLogger() { return m_coreLogger; }
		inline static std::shared_ptr<Logger> &getGameLogger() { return m_gameLogger; }

	private:
		static std::shared_ptr<Logger> m_coreLogger;
		static std::shared_ptr<Logger> m_gameLogger;
	};

	class Logger {
	public:
		Logger(const char* name);
		~Logger();

		template<typename ...A>
		void print(const char* color, const char* format, A... arguments)
		{
			char buffer[10 + strlen(color) + strlen(m_name) + strlen(format)];
			sprintf(buffer, "%s%s: %s\033[0m\n", color, m_name, format);
			printf(buffer, arguments...);
		}

		template<typename ...A>
		void print(const char* color, std::string format, A... arguments)
		{
			this->print(color, format.c_str(), arguments...);
		}

		template<typename T, typename ...A>
		void log(T format, A... arguments)
		{
			this->print("", format, arguments...);
		}

		template<typename T, typename ...A>
		void info(T format, A... arguments)
		{
			this->print("\x1B[34m", format, arguments...);
		}

		template<typename T, typename ...A>
		void warn(T format, A... arguments)
		{
			this->print("\x1B[33m", format, arguments...);
		}

		template<typename T, typename ...A>
		void danger(T format, A... arguments)
		{
			this->print("\x1B[31m", format, arguments...);
		}

		template<typename T, typename ...A>
		void success(T format, A... arguments)
		{
			this->print("\x1B[32m", format, arguments...);
		}

	private:
		const char* m_name;
	};

}

#define NF_CORE_LOG(...)     Inferno::Log::getCoreLogger()->log(__VA_ARGS__)
#define NF_CORE_INFO(...)    Inferno::Log::getCoreLogger()->info(__VA_ARGS__)
#define NF_CORE_WARN(...)    Inferno::Log::getCoreLogger()->warn(__VA_ARGS__)
#define NF_CORE_DANGER(...)  Inferno::Log::getCoreLogger()->danger(__VA_ARGS__)
#define NF_CORE_SUCCESS(...) Inferno::Log::getCoreLogger()->success(__VA_ARGS__)

#define NF_LOG(...)          Inferno::Log::getGameLogger()->log(__VA_ARGS__)
#define NF_INFO(...)         Inferno::Log::getGameLogger()->info(__VA_ARGS__)
#define NF_WARN(...)         Inferno::Log::getGameLogger()->warn(__VA_ARGS__)
#define NF_DANGER(...)       Inferno::Log::getGameLogger()->danger(__VA_ARGS__)
#define NF_SUCCESS(...)      Inferno::Log::getGameLogger()->success(__VA_ARGS__)

#endif // LOG_H
