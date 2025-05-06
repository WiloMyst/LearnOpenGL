#pragma once

#include <vector>

#include <GL/glew.h>

struct VertexBufferElement
{
	unsigned int type; //数据类型
	unsigned int count; //数据数量
	unsigned char normalized; //是否归一化
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements; //顶点缓冲区元素
	unsigned int m_Stride; //步长
public:
	VertexBufferLayout() : m_Stride(0) {} //构造函数
	~VertexBufferLayout() {} //析构函数

	template<typename T>
	void Push(unsigned int count) //推送数据类型
	{
		ASSERT(false); //断言错误
	}
	template<>
	void Push<float>(unsigned int count) //推送float类型
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT); //计算步长
	}
	template<>
	void Push<unsigned int>(unsigned int count) //推送unsigned int类型
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT); //计算步长
	}
	template<>
	void Push<unsigned char>(unsigned int count) //推送unsigned char类型
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE); //计算步长
	}
};