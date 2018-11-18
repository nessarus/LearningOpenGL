#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

/* Lecture: Creating a Texture Test in OpenGL */

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//----------------------------------------------------------------------------------
	// Previous:
	//----------------------------------------------------------------------------------
	// OpenGL Core profile and version (demonstration)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//----------------------------------------------------------------------------------
	// Previous: 
	//----------------------------------------------------------------------------------
	// slow down the frame rate so we can see the changes
	glfwSwapInterval(2);

	// Move glewInit() after creating OpenGL context
	GLenum err = glewInit();
	if (GLEW_OK != err)
		std::cout << "Error!" << std::endl;

	// Check OpenGL version
	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		// Enable blending of alpha (layers of transparency in textures)
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		// Setting up Renderer
		Renderer renderer;

		// Setting up UI by setting up ImGui context
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);

		// Setup UI style
		ImGui::StyleColorsDark();

		// Setting up test framework
		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		// test for clearing colors
		testMenu->RegisterTest<test::TestClearColor>("Clear Color");

		//----------------------------------------------------------------------------------
		// Lesson: Creating new test for textures (see TextTexture2D.cpp, TestTexture2D.h)
		//----------------------------------------------------------------------------------
		// test for testing textures
		testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			//----------------------------------------------------------------------------------
			// Previous
			//----------------------------------------------------------------------------------
			// set background color back to black
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

			// glClear(GL_COLOR_BUFFER_BIT); 
			renderer.Clear();

			// Sets up new ImGui frame (setup before any ImGui code for this frame)
			ImGui_ImplGlfwGL3_NewFrame();

			// do we have a test?
			if (currentTest)
			{
				// setup the test
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Test");

				// Still selecting the test in the test menu or returned to test menu
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					// delete the testing instance
					delete currentTest;
					currentTest = testMenu;
				}

				// draw test UI
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			// Draw ImGui into frame buffer
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			//----------------------------------------------------------------------------------
			//----------------------------------------------------------------------------------

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		//----------------------------------------------------------------------------------
		// Previous:
		//----------------------------------------------------------------------------------
		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}
	// glfwTerminate deletes OpenGl context so need scope to destroy shader before it.

	// Cleaning up ImGui
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------

	glfwTerminate();
	return 0;
}