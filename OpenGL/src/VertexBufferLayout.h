#pragma once

#include <vector>

#include <GL/glew.h>

struct VertexBufferElement
{
	unsigned int type; //��������
	unsigned int count; //��������
	unsigned char normalized; //�Ƿ��һ��
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements; //���㻺����Ԫ��
	unsigned int m_Stride; //����
public:
	VertexBufferLayout() : m_Stride(0) {} //���캯��
	~VertexBufferLayout() {} //��������

	template<typename T>
	void Push(unsigned int count) //������������
	{
		ASSERT(false); //���Դ���
	}
	template<>
	void Push<float>(unsigned int count) //����float����
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT); //���㲽��
	}
	template<>
	void Push<unsigned int>(unsigned int count) //����unsigned int����
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT); //���㲽��
	}
	template<>
	void Push<unsigned char>(unsigned int count) //����unsigned char����
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE); //���㲽��
	}
};