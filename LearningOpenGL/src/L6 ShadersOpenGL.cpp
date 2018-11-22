#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

/* Lecture: How Shaders Work in OpenGL */
/* Lecture: Writing a Shader in OpenGL */

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
// Lesson: Writing a Shader
//----------------------------------------------------------------------------------
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	// Create a shader object. Type specifies GPU processor optimisation.
	unsigned int id = glCreateShader(type);
	// Shader types: GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, 
	//				 GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, or GL_FRAGMENT_SHADER.
	
	// creates the raw pointer of the source (shader) file
	const char* src = source.c_str();
	// source.c_str() == source == &source[0] 
	// c_str() ensures string is null terminated for C++98 (c++11 guarantees null termination)

	// Replaces the source code in a shader object
	glShaderSource(id, 1, &src, nullptr);
	// glShaderSource(shader, count, strings[], length[])
	// - shader: specifies the name of the shader object
	// - count: number of elements in string and length
	// - strings: the array of strings for the shader
	// - lengths: an array for the length of each string (default: NULL, means strings
	//			  are assumed to be null terminated).

	// Compiles shader object
	glCompileShader(id);

	//----------------------------------------------------------------------------------
	// Lesson: Error handling
	//----------------------------------------------------------------------------------
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	//Returns a parameter from a shader object 
	//GL_SHADER_TYPE, GL_DELETE_STATUS, GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH, GL_SHADER_SOURCE_LENGTH.

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// alloca allocates memory that is automatically freed (on the stack i.e. freed after scope)
		char* message = (char*) alloca(length * sizeof(char));

		// Returns the information log for a shader object
		glGetShaderInfoLog(id, length, &length, message);
		// glGetShaderInfoLog(shader, maxLength, *length, *infoLog)
		// - infolog: Specifies an array of characters that is used to return the information log.

		std::cout << "Failed to compile " << 
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" <<std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	// glGEtShaderiv(shader, pname, *params)
	// - pname: GL_SHADER_TYPE, GL_DELETE_STATUS, GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH, GL_SHADER_SOURCE_LENGTH.
	// - params: returns the requested object parameters. (see http://docs.gl/gl4/glGetShader for disrcription)
	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// creates a program object for shaders. 
	unsigned int program = glCreateProgram();

	// Our own shader compile program
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Links the two shader objects into one program
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	//glLinkProgram links the program to the GPU
	glLinkProgram(program); 

	//Checks program for errors.
	glValidateProgram(program); 

	// Clean up intermediate files 
	glDeleteShader(vs); 
	glDeleteShader(fs);

	return program;
}
//----------------------------------------------------------------------------------
// The shader programs are written as strings are fed into createShader and outputs the program.
// Typically, open them from text files, however we are going to write them as strings
// this time for simplicity sake.


int main(void)
{
	GLFWwindow* window;

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

	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------
	// Previous: 
	//----------------------------------------------------------------------------------
	// Move glewInit() after creating OpenGL context
	GLenum err = glewInit();
	if (GLEW_OK != err)
		std::cout << "Error!" << std::endl;

	// Check OpenGL version
	std::cout << glGetString(GL_VERSION) << std::endl;

	// Storing points in the vertex buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);

	// Selecting a buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Geometric data of a triangle
	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	// Setting the size and adding data to the buffer.
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(positions[0]), positions, GL_STATIC_DRAW);

	// Declaring the type of data layout in the bound buffer.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// Enabling the vertex attribute array
	glEnableVertexAttribArray(0);
	//----------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------
	// Lesson: Writing the shader
	//----------------------------------------------------------------------------------
	std::string vertexShader = R"(
		#version 330 core

		layout(location = 0) in vec4 position;

		void main()
		{
			gl_Position = position;
		}
	)";

	std::string fragmentShader = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		void main()
		{
			color = vec4(1.0, 0.0, 0.0, 1.0);
		}
	)";
	
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);
	//----------------------------------------------------------------------------------


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT); //The glClear function clears colour buffer to preset values.


		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------
		// Previous:
		//----------------------------------------------------------------------------------
		// Simple non-standard draw method to quickly draw our triangle again
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	
	//----------------------------------------------------------------------------------
	// Lesson: Clean Up
	//----------------------------------------------------------------------------------
	glDeleteProgram(shader);
	//----------------------------------------------------------------------------------

	glfwTerminate();
	return 0;
}