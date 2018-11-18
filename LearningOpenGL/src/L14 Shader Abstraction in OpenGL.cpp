#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

/* Lecture: Shader Abstraction in OpenGL */

//----------------------------------------------------------------------------------
// Lesson: Abstracting Shader to Shader Class (Shader.cpp)
//----------------------------------------------------------------------------------
// Copy includes into Shader Class
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

//// A struct assisting ShaderProgramSource to return two string in one function.
//struct ShaderProgramSource
//{
//	std::string VertexSource;
//	std::string FragmentSource;
//};
//
////Parses the file and delivers back vertex and frament strings.
//static ShaderProgramSource ParseShader(const std::string& filepath)
//{
//	//ifstream input stream class to operate on files
//	std::ifstream stream(filepath);
//
//	//our own terminology to seperate the vertex and fragment shader code
//	enum class ShaderType
//	{
//		NONE = -1, VERTEX = 0, FRAGMENT = 1
//	};
//
//	//devide the file into ss[0], ss[1]
//	std::string line;
//	std::stringstream ss[2];
//	ShaderType type = ShaderType::NONE;
//	while (getline(stream, line))
//	{
//		if (line.find("#shader") != std::string::npos)
//		{
//			if (line.find("vertex") != std::string::npos)
//				type = ShaderType::VERTEX;
//			else if (line.find("fragment") != std::string::npos)
//				type = ShaderType::FRAGMENT;
//		}
//		else
//		{
//			ss[(int)type] << line << '\n';
//		}
//	}
//	return { ss[0].str() , ss[1].str() };
//}
//
//// Compiling a shader
//static unsigned int CompileShader(unsigned int type, const std::string& source)
//{
//	// Create a shader object. Type specifies GPU processor optimisation.
//	GLCall(unsigned int id = glCreateShader(type));
//
//	// creates the raw pointer of the source (shader) file
//	const char* src = source.c_str();
//
//	// Replaces the source code in a shader object
//	GLCall(glShaderSource(id, 1, &src, nullptr));
//
//	// Compiles shader object
//	GLCall(glCompileShader(id));
//
//	// Error handling
//	// Returns a parameter from a shader object 
//	int result;
//	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
//
//	if (result == GL_FALSE)
//	{
//		int length;
//		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
//
//		// alloca allocates memory that is automatically freed (on the stack i.e. freed after scope)
//		char* message = (char*)alloca(length * sizeof(char));
//
//		// Returns the information log for a shader object
//		GLCall(glGetShaderInfoLog(id, length, &length, message));
//
//		std::cout << "Failed to compile " <<
//			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
//		std::cout << message << std::endl;
//		GLCall(glDeleteShader(id));
//		return 0;
//	}
//	return id;
//}
//

// The shader programs are written as strings are fed into createShader and outputs the program.
//static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
//{
//	// creates a program object for shaders. 
//	unsigned int program = glCreateProgram();
//
//	// Our own shader compile program
//	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
//
//	// Links the two shader objects into one program
//	GLCall(glAttachShader(program, vs));
//	GLCall(glAttachShader(program, fs));
//
//	//glLinkProgram links the program to the GPU
//	GLCall(glLinkProgram(program));
//
//	//Checks program for errors.
//	GLCall(glValidateProgram(program));
//
//	// Clean up intermediate files 
//	GLCall(glDeleteShader(vs));
//	GLCall(glDeleteShader(fs));
//
//	return program;
//}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

int main14(void)
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

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
		// Geometric data of a square
		float positions[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
		};

		// Index buffer indicates which positions each vertex has for each triangle
		unsigned int indices[] = {
			0, 1, 2,		// triangle 1
			2, 3, 0			// triangle 2
		};

		// Setting up Vertex Buffer and Index Buffer
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));
		IndexBuffer ib(indices, 6);

		// Setup the vertex array
		VertexArray va;

		// Setup the vertex buffer layouts
		VertexBufferLayout layout;
		layout.Push<float>(2);

		// Adding vertex buffer and layout together to the vertex array
		va.AddBuffer(vb, layout);

		//----------------------------------------------------------------------------------
		// Lesson: Abstracting Shader Setup to call Shader class instead (Shader.cpp)
		//----------------------------------------------------------------------------------
		// // Parses shader file into two strings for vertex and fragment shaders
		// ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
		// // Creating the shaders
		// GLCall(unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource));
		// // Binding the shader to OpenGL
		// GLCall(glUseProgram(shader));

		// // Get the ID for uniform variable "u_Color" which is in the shader.
		// GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		// ASSERT(location != -1); //error handling
		// // Set data into "u_Color" i.e. pink
		// GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

		// Setting up shaders
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		// Set data (color: pink) into new variable "u_Color"
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		// Unbinding all buffers
		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();
		//GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		//----------------------------------------------------------------------------------

		// new data for uniform variable
		float r = 0.0f;
		float increment = 0.05f;

		//----------------------------------------------------------------------------------

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT); //The glClear function clears colour buffer to preset values.

			//----------------------------------------------------------------------------------
			// Lesson: Replacing shader code with shader class instance
			//----------------------------------------------------------------------------------
			// // Rebinding buffers
			// GLCall(glUseProgram(shader));
			// // Set new uniform values for draw
			// GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

			// Rebinding buffers
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			//----------------------------------------------------------------------------------
			// Previous:
			//----------------------------------------------------------------------------------
			va.Bind();
			ib.Bind();


			// Drawing primitives using the index buffer
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			// Setting new values into uniform variable per draw
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;
			//----------------------------------------------------------------------------------


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		//----------------------------------------------------------------------------------
		// Lesson: No longer need to delete shader as we have destructor
		//----------------------------------------------------------------------------------
		// // Clean Up
		// GLCall(glDeleteProgram(shader));
		//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------
	// Previous: 
	//----------------------------------------------------------------------------------
	}
	// glfwTerminate deletes OpenGl context so need scope to destroy shader before it.
	//----------------------------------------------------------------------------------
	glfwTerminate();
	return 0;
}