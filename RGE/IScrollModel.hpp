#ifndef ISCROLLMODEL_HPP
#define ISCROLLMODEL_HPP

class IScrollModel {
public:
	virtual void SetValue(int val) = 0;

	virtual int GetValue() = 0;
	virtual int GetMax() = 0;
	virtual int GetMin() = 0;
};

#endif
