#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RendererID)); //生成缓冲区对象
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); //绑定缓冲区对象
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); //设置缓冲区数据
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID)); //删除缓冲区对象
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); //绑定缓冲区对象
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); //解绑缓冲区对象
}
