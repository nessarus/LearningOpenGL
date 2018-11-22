#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/* Lecture: Dealing with Errors in OpenGL */

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
// Lesson: Initializing step before calling glGetError
//----------------------------------------------------------------------------------
// Clears all remaining error flags in OpenGL
static void GLClearError()
{
	//clears the errors
	while (glGetError() != GL_NO_ERROR);
}

// prints all the new errors.
//static void GLCheckError()
//{
//	while (GLenum error = glGetError())
//	{
//		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
//	}
//}

//----------------------------------------------------------------------------------
// Lesson: Automating the error message generation process
//----------------------------------------------------------------------------------
// a macro to your compiler
#define ASSERT(x) if (!(x)) __debugbreak();
// Note: __function means that the macro is specific to Microsoft VC, not gcc.

// a macro to clear the errors and call glGetError on the same line.
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
// Note: the new line will end the macro. To prevent this, end the line with with a \.

// Simplistic function to return false when an error occurs
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------

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
	GLCall(unsigned int id = glCreateShader(type));

	// creates the raw pointer of the source (shader) file
	const char* src = source.c_str();

	// Replaces the source code in a shader object
	GLCall(glShaderSource(id, 1, &src, nullptr));

	// Compiles shader object
	GLCall(glCompileShader(id));

	// Error handling
	// Returns a parameter from a shader object 
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		// alloca allocates memory that is automatically freed (on the stack i.e. freed after scope)
		char* message = (char*)alloca(length * sizeof(char));

		// Returns the information log for a shader object
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
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
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	//glLinkProgram links the program to the GPU
	GLCall(glLinkProgram(program));

	//Checks program for errors.
	GLCall(glValidateProgram(program));

	// Clean up intermediate files 
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

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
	GLCall(glGenBuffers(1, &buffer));

	// Selecting a buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));


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

	// Setting the size and adding data to the buffer.
	GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(positions[0]), positions, GL_STATIC_DRAW));

	// Declaring the type of data layout in the bound buffer.
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	// Enabling the vertex attribute array
	GLCall(glEnableVertexAttribArray(0));

	// Initilize an index buffer
	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));

	// Selecting a buffer 
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

	// Setting the size and adding data to the buffer.
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(indices[0]), indices, GL_STATIC_DRAW));

	// Parses shader file into two strings for vertex and fragment shaders
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	// testing ParseShader
	//std::cout << "VERTEX" << std::endl;
	//std::cout << source.VertexSource << std::endl;
	//std::cout << "FRAGMENT" << std::endl;
	//std::cout << source.FragmentSource << std::endl;

	// Creating the shaders
	GLCall(unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource));

	// Binding the shader to OpenGL
	GLCall(glUseProgram(shader));
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

		// Drawing primitives using the index buffer
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		//----------------------------------------------------------------------------------
		// Lesson: Handling an error.
		//----------------------------------------------------------------------------------
		// Clear all other errors
		//GLClearError();

		//glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
		//glDrawElements now has an error (GL_INT instead of GL_UNSIGNED_INT).
		
		// Check the new errors from glDrawElements
		//GLCheckError();
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		// Lesson: Automating the error handling process.
		//----------------------------------------------------------------------------------
		// Assert allows us to break the code at the point of the error. 
		//ASSERT(GLLogCall());
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		// Lesson: To further automate the error handling process.
		//----------------------------------------------------------------------------------
		// GLCall uses a macro to call GLCLearError, and ASSERT(GLLogCall()) 
		//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr));
		//----------------------------------------------------------------------------------


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------
	// Previous:
	//----------------------------------------------------------------------------------
	// Clean Up
	GLCall(glDeleteProgram(shader));
	//----------------------------------------------------------------------------------

	glfwTerminate();
	return 0;
}