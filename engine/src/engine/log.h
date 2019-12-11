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
		Logger(const char *name);
		~Logger();

		void log(const char *message, const char *level = "");
		void info(const char *message);
		void warn(const char *message);
		void danger(const char *message);
		void success(const char *message);

	private:
		const char *m_name;
	};

}

#define LOG_ENGINE_LOG(x)     Engine::Log::getEngineLogger()->log(x)
#define LOG_ENGINE_INFO(x)    Engine::Log::getEngineLogger()->info(x)
#define LOG_ENGINE_WARN(x)    Engine::Log::getEngineLogger()->warn(x)
#define LOG_ENGINE_DANGER(x)  Engine::Log::getEngineLogger()->danger(x)
#define LOG_ENGINE_SUCCESS(x) Engine::Log::getEngineLogger()->success(x)

#define LOG_LOG(x)            Engine::Log::getGameLogger()->log(x)
#define LOG_INFO(x)           Engine::Log::getGameLogger()->info(x)
#define LOG_WARN(x)           Engine::Log::getGameLogger()->warn(x)
#define LOG_DANGER(x)         Engine::Log::getGameLogger()->danger(x)
#define LOG_SUCCESS(x)        Engine::Log::getGameLogger()->success(x)

#endif // LOG_H
