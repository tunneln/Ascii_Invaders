//BigBaddie.cpp
#include "BigBaddie.h"

BigBaddie::BigBaddie(){
	rate = .45;
	gun = '&';
	type = 'W';
}

BigBaddie::BigBaddie(int a, int b){
	x = a;
	y = b;
	rate = .45;
	type = 'W';
	gun = '&';
}


