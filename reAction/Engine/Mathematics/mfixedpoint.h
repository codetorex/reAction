#ifndef MFIXEDPOINT_H
#define MFIXEDPOINT_H

// 28.4 fixed point
class fxd
{
public:
	fxd();
	fxd(float b);
	fxd(int b);


	int a;
	
	operator	float	(void);
	operator	int		(void);

	fxd&	operator =  (float b);
	fxd&	operator =  (int b);

	fxd		operator+	(fxd b);
	fxd		operator-	(fxd b);
	fxd		operator*	(fxd b);
	fxd		operator/	(fxd b);
};

// 24.8 fixed point
class fxd8
{
public:
	fxd8();
	fxd8(float b);
	fxd8(int b);
	
	int a;
	
	operator	float	(void);
	operator	int		(void);
	
	fxd8&	operator =  (float b);
	fxd8&	operator =  (int b);
	
	fxd8	operator+	(fxd8 b);
	fxd8	operator-	(fxd8 b);
	fxd8	operator*	(fxd8 b);
	fxd8	operator/	(fxd8 b);	
};



#endif