#pragma once

template<class T>
class ResultHandler
{
private:
	bool _gotResult;
protected:
	void SetGotResult(bool gotIt) { _gotResult = gotIt; }
public:
	ResultHandler() { _gotResult = false; }
	ResultHandler(ResultHandler<T>& other) { _gotResult = other._gotResult; }
	virtual ~ResultHandler() = default;

	virtual ResultHandler<T>* clone() = 0;

	virtual bool UpdateResult(T* result) = 0;

	bool GetGotResult() const
	{
		return _gotResult;
	}
};

