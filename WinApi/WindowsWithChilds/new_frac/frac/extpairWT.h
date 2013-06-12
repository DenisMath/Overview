#ifndef EXTPAIRWT
#define EXTPAIRWT
#include<list>
#include<algorithm>
#include<math.h>
#include "extpair.h"
#include "vector"
//#include "stdafx.h"

//using namespace std;

inline
	float maxN(float x, float y)
{ 
	return ((x>y) ? x : y) ;
}

class ExtpairWT{
public:
	ExtpairWT( )
	{ }

	ExtpairWT( Extpair e, float t):extpair(e),tension(t)
	{ 
	}

	ExtpairWT( float xCoord, float yCoord, float t):extpair(xCoord,yCoord),tension(t)
	{ 
	}



	Extpair extpair;
	float tension;

	ExtpairWT &operator=(const ExtpairWT &m){
		extpair = m.extpair;
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

inline
ExtpairWT floorExtWT(const ExtpairWT &input)
{
	ExtpairWT output( floor(input.extpair.xKoord),floor(input.extpair.yKoord), input.tension );
		return output;
}

inline
bool equalExtpair(const ExtpairWT &a, const ExtpairWT &b)
{
	return ( a.extpair == b.extpair );
}

inline

bool moreExtWT(const ExtpairWT &a, const ExtpairWT &b)
{
	return b<a;
}

inline
 void floorListExtWTWeak(std::list<ExtpairWT> &input)
{   
	transform(input.begin(),input.end(),input.begin(), floorExtWT);
	input.sort();
	input.unique();
}

inline
 void floorListExtWTStrong(std::list<ExtpairWT> &input)
{
	transform(input.begin(), input.end(), input.begin(), floorExtWT);
	//input.sort();
	input.sort(moreExtWT);
	input.unique(equalExtpair);
}

inline
 void floorListExtWTStrongInv(std::list<ExtpairWT> &input)
{
	transform(input.begin(), input.end(), input.begin(), floorExtWT);
	//input.sort();
	input.sort();
	input.unique(equalExtpair);
}

void init_color( std::vector<ExtpairWT> input, RGBTRIPLE* color, int Width = 100, int Height = 100);

#endif
