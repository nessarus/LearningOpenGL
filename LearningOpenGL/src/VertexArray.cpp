#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "Renderer.h"


VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout)
{
	// binds vertex array and vertex buffer
	Bind();
	vb.Bind();

	// adds vertex buffer layout per element
	const auto& elements = layout.GetElements();
	unsigned int offset = 0; 
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		// The vertex attributes (vertex data layout) binds to index i of currently bound vertex array
		GLCall(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*) offset));

		// Recalculate offset
		offset += element.count * VertexBufferElement::GetSizedOfType(element.type);

		// Enables vertex array index i vertex attribute (vertex data layout)
		GLCall(glEnableVertexAttribArray(i));
	}

}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
