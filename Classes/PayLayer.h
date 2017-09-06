#ifndef __PAYLAYER_H__
#define __PAYLAYER_H__

#include "Common.h"

class PayLayer :public CCLayer
{
	int fireNum;
	int price;
	static PayLayer *payInstance;
	enum fireType{ FIRE1 = 1, FIRE2, FIRE3 };
public:
	CREATE_FUNC(PayLayer);
	bool init();
	void initReturn();
	void rCallback(CCObject*);
	void judgePayLayer(CCObject*);
	void goPayLayer(fireType ft,int fireNum, int price);
	void paySuccess(int ft, int fireNum);

	static PayLayer *getPayInstance(){ return payInstance; }
};

#endif