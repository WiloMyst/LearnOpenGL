#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID; //缓冲区标识
	unsigned int m_Count; //索引数量
public:
	IndexBuffer(const unsigned int* data, unsigned int count); //构造函数
	~IndexBuffer(); //析构函数

	void Bind() const; //绑定缓冲区
	void Unbind() const; //解绑缓冲区

	inline unsigned int GetCount() const { return m_Count; } //获取索引数量
};