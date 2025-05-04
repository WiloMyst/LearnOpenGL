#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID; //��������ʶ
public:
	VertexBuffer(const void* data, unsigned int size); //���캯��
	~VertexBuffer(); //��������

	void Bind() const; //�󶨻�����
	void Unbind() const; //��󻺳���
};