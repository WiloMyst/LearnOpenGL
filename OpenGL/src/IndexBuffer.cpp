#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint)); //检查unsigned int和GLuint大小是否相同

	GLCall(glGenBuffers(1, &m_RendererID)); //生成缓冲区对象
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); //绑定缓冲区对象
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); //设置缓冲区数据
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID)); //删除缓冲区对象
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); //绑定缓冲区对象
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); //解绑缓冲区对象
}
