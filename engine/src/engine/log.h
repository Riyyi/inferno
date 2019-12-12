#ifndef LOG_H
#define LOG_H

#include <memory>

namespace Engine {

	class Logger;

	class Log
	{
	public:
		static void init();

// -----------------------------------------

		inline static std::shared_ptr<Logger> &getEngineLogger() { return m_engineLogger; }
		inline static std::shared_ptr<Logger> &getGameLogger() { return m_gameLogger; }

	private:
		static std::shared_ptr<Logger> m_engineLogger;
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

#define NF_CORE_LOG(x)     Engine::Log::getEngineLogger()->log(x)
#define NF_CORE_INFO(x)    Engine::Log::getEngineLogger()->info(x)
#define NF_CORE_WARN(x)    Engine::Log::getEngineLogger()->warn(x)
#define NF_CORE_DANGER(x)  Engine::Log::getEngineLogger()->danger(x)
#define NF_CORE_SUCCESS(x) Engine::Log::getEngineLogger()->success(x)

#define NF_LOG(x)            Engine::Log::getGameLogger()->log(x)
#define NF_INFO(x)           Engine::Log::getGameLogger()->info(x)
#define NF_WARN(x)           Engine::Log::getGameLogger()->warn(x)
#define NF_DANGER(x)         Engine::Log::getGameLogger()->danger(x)
#define NF_SUCCESS(x)        Engine::Log::getGameLogger()->success(x)

#endif // LOG_H
