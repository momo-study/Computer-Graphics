#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <learnopengl/shader_m.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *glsl_version = "#version 330";

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// setting
	bool translate = false,
		rotate = false,
		scale = false,
		enable = true,
		bonus = false;


	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("shader.vs", "shader.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-2.0f, -2.0f, 2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, -2.0f, 2.0f, 1.0f, 0.0f, 0.0f,
		2.0f,  2.0f, 2.0f, 1.0f, 0.0f, 0.0f,
		2.0f,  2.0f, 2.0f, 1.0f, 0.0f, 0.0f,
		-2.0f,  2.0f, 2.0f, 1.0f, 0.0f, 0.0f,
		-2.0f, -2.0f, 2.0f, 1.0f, 0.0f, 0.0f,

		-2.0f, -2.0f,  6.0f, 0.0f, 1.0f, 0.0f,
		2.0f, -2.0f,  6.0f, 0.0f, 1.0f, 0.0f,
		2.0f,  2.0f,  6.0f, 0.0f, 1.0f, 0.0f,
		2.0f,  2.0f,  6.0f, 0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f,  6.0f, 0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f,  6.0f, 0.0f, 1.0f, 0.0f,

		-2.0f,  2.0f,  6.0f, 0.0f, 0.0f, 1.0f,
		-2.0f,  2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f,  6.0f, 0.0f, 0.0f, 1.0f,
		-2.0f,  2.0f,  6.0f, 0.0f, 0.0f, 1.0f,

		2.0f,  2.0f,  6.0f, 0.0f, 1.0f, 1.0f,
		2.0f,  2.0f, 2.0f, 0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, 2.0f, 0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, 2.0f, 0.0f, 1.0f, 1.0f,
		2.0f, -2.0f,  6.0f, 0.0f, 1.0f, 1.0f,
		2.0f,  2.0f,  6.0f, 0.0f, 1.0f, 1.0f,

		-2.0f, -2.0f, 2.0f, 1.0f, 1.0f, 0.0f,
		2.0f, -2.0f, 2.0f, 1.0f, 1.0f, 0.0f,
		2.0f, -2.0f,  6.0f, 1.0f, 1.0f, 0.0f,
		2.0f, -2.0f,  6.0f, 1.0f, 1.0f, 0.0f,
		-2.0f, -2.0f,  6.0f, 1.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, 2.0f, 1.0f, 1.0f, 0.0f,

		-2.0f,  2.0f, 2.0f, 1.0f, 0.0f, 1.0f,
		2.0f,  2.0f, 2.0f, 1.0f, 0.0f, 1.0f,
		2.0f,  2.0f,  6.0f, 1.0f, 0.0f, 1.0f,
		2.0f,  2.0f,  6.0f, 1.0f, 0.0f, 1.0f,
		-2.0f,  2.0f,  6.0f, 1.0f, 0.0f, 1.0f,
		-2.0f,  2.0f, 2.0f, 1.0f, 0.0f, 1.0f
	};
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// gui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Transformation\n");
		ImGui::SetWindowSize(ImVec2(300, 220));

		ImGui::Checkbox("translation", &translate);
		ImGui::Checkbox("rotation", &rotate);
		ImGui::Checkbox("scaling", &scale);
		ImGui::Checkbox("enable depth testing", &enable);
		ImGui::Checkbox("bonus", &bonus);

		ImGui::End();

		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// activate shader
		ourShader.use();

		// create transformations
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		
		if (translate) {
			model = glm::translate(model, sin((float)glfwGetTime()) * glm::vec3(5.0f, 0.0f, 0.0f));
		}
		if (rotate) {
			model = glm::rotate(model, (float)glfwGetTime() * 20, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		if (scale) {
			model = glm::scale(model, 2 * abs(sin((float)glfwGetTime())) * glm::vec3(0.5f, 0.5f, 0.5f));
		}
		if (enable) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
		if (bonus) {
			float radius = 15.0f;
			view = glm::lookAt(glm::vec3(sin(glfwGetTime()) * radius, 0.0f, cos(glfwGetTime()) * radius), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else {
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
		}

		ourShader.setMat4("model", model);
		projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);

		// render box
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}