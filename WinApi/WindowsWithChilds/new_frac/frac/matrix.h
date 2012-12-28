#ifndef MATRIX
#define MATRIX
//#include <iostream>
#include "extpair.h"
#include "extpairWT.h"
#include <math.h>

#define PI 3.14159265

inline
float alpha_max(float A, float B)
{
	if(B > 0) 
	{
		if(A > 0)
		{
			return atan(B/A)/2;
		}
		else if(A < 0)
		{
			return PI/4 + atan(A/B)/2;
		}
		else
		{
			return PI/4;
		}
	}
	else if (B = 0)
	{
		if(A > 0)
		{
			return PI;
		}
		else if (A < 0)
		{
			return PI/2;
		}
		else 
		{
			return 0;
		}
	}
	else 
	{
		if(A != 0)
		{
			return (PI+atan(B/A))/2;
		}

		else if(A < 0)
		{
			return PI/4 - atan(A/B)/2;
		}

		else
		{
			return 3*PI/4;
		}
	}
}


class Matrix {
private:
	float tension;
	public:

	Matrix(float x = 1,float y = 0,float z = 0,float v = 1): oo(x), ot(y), to(z), tt(v)
	{ tension = 0; }

	Matrix(const Matrix & a): oo(a.oo), ot(a.ot), to(a.to), tt(a.tt)
	{ }

	float oo;
	float ot;
	float to;
	float tt;

	float SetTension()
	{
		float A1 = oo*oo + to*to;
		float A2 = ot*ot + tt*tt;
		float A3 = oo*ot + to*tt;
		float alpha = alpha_max(A1 - A2, 2*A3);
		tension = sqrt(A1*cos(alpha)*cos(alpha) + A2*sin(alpha)*sin(alpha) + A3*sin(2*alpha));
		//tension = sqrt(oo*oo+ot*ot+tt*tt+to*to);
		return tension;
		
	}

	float GetTension()
	{
		return tension;
	}

	Extpair multRow(const Extpair &v)//mult 2x2 matrix on row vector;
	{
		Extpair tmp( oo*(v.xKoord)+ot*(v.yKoord) , to*(v.xKoord)+tt*(v.yKoord) );
		return tmp;
	}

	Extpair operator*(const Extpair &v)//mult 2x2 matrix on row vector;
	{
		Extpair tmp( oo*(v.xKoord)+ot*(v.yKoord) , to*(v.xKoord)+tt*(v.yKoord) );
		return tmp;
	}

	friend Matrix operator-(const Matrix &a1); 
	friend Matrix operator+(const Matrix & a1, const Matrix & a2); 
	friend Matrix operator-(const Matrix & a1, const Matrix & a2); 
	friend Matrix operator*(const Matrix & a1, const Matrix & a2); 
	friend Matrix operator*(float a1, const Matrix & a2); 
	friend bool operator==(const Matrix & a1, const Matrix & a2); 

	Matrix & operator=(const Matrix &m){
		oo=m.oo;
		ot=m.ot;
		to=m.to;
		tt=m.tt;
return *this;
	}
};

const Matrix ID_MATRIX(1,0,0,1);

inline
Matrix operator-(const Matrix &a1){
	Matrix tmp(-a1.oo, -a1.ot, -a1.to, -a1.tt);
	return tmp;
}

inline
Matrix operator+(const Matrix & a1, const Matrix & a2){
	Matrix tmp(a1.oo+a2.oo, a1.ot+a2.ot, a1.to+a2.to, a1.tt+a2.tt);
	return tmp;
}

inline
Matrix operator-(const Matrix & a1, const Matrix & a2){
	Matrix tmp(a1.oo-a2.oo, a1.ot-a2.ot, a1.to-a2.to, a1.tt-a2.tt);
	return tmp;
}

inline
Matrix operator*(const Matrix & a1, const Matrix & a2){
	Matrix tmp(a1.oo*a2.oo+a1.ot*a2.to, a1.oo*a2.ot+a1.ot*a2.tt, a1.to*a2.oo+a1.tt*a2.to, a1.to*a2.ot+a1.tt*a2.tt);
	return tmp;
}

inline
Matrix operator*(float a1, const Matrix & a2){
	Matrix tmp(a1*a2.oo, a1*a2.ot, a1*a2.to, a1*a2.tt);
	return tmp;
}

inline
bool operator==(const Matrix & a1, const Matrix & a2)
{
	return ((a1.oo==a2.oo)&&(a1.ot==a2.ot)&&(a1.to==a2.to)&&(a1.tt==a2.tt));
}

inline 
	float detOfMatrix(const Matrix & a)
{
	return (a.oo*a.tt - a.to*a.ot);
}

inline
Matrix invMatrix(const Matrix &a){
	double temp = detOfMatrix(a);
	Matrix tmp(a.tt, -a.ot, -a.to, a.oo);
	double temp1 = 1/temp;
	if(temp = 0){return ID_MATRIX;}
	else{return temp1*tmp;}
}

inline
Extpair getStablePoint(const Matrix & a, const Extpair &b)//return stable point of a*x + b
{
	Matrix temp = invMatrix(ID_MATRIX - a);
	Extpair temp1 = temp*b;
	return temp*b;
}


#endif
