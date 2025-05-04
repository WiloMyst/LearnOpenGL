#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID; //��������ʶ
	unsigned int m_Count; //��������
public:
	IndexBuffer(const unsigned int* data, unsigned int count); //���캯��
	~IndexBuffer(); //��������

	void Bind() const; //�󶨻�����
	void Unbind() const; //��󻺳���

	inline unsigned int GetCount() const { return m_Count; } //��ȡ��������
};