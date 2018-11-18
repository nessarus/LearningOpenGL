#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string & filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
	// Parses shader file into two strings for vertex and fragment shaders
	ShaderProgramSource source = ParseShader(filepath);

	// Creating the shaders
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

//Parses the file and delivers back vertex and frament strings.
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
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
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string & name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string & name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

// Set data (color) into new variable "name"
void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

// Setting data (matrix) into new variabl "name"
void Shader::SetUniformMat4f(const std::string & name, const glm::mat4 & matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

// Get the ID for uniform variable "name.c_str()" which is in the shader.
int Shader::GetUniformLocation(const std::string & name)
{

	// Returns glGetUniformLocation if called before
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	
	// OpenGL comand to fetch location of variable from Shader Object (the Shader compiled file)
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}
