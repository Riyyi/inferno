#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory> // std::unique_ptr

namespace Inferno {

	template<typename T>
	class Singleton {
	public:
		static inline T& the() {
			static const std::unique_ptr<T> instance { new T { s {} } };
			return *instance;
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
	};

} // namespace Inferno

#endif // SINGLETON_H
