#ifndef EXTPAIRWT
#define EXTPAIRWT

#include "extpair.h"

//using namespace std;

inline
double max(double x, double y)
{ 
	return (x>y) ? x : y ;
}

class ExtpairWT: public Extpair {
	public:
	ExtpairWT(float x=0, float y=0, float t = 1):Extpair(x,y), tension(t)
	{ }

	float tension;
	
	friend ExtpairWT operator+(const ExtpairWT &a1, const ExtpairWT &a2); 
	friend ExtpairWT operator-(const ExtpairWT &a1, const ExtpairWT &a2); 
	friend ExtpairWT operator*(const ExtpairWT &a1, const ExtpairWT &a2); 
	friend bool operator==(const ExtpairWT &a1, const ExtpairWT &a2); 
	friend bool operator<(const ExtpairWT &a1, const ExtpairWT &a2); 
	ExtpairWT &operator=(const ExtpairWT &m){
	xKoord=m.xKoord;
	yKoord=m.yKoord;
	tension = m.tension;
return *this;
	}

ExtpairWT  operator-(const ExtpairWT& a1){
	ExtpairWT tmp(-a1.xKoord, -a1.yKoord, a1.yKoord);
	return tmp;
}
};

inline
 ExtpairWT operator+(const ExtpairWT &a1, const ExtpairWT &a2){
	ExtpairWT tmp(a1.xKoord+a2.xKoord, a1.yKoord+a2.yKoord, max(a1.tension, a2.tension));
	return tmp;
}

inline ExtpairWT operator-(const ExtpairWT &a1, const ExtpairWT &a2){
	ExtpairWT tmp(a1.xKoord-a2.xKoord, a1.yKoord-a2.yKoord, a1.tension);
	return tmp;
}

inline bool operator==(const ExtpairWT &a1, const ExtpairWT &a2){
	return ((a1.xKoord==a2.xKoord)&&(a1.yKoord==a2.yKoord)&&(a1.tension == a2.tension));
}

inline bool operator<(const ExtpairWT &a1, const ExtpairWT &a2){
	return ((a1.xKoord<a2.xKoord)||((a1.xKoord==a2.xKoord)&&(a1.yKoord<a2.yKoord))||((a1.xKoord==a2.xKoord)&&(a1.yKoord==a2.yKoord)&&(a1.tension == a2.tension)));
}

#endif
