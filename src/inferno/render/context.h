#pragma once

struct GLFWwindow;

namespace Inferno {

	class Context {
	public:
		Context(GLFWwindow* window);

		void initialize();
		void destroy();
		void render();

		void setCurrent();

	private:
		GLFWwindow* m_window { nullptr };
	};

} // namespace Inferno
