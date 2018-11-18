#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// a macro to break on OpenGL error to help debugging
#define ASSERT(x) if (!(x)) __debugbreak();

// a macro to clear the errors and call glGetError on the same line.
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// Clears all remaining error flags in OpenGL
void GLClearError();

// A function that indicates when and where an OpenGl error occurs
bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};