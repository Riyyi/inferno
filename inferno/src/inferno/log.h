#ifndef LOG_H
#define LOG_H

#include <memory>

namespace Inferno {

	class Logger;

	class Log
	{
	public:
		static void init();

// -----------------------------------------

		inline static std::shared_ptr<Logger> &getCoreLogger() { return m_coreLogger; }
		inline static std::shared_ptr<Logger> &getGameLogger() { return m_gameLogger; }

	private:
		static std::shared_ptr<Logger> m_coreLogger;
		static std::shared_ptr<Logger> m_gameLogger;
	};

	class Logger
	{
	public:
		Logger(const char* name);
		~Logger();

		void log(const char* message, const char* level = "");
		void info(const char* message);
		void warn(const char* message);
		void danger(const char* message);
		void success(const char* message);

	private:
		const char* m_name;
	};

}

#define NF_CORE_LOG(x)     Inferno::Log::getCoreLogger()->log(x)
#define NF_CORE_INFO(x)    Inferno::Log::getCoreLogger()->info(x)
#define NF_CORE_WARN(x)    Inferno::Log::getCoreLogger()->warn(x)
#define NF_CORE_DANGER(x)  Inferno::Log::getCoreLogger()->danger(x)
#define NF_CORE_SUCCESS(x) Inferno::Log::getCoreLogger()->success(x)

#define NF_LOG(x)          Inferno::Log::getGameLogger()->log(x)
#define NF_INFO(x)         Inferno::Log::getGameLogger()->info(x)
#define NF_WARN(x)         Inferno::Log::getGameLogger()->warn(x)
#define NF_DANGER(x)       Inferno::Log::getGameLogger()->danger(x)
#define NF_SUCCESS(x)      Inferno::Log::getGameLogger()->success(x)

#endif // LOG_H
