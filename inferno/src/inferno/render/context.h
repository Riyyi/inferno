#ifndef CONTEXT_H
#define CONTEXT_H

struct GLFWwindow;

namespace Inferno {

	class Context {
	public:
		Context(GLFWwindow* window);
		// virtual ~Context();

		void initialize();
		void update();
		// void render();
		void destroy();

	private:
		GLFWwindow* m_window;
	};

}


#endif // CONTEXT_H
