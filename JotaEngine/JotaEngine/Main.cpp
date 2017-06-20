#include <iostream>

#define GLEW_STATIC

#include <GL\glew.h>

#include <GLFW\glfw3.h>

#include "Shader.h"

const GLint WIDHT = 800;
const GLint HEIGHT = 600;

int main()
{
	glfwInit();
	// Set the version of opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLU_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow * window = glfwCreateWindow(WIDHT, HEIGHT, "JotaEngine", nullptr, nullptr);

	int screenWidht, screenHeight;
	glfwGetFramebufferSize(window, &screenWidht, &screenHeight);

	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	// Enable glew
	glewExperimental = GL_TRUE;

	// Initialize glew and make sure is succesfully
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, screenWidht, screenHeight);

	Shader shader("vertex.glsl", "fragment.glsl");

	// Triangle
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,			1.0f, 0.0f, 0.0f, // bottom left
		0.5f, -0.5f, 0.0f,			0.0f, 1.0f, 0.0f, // bottom right
		0.0f, 0.5f, 0.0f,			0.0f, 0.0f, 1.0f,// top triangle
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any event has been activated
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Swap screen buffers
		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return EXIT_SUCCESS;

}