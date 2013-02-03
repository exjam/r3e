#ifndef STB_H
#define STB_H

namespace ROSE {
	class STB {
	public:
		STB();
		STB(const char* path);
		~STB();

		bool Load(const char* path);

		int GetInt(int row, int column) const; 
		const char* GetString(int row, int column) const;

		unsigned int Rows() const;
		unsigned int Columns() const;

	private:
		unsigned int mRowCount;
		unsigned int mColCount;

		long mBytes;
		long* mDataOffsets;
		char* mStbData;
	};
};

#endif