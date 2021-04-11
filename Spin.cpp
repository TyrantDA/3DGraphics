#include "Spin.h"



Spin::Spin()
{
	positiveSpin = 0;
	negativeSpin = 0;
}

void Spin::setPositiveSpin(bool press)
{
	if (press)
	{
		positiveSpin += moveSpeed;
	}
	else
	{
		positiveSpin -= decaySpeed;
	}
	if (positiveSpin > 360 || positiveSpin < 0)
	{
		positiveSpin = 0;
	}
}

void Spin::setNeqativeSpin(bool press)
{
	if (press)
	{

		negativeSpin -= moveSpeed;
	}
	else
	{
		negativeSpin += decaySpeed;
	}
	if (negativeSpin < -360 || negativeSpin > 0)
	{
		negativeSpin = 0;
	}
}

float Spin::spinSet()
{
	return positiveSpin + negativeSpin;
}

Spin::~Spin()
{
}
