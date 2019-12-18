#include "inferno/log.h"

namespace Inferno {

	// Reserve memory
	std::shared_ptr<Logger> Log::m_coreLogger;
	std::shared_ptr<Logger> Log::m_gameLogger;

	void Log::initialize()
	{
		// Create engine Logger
		m_coreLogger = std::make_shared<Logger>("Inferno");
		// Create game Logger
		m_gameLogger = std::make_shared<Logger>("Game");

		NF_CORE_INFO("Log initialized");
	}

	Logger::Logger(const char* name) :
		m_name(name)
	{
	}

	Logger::~Logger()
	{
	}

}
