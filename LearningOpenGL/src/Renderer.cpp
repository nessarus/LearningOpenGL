#include "Renderer.h"

#include <iostream>

// Clears all remaining error flags in OpenGL
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

// A function that indicates when and where an OpenGl error occurs
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	//The glClear function clears colour buffer to preset values.
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	// Drawing primitives using the index buffer
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
