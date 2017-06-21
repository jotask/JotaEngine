#include <iostream>

#define GLEW_STATIC

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include <GL\glew.h>

#include <GLFW\glfw3.h>

#include "Shader.h"
#include "Camera.h"

const GLint WIDHT = 800;
const GLint HEIGHT = 600;

int screenWidht, screenHeight;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mode );
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

glm::vec3 randomColor();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDHT / 2.0f;
GLfloat lastY = WIDHT / 2.0f;

bool keys[1024];
bool firstMouse = true;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

const bool BLOCKMOUSE = false;

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

	glfwGetFramebufferSize(window, &screenWidht, &screenHeight);

	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	// setting callbacks
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// disable mouse on screen
	if(BLOCKMOUSE)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Enable glew
	glewExperimental = GL_TRUE;

	// Initialize glew and make sure is succesfully
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, screenWidht, screenHeight);

	glEnable(GL_DEPTH_TEST);

	Shader lightShader("light_vertex.glsl", "light_fragment.glsl");
	Shader lampShader("lamp_vertex.glsl", "lamp_fragment.glsl");
	
    // use with Perspective Projection
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	glm::vec3 pointLightPosition[] =
	{
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.0f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	GLuint VBO, boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // unbind vao

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glm::mat4 projection = glm::perspective(camera.getZoom(), (GLfloat)(screenWidht / screenHeight), 0.1f, 1000.0f);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check if any event has been activated
		glfwPollEvents();

		lightPos.x = glm::cos(currentFrame) * 2.0f;
		lightPos.y = glm::sin(currentFrame) * 2.0f;
		lightPos.z = glm::sin(currentFrame) * 2.0f;

		DoMovement();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// -------------------------------------------------------------

		lightShader.use();
		GLint viewPosLocation = glGetUniformLocation(lightShader.program, "viewPos");
		glUniform3f(viewPosLocation, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

		// Direction light
		glUniform3f(glGetUniformLocation(lightShader.program, "directionLight.direction"), -2.0f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightShader.program, "directionLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightShader.program, "directionLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightShader.program, "directionLight.specular"), 0.5f, 0.5f, 0.5f);

		// Point light 1
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[0].position"), pointLightPosition[0].x, pointLightPosition[0].y, pointLightPosition[0].z);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[0].quadratic"), 0.032f);
		// Point light 2
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[1].position"), pointLightPosition[1].x, pointLightPosition[1].y, pointLightPosition[1].z);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[1].quadratic"), 0.032f);
		// Point light 3
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[2].position"), pointLightPosition[2].x, pointLightPosition[2].y, pointLightPosition[2].z);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[2].quadratic"), 0.032f);
		// Point light 4
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[3].position"), pointLightPosition[3].x, pointLightPosition[3].y, pointLightPosition[3].z);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightShader.program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightShader.program, "pointLights[3].quadratic"), 0.032f);

		// Spot light
		glUniform3f(glGetUniformLocation(lightShader.program, "spotLight.position"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		glUniform3f(glGetUniformLocation(lightShader.program, "spotLight.direction"), camera.getFront().x, camera.getFront().y, camera.getFront().z);
		glUniform3f(glGetUniformLocation(lightShader.program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightShader.program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightShader.program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightShader.program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightShader.program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightShader.program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightShader.program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightShader.program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.5f)));

		glUniform3f(glGetUniformLocation(lightShader.program, "material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(lightShader.program, "material.diffuse"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(lightShader.program, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(lightShader.program, "material.shininess"), 32.0f);

		glm::mat4 view;
		view = camera.getViewMatrix();

		GLint modelLoc = glGetUniformLocation(lightShader.program, "model");
		GLint viewLoc = glGetUniformLocation(lightShader.program, "view");
		GLint projLoc = glGetUniformLocation(lightShader.program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model;
		glBindVertexArray(boxVAO);
		for (GLuint i = 0; i < 10; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);

			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		// ------------------------------------------


		lampShader.use();

		modelLoc = glGetUniformLocation(lampShader.program, "model");
		viewLoc = glGetUniformLocation(lampShader.program, "view");
		projLoc = glGetUniformLocation(lampShader.program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 1, 36);
		glBindVertexArray(0);

		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPosition[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES , 0, 36);
		}
		glBindVertexArray(0);

		// Swap screen buffers
		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &boxVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return EXIT_SUCCESS;

}

void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.processKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.processKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.processKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.processKeyboard(RIGHT, deltaTime);
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (GLFW_PRESS == action)
		{
			keys[key] = true;
		}
		else if (GLFW_RELEASE == action)
		{
			keys[key] = false;
		}
	}
}


void  MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.processMouseMovement(xOffset, yOffset);

}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processMouseScroll(yOffset);
}


glm::vec3 randomColor()
{
	glm::vec3 color;
	color.r = sin(glfwGetTime() * 2.0f);
	color.g = sin(glfwGetTime() * 0.7f);
	color.b = sin(glfwGetTime() * 1.3f);
	return color;
}