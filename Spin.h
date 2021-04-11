#pragma once
class Spin
{
public:
	Spin();
	void setPositiveSpin(bool press);
	void setNeqativeSpin(bool press);
	float spinSet();
	~Spin();
private:
	float moveSpeed = 0.0001f;
	float decaySpeed = 0.00005f;
	float positiveSpin;
	float negativeSpin;
};

