#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/* Lecture: Index Buffers in OpenGL */

//----------------------------------------------------------------------------------
// Previous: 
//----------------------------------------------------------------------------------
// Taking in a shader from a file
// Our own struct to return two string in one function.
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

//Parses the file and delivers back vertex and frament strings.
static ShaderProgramSource ParseShader(const std::string& filepath)
{
	//ifstream input stream class to operate on files
	std::ifstream stream(filepath);

	//our own terminology to seperate the vertex and fragment shader code
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	//devide the file into ss[0], ss[1]
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str() , ss[1].str() };
}


// Compiling a shader
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	// Create a shader object. Type specifies GPU processor optimisation.
	unsigned int id = glCreateShader(type);

	// creates the raw pointer of the source (shader) file
	const char* src = source.c_str();

	// Replaces the source code in a shader object
	glShaderSource(id, 1, &src, nullptr);

	// Compiles shader object
	glCompileShader(id);

	// Error handling
	// Returns a parameter from a shader object 
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// alloca allocates memory that is automatically freed (on the stack i.e. freed after scope)
		char* message = (char*)alloca(length * sizeof(char));

		// Returns the information log for a shader object
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

// The shader programs are written as strings are fed into createShader and outputs the program.
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
//----------------------------------------------------------------------------------

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


	//----------------------------------------------------------------------------------
	// Lesson: How to create an index buffer
	//----------------------------------------------------------------------------------
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
	//----------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------
	// Previous: 
	//----------------------------------------------------------------------------------
	// Setting the size and adding data to the buffer.
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(positions[0]), positions, GL_STATIC_DRAW);

	// Declaring the type of data layout in the bound buffer.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// Enabling the vertex attribute array
	glEnableVertexAttribArray(0);

	//----------------------------------------------------------------------------------
	// Lesson: How to initilize an index buffer
	//----------------------------------------------------------------------------------
	// Storing points in the vertex buffer
	unsigned int ibo;
	glGenBuffers(1, &ibo);

	// Selecting a buffer 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// Setting the size and adding data to the buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	//----------------------------------------------------------------------------------


	// Parses shader file into two strings for vertex and fragment shaders
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	// testing ParseShader
	//std::cout << "VERTEX" << std::endl;
	//std::cout << source.VertexSource << std::endl;
	//std::cout << "FRAGMENT" << std::endl;
	//std::cout << source.FragmentSource << std::endl;

	// Creating the shaders
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	//----------------------------------------------------------------------------------

	// Binding the shader to OpenGL
	glUseProgram(shader);
	//----------------------------------------------------------------------------------
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
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		// Lesson: Drawing using the index buffer
		//----------------------------------------------------------------------------------
		// draws primitives from array buffer
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	//----------------------------------------------------------------------------------
	// Lession: Clean Up
	//----------------------------------------------------------------------------------
	glDeleteProgram(shader);
	//----------------------------------------------------------------------------------

	glfwTerminate();
	return 0;
}