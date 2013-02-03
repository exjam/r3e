#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "OpenGL.hpp"
#include "Array.hpp"

class IVertexBuffer {
public:
	IVertexBuffer(){}
	virtual ~IVertexBuffer(){}

	virtual void AddVertex(void* vertex) = 0;

	virtual char* Buffer() = 0;

	virtual unsigned int Size() = 0;
	virtual unsigned int BufferSize() = 0;

	virtual void SetSize(int vertices) = 0;
	virtual void SetCount(int vertices) = 0;
	virtual void SetDynamic(bool dynamic) = 0;

	virtual void Bind() = 0;
	virtual void BindBufferOnly() = 0;
};

template<class T> class VertexBuffer : public IVertexBuffer {
public:
	VertexBuffer() : mBufferID(GL_INVALID_INDEX), mDynamic(false) {}
	~VertexBuffer(){
		mVertices.Clear();
		OpenGL::UnloadBuffer(mBufferID);
	}

	void SetDynamic(bool dynamic){
		mDynamic = dynamic;
	}

	void SetSize(int vertices){
		mVertices.Resize(vertices);
	}

	void SetCount(int vertices){
		mVertices.SetSize(vertices);
	}

	void AddVertex(void* vertex){
		mVertices.PushBack(*((T*)vertex));
	}

	unsigned int Size(){
		return mVertices.Size();
	}

	unsigned int BufferSize(){
		return Size() * sizeof(T);
	}

	char* Buffer(){
		return (char*)mVertices.Data();
	}

	void Bind(){
		BindBufferOnly();
		T::Bind();
		OpenGL::mVertexCount += mVertices.Size();
	}

	void BindBufferOnly(){
		if(mBufferID == GL_INVALID_INDEX) LoadBuffer();
		OpenGL::BindBuffer(GL_ARRAY_BUFFER, mBufferID);
	}

	void UpdateBuffer(){
		if(!mDynamic) return;
		if(mBufferID == GL_INVALID_INDEX) LoadBuffer();

		OpenGL::BindBuffer(GL_ARRAY_BUFFER, mBufferID);
		OpenGL::BufferData(GL_ARRAY_BUFFER, BufferSize(), Buffer(), GL_DYNAMIC_DRAW);
	}

private:
	bool LoadBuffer(){
		OpenGL::GenBuffers(1, &mBufferID);
		if(!mBufferID){
			mBufferID = GL_INVALID_INDEX;
			return false;
		}

		OpenGL::BindBuffer(GL_ARRAY_BUFFER, mBufferID);
		OpenGL::BufferData(GL_ARRAY_BUFFER, BufferSize(), Buffer(), (mDynamic)?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);
		return true;
	}

public:
	Array<T> mVertices;

private:
	GLuint mBufferID;
	bool mDynamic;
};

#endif