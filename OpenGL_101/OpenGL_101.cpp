#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if _MSC_VER
#define DLL_EXPORT _declspec(dllexport)
#else
#define DLL_EXPORT
#endif

extern "C" {
	DLL_EXPORT uint32_t NvOptimusEnablement = 0x00000001;
	DLL_EXPORT int AmdPowerXpressRequestHighPermance = 1;
}

struct Application {
	int m_width;
	int m_height;

	inline void setSize(int width, int height) {
		m_width = width;
		m_height = height;
	}

	bool initialize() {
		return true;
	}

	void denitialize() {

	}

	void render() {
		glEnable(GL_SCISSOR_TEST);

		// En bas à gauche
		glViewport(0, 0, m_width / 2, m_height / 2);
		glScissor(0, 0, m_width / 2, m_height / 2);
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*glBegin(GL_TRIANGLES);
		glVertex2d(-0.8f, -0.8f);
		glVertex2d(0, 0.8f);
		glVertex2d(0.8f, -0.8f);
		glEnd();*/

		static const float triangle[] = {
			-0.8f, 0.8f,
			0, -0.8f,
			0.8f, 0.8f
		};

		static const float color[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
		};

		/*
		Le premier param est un "canal" (OpenGL utilise le terme "location")
		si le contenu des données est un tableau, il faut dire à OpenGL avec glEnableVertexAttribArray()
		*/
		static constexpr uint32_t POSITION = 0;
		static constexpr uint32_t COLOR = 3;

		glEnableVertexAttribArray(POSITION);

		// Décrit un sommet
		// stride (avant dernier param) est la distance en octets entre 2 sommets
		// offset (dernier param) est l'adresse 
		glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), &triangle[0]);

		glEnableVertexAttribArray(COLOR);
		glVertexAttribPointer(COLOR, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), color);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// En bas à droite

		glViewport(m_width / 2, 0, m_width / 2, m_height / 2);
		glScissor(m_width / 2, 0, m_width / 2, m_height / 2);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2d(-0.8f, -0.8f);
		glVertex2d(0, 0.8f);
		glVertex2d(0.8f, -0.8f);
		glEnd();

		// En haut à droite
		glViewport(m_width / 2, m_height / 2, m_width / 2, m_height / 2);
		glScissor(m_width / 2, m_height / 2, m_width / 2, m_height / 2);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2d(-0.8f, 0.8f);
		glVertex2d(0, -0.8f);
		glVertex2d(0.8f, 0.8f);
		glEnd();

		// En haut à gauche
		glViewport(0, m_height / 2, m_width / 2, m_height / 2);
		glScissor(0, m_height / 2, m_width / 2, m_height / 2);
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2d(-0.8f, 0.8f);
		glVertex2d(0, -0.8f);
		glVertex2d(0.8f, 0.8f);
		glEnd();
	}
};



int main(void)
{
	GLFWwindow* window;
	Application application;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// A faire après glfwMakeContextCurrent
	GLenum error = glewInit();

	// A partir de maintenant on peut utiliser des commandes openGL
	application.initialize();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// gere le dimensionnement de la fenêtre
		// alternativement glfwSetWindowSizeCallback
		int width, height;

		glfwGetWindowSize(window, &width, &height);
		application.setSize(width, height);
		application.render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	application.denitialize();

	glfwTerminate();
	return 0;
}