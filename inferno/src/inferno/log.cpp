#include <cstdio> // printf

#include "inferno/log.h"

namespace Inferno {

	// Reserve memory
	std::shared_ptr<Logger> Log::m_engineLogger;
	std::shared_ptr<Logger> Log::m_gameLogger;

	void Log::init()
	{
		// Create engine Logger
		m_engineLogger = std::make_shared<Logger>("Inferno");
		// Create game Logger
		m_gameLogger = std::make_shared<Logger>("Game");
	}

	Logger::Logger(const char* name) :
		m_name(name)
	{
	}

	Logger::~Logger()
	{
	}

	void Logger::log(const char* message, const char* level)
	{
		printf("%s%s: %s\033[0m\n", level, m_name, message);
	}

	void Logger::info(const char* message)
	{
		this->log(message, "\x1B[34m");
	}

	void Logger::warn(const char* message)
	{
		this->log(message, "\x1B[33m");
	}

	void Logger::danger(const char* message)
	{
		this->log(message, "\x1B[31m");
	}

	void Logger::success(const char* message)
	{
		this->log(message, "\x1B[32m");
	}

}
