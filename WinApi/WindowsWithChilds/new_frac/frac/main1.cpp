#include<iostream>
#include<cmath>
#include "matrix.h"
using namespace std;

int main()
{
	float a11, a12, a21, a22, b1, b2, alpha, tension;
	while(1){
	cout<<"a11 = ";
	cin>> a11;
	cout<<"a12 = ";
	cin>> a12;
	cout<<"a21 = ";
	cin>> a21;
	cout<<"a22 = ";
	cin>> a22;
	cout<<"b1 = ";
	cin>> b1;
	cout<<"b2 = ";
	cin>> b2;
		/*cout << "alpha = " ;
		cin >> alpha ;
		cout << "tension = " ;
		cin >> tension ;
		cout<<"b1 = ";*/
	/*cin>> b1;
	cout<<"b2 = ";
	cin>> b2;*/
	//Matrix a(tension*cos((PI/180)*alpha) , -tension*sin((PI/180)*alpha) ,tension*sin((PI/180)*alpha) ,tension*cos((PI/180)*alpha) );
	Matrix a(a11, a12, a21, a22 );
	Extpair b(b1, b2);
	Extpair result = getStablePoint(a,b);
	cout<<"result = ("<< result.xKoord << "," << result.yKoord << ")"<<endl;}
	system("pause");
	return 0;
}