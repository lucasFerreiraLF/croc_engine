#include <Croc.h>

#include "imgui/imgui.h"


class SampleLayer : public Croc::Layer
{
public:
	SampleLayer() :
		Layer("Sample"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPos(0.0f)
	{
		// Triangle Vertex Array Stuff

		m_VertexArray.reset(Croc::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.3f, 0.8f, 0.2f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.8f, 0.9f, 0.2f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.3f, 0.2f, 0.8f, 1.0f
		};

		std::shared_ptr<Croc::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Croc::VertexBuffer::Create(vertices, sizeof(vertices)));
		Croc::BufferLayout layout = {
			{ Croc::ShaderDataType::Float3, "a_Position" },
			{ Croc::ShaderDataType::Float4, "a_Color" },
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Croc::IndexBuffer> indexBuffer;
		indexBuffer.reset(Croc::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Square Vertex Array Stuff

		m_SquareVA.reset(Croc::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};


		std::shared_ptr<Croc::VertexBuffer> squareVB;
		squareVB.reset(Croc::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Croc::BufferLayout squareLayout = {
			{ Croc::ShaderDataType::Float3, "a_Position" }
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Croc::IndexBuffer> squareIB;
		squareIB.reset(Croc::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader.reset(new Croc::Shader(vertexSource, fragmentSource));

		std::string purpleVertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			
			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	

			}
		)";

		std::string purpleFragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.8, 0.3, 0.8, 1.0);
			}
		)";

		m_PurpleShader.reset(new Croc::Shader(purpleVertexSource, purpleFragmentSource));

	}

	void OnUpdate(Croc::Timestep timestep) override
	{
		float time = timestep;

		//CROC_TRACE("Delta time: {0}s, ({1}ms)", timestep.GetSeconds(), timestep.GetMiliSeconds());

		if(Croc::Input::IsKeyPressed(CROC_KEY_LEFT))
			m_CameraPos.x -= m_CameraMoveSpeed * time;
		else if (Croc::Input::IsKeyPressed(CROC_KEY_RIGHT))
			m_CameraPos.x += m_CameraMoveSpeed * time;
			
		if (Croc::Input::IsKeyPressed(CROC_KEY_DOWN))
			m_CameraPos.y -= m_CameraMoveSpeed * time;
		else if (Croc::Input::IsKeyPressed(CROC_KEY_UP))
			m_CameraPos.y += m_CameraMoveSpeed * time;
	
		if (Croc::Input::IsKeyPressed(CROC_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * time;
		if (Croc::Input::IsKeyPressed(CROC_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * time;


		Croc::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Croc::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPos);
		m_Camera.SetRotation(m_CameraRotation);
		Croc::Renderer::BeginScene(m_Camera);
		
		Croc::Renderer::Submit(m_PurpleShader, m_SquareVA);
		Croc::Renderer::Submit(m_Shader, m_VertexArray);
		
		Croc::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		
	}

	void OnEvent(Croc::Event& event) override
	{
	}

	bool OnKeyPressedEvent(Croc::KeyPressedEvent& event)
	{
		
	}

private:
	std::shared_ptr<Croc::Shader> m_Shader;
	std::shared_ptr<Croc::VertexArray> m_VertexArray;

	std::shared_ptr<Croc::Shader> m_PurpleShader;
	std::shared_ptr<Croc::VertexArray> m_SquareVA;;

	Croc::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPos;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

};

class Sandbox : public Croc::Application
{
public:
	Sandbox() 
	{		
		PushLayer(new SampleLayer());
	}

	~Sandbox() 
	{
	}
};

Croc::Application* Croc::CreateApplication() 
{
	return new Sandbox();
}