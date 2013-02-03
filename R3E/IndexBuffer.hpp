#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "OpenGL.hpp"
#include "Array.hpp"

class IIndexBuffer {
public:
	IIndexBuffer(){}
	virtual ~IIndexBuffer(){}

	virtual void AddIndex(void* index) = 0;

	virtual char* Buffer() = 0;

	virtual unsigned int Size() = 0;
	virtual unsigned int BufferSize() = 0;

	virtual void SetSize(int indices) = 0;
	virtual void SetCount(int indices) = 0;

	virtual void SetType(int type) = 0;

	virtual void Bind() = 0;
	virtual void Draw() = 0;
};

template <class T> class IndexBuffer : public IIndexBuffer {
public:
	IndexBuffer() : mBufferID(GL_INVALID_INDEX) {
		mType = GL_TRIANGLES;
	}

	~IndexBuffer(){
		mIndices.Clear();

		if(mBufferID != GL_INVALID_INDEX)
			OpenGL::DeleteBuffers(1, &mBufferID);
	}

	void SetSize(int indices){
		mIndices.Resize(indices);
	}

	void SetCount(int indices){
		mIndices.SetSize(indices);
	}

	void AddIndex(void* index){
		mIndices.PushBack(*((T*)index));
	}

	unsigned int Size(){
		return mIndices.Size();
	}

	unsigned int BufferSize(){
		return Size() * sizeof(T);
	}

	void SetType(int type){
		mType = type;
	}

	char* Buffer(){
		return (char*)mIndices.Data();
	}

	void Bind(){
		if(mBufferID == GL_INVALID_INDEX) LoadBuffer();
		OpenGL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
	}

	void Draw(){
		glDrawElements(mType, mIndices.Size(), GL_UNSIGNED_SHORT, 0);
	}

private:
	bool LoadBuffer(){
		OpenGL::GenBuffers(1, &mBufferID);
		if(!mBufferID){
			mBufferID = GL_INVALID_INDEX;
			return false;
		}

		OpenGL::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
		OpenGL::BufferData(GL_ELEMENT_ARRAY_BUFFER, BufferSize(), Buffer(), GL_STATIC_DRAW);
		return true;
	}

private:
	Array<T> mIndices;
	GLuint mBufferID;
	int mType;
};

#endif