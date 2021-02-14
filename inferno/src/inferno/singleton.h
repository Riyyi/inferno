#ifndef SINGLETON_H
#define SINGLETON_H

#include "inferno/assert.h"

namespace Inferno {

	template<typename T>
	class Singleton {
		// Application is allowed to access its Singleton instance for early setting
		friend class Application;

	public:
		static inline void initialize()
		{
			ASSERT(!s_instance, "Singleton already exists!");
			s_instance = new T { s {} };
		}

		static inline void destroy()
		{
			ASSERT(s_instance, "Singleton does not exist!");
			delete s_instance;
			s_instance = nullptr;
		}

		static inline T& the()
		{
			ASSERT(s_instance, "Singleton does not exist!");
			return *s_instance;
		}

		// Remove copy constructor and copy assignment operator
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator=(Singleton&&) = delete;

	protected:
		Singleton() {}

		// Constructor token
		struct s {};

	private:
		static T* s_instance;
	};

	template<typename T>
	T* Singleton<T>::s_instance = nullptr;

} // namespace Inferno


#endif // SINGLETON_H
