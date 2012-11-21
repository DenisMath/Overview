#ifndef EXTPAIRWT
#define EXTPAIRWT

#include "extpair.h"

//using namespace std;

inline
	double maxN(double x, double y)
{ 
	return ((x>y) ? x : y) ;
}

class ExtpairWT{
public:
	ExtpairWT( )
	{ }

	ExtpairWT( Extpair e, float t)
	{ 
	extpair = e;
	tension = t;
	}

	Extpair extpair;
	float tension;

	ExtpairWT &operator=(const ExtpairWT &m){
		extpair =m.extpair;
		tension = m.tension;
		tension = m.tension;
		return *this;
	}

	ExtpairWT  operator-(){
		ExtpairWT tmp( -extpair, -tension);
		return tmp;
	}
	 //Extpair new1 = -a1.extpair;
	friend bool operator==(const ExtpairWT &a1, const ExtpairWT &a2); 
	friend bool operator<(const ExtpairWT &a1, const ExtpairWT &a2); 
};

inline
bool operator==(const ExtpairWT &a1, const ExtpairWT &a2)
{
	return (a1.extpair == a2.extpair )&&(a1.tension == a2.tension);
}

inline
bool operator<(const ExtpairWT &a1, const ExtpairWT &a2)
{
	return ( a1.extpair < a2.extpair ) || ((a1.extpair == a2.extpair)&&(a1.tension < a2.tension));
}


#endif
