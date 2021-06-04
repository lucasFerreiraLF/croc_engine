#include "crocpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Plataform/OpenGL/OpenGLVertexArray.h"

namespace Croc {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:   CROC_CORE_ASSERT(false, "RendererAPI::None is currentyly not supported!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}

		CROC_CORE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}
}