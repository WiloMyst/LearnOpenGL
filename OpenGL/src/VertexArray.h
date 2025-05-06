#pragma once

#include "VertexBuffer.h"

class VertexArray
{	
private:

public:
	VertexArray(); //���캯��
	~VertexArray(); //��������

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout); //��ӻ�����

	void Bind() const; //�󶨶����������
	void Unbind() const; //��󶥵��������
};