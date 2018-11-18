#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"



#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	TestTexture2D::TestTexture2D()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0) 
	{
		// Enable blending of alpha (layers of transparency in textures)
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		// Geometric data of a square with texture coordinates (x, y, normal_x, normal_y)
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f,
		};

		// Setup the vertex buffer layouts
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		// Index buffer indicates which positions each vertex has for each triangle
		unsigned int indices[] = {
			0, 1, 2,		// triangle 1
			2, 3, 0			// triangle 2
		};

		// Setting up Vertex Buffer and Index Buffer
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		// Setup the vertex array
		m_VAO = std::make_unique<VertexArray>();

		// Adding vertex buffer and layout together to the vertex array
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		// Setting up shaders
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();

		// Set data (color: pink) into new variable "u_Color"
		m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		// Setting up textures
		m_Texture = std::make_unique<Texture>("res/textures/Nu Final.png");

		//// Bind to texture slot
		//texture.Bind();

		// Sets the texture slot
		m_Shader->SetUniform1i("u_Texture", 0);

		//// Unbinding all buffers
		//va.Unbind();
		//shader.Unbind();
		//vb.Unbind();
		//ib.Unbind();
	}

	TestTexture2D::~TestTexture2D()
	{
	}
	
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}
	
	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		// Binding texture
		m_Texture->Bind();

		// first icon draw
		{
			// the model matrix
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);

			// multiplying a translation matrices together to get final image
			glm::mat4 mvp = m_Proj * m_View * model;

			// Rebinding buffers
			m_Shader->Bind();

			// Setting uniform to pass in projection into vertex shader
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			// Drawing primitives using the vertex array, index buffer and shader
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		// second icon draw
		{
			// the model matrix
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);

			// multiplying a translation matrices together to get final image
			glm::mat4 mvp = m_Proj * m_View * model;

			// Rebinding buffers
			m_Shader->Bind();

			// Setting uniform to pass in projection into vertex shader
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			// Drawing primitives using the vertex array, index buffer and shader
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}
	
	void TestTexture2D::OnImGuiRender()
	{
		// Creating UI window
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}