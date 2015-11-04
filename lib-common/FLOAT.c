#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	//nemu_assert(0);
	long long tmp=(long long)a*(long long)b;
	if(tmp>=0)
		return (FLOAT)(tmp>>16);
	else
		return (FLOAT)-(-tmp>>16);
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	//nemu_assert(0);
	return (a<<8)/(b>>8);
}

FLOAT f2F(float a) {
	int aint=*((int*)&a);
	int exp=((aint>>23)&0xFF)-127;//
	//int f=aint&0x7FFFFF;//frac
	int s=(aint>>31)&1;
	
	int f2=(aint&0x7fff)|0x800000;
	int offset=23-exp;
	int t=((f2>>offset)<<16)&0x7fff0000;
	f2=((f2<<(32-offset))>>16)&0x0000FFFF;
	int F=f2+t;
	if(s==1)return -F;
	else
		return F;
	//nemu_assert(0);
}

FLOAT Fabs(FLOAT a) {
	//nemu_assert(0);
	if(a>=0)return a;
	else
		return -a;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

