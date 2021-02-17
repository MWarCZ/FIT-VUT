/********************************/
/*       IZP - Projekt č.2      */
/*                              */
/*   Vytvořil: Miroslav Válka   */
/*                              */
/*      Login: xvalka05         */
/*                              */
/********************************/

/*
 _        _  __  __    __  __   __       ______     ____     ______
| |\    /| | \ \ \ \  / / / /  /  \     | |___ \   / __ \   |___  /
| | \  / | |  \ \ \ \/ / / /  / /\ \    | |___| | / /  \_\     / /
| |\ \/ /| |   \ \ \/ / / /  / /__\ \   | |__  /  | |   _     / /
| | \__/ | |    \ \/ /\/ /  / /____\ \  | |  \ \  \ \__/ /   / /__
|_|      |_|     \__/\__/  /_/      \_\ |_|   \_\  \____/   /_____|

-/\/\-\/\/-
*/

/***************************************
 * Definovaná makra pro podmínky ifdef *
 ***************************************/

#define MY_STRING_H
#define MY_MATH_H
#define MY_PRINT_H


/**********************
 * Prilozene knihovny *
 **********************/
#include <stdio.h>
#include <stdlib.h>

/* Využití funkcí log, pow pro kontrolu a konstanty NAN, INF. */
#include <math.h>


/** 
 * Enum TParams
 * Jde o kolekci představující všechny možné parametry, které může program dostat.
 * Jedna proměná může nabývat více hodnot -> proto jsou hodnoty 2 na n
 */
typedef enum
{
	__NA  =  0, // Nebyl zadán parametr
	__ERR =  1, // Chyba
	__LOG =  2, // Načten parametr --log
	__POW =  4, // Načten parametr --pow
	__XX  =  8, // Nacteno číslo x
	__YY  = 16, // Načteno číslo y
	__NN  = 32  // Načteno číslo n
} TParams;

/*********************
 * Prototypy funkci  *
 *********************/

/**
 * Prototypy funkcí pro základní práci s textem
 */
#ifdef MY_STRING_H
int StrLen(char *s);
int StrCmp(char *s1, char *s2);
int IsDouble(char *s);
#endif

/**
 * Prototypy matematických funkcí log, pow
 */
#ifdef MY_MATH_H
double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
double t_pow(int version,double x, double y, unsigned int n);
double taylor_pow(double x, double y, unsigned int n);
double taylorcf_pow(double x, double y, unsigned int n);
/*
double DoubleAbs(double x);
double MySqrt(double a, int b);
double IntegerPow(double x, int y);
double MyPow(double x, double y);
	#define EPS 0.0001
*/
#endif

/**
 * Prototypy funkcí pro tisk požadovaných dat v daném formátu
 */
#ifdef MY_PRINT_H
void PrintLog(double x, unsigned int n);
void PrintPow(double x, double y, unsigned int n);
void PrintHelp();
#endif

int MainWorker(int argc, char **argv);

/*****************
 *  FUNKCE MAIN  *
 *****************/
int main(int argc, char **argv)
{
	(void)argc; (void)argv;

	if( MainWorker(argc, argv) )
	{
		PrintHelp();
	}

	return 0;
}

/**
 * Funkce se stará o zpracovávání parametrů a následné spuštění sprvných funkcí
 * @return : Vrací hodnotu -1 pokud byly zadány neplatné argumenty nebo špatný formát argumentů. Jinak vrací 0.
 * @param argc : Udává velikost pole obsahující textové řeťězce
 * @param argv : Obsahuje pole textových řeťezců
 */
int MainWorker(int argc, char **argv)
{
	(void)argc; (void)argv;
	
	TParams params = __NA;
	TParams loadNumber = __NA;
	double x = -1,y = -1;
	unsigned int n = 0;
	


	for(int i = 1; i < argc; i++)
	{	
		int len = StrLen(argv[i]);

		if(loadNumber)
		{
			int isNumber = IsDouble(argv[i]);
			if(isNumber)
			{
				double tmpValue = atof(argv[i]);

				if( loadNumber & __XX )
				{
					x = tmpValue;
					loadNumber ^= __XX;
					params |= __XX;
				}
				else if( loadNumber & __YY )
				{
					y = tmpValue;
					loadNumber ^= __YY;
					params |= __YY;
				}
				else if( (loadNumber & __NN) && isNumber == 2)
				{
					n = (unsigned int)tmpValue;
					loadNumber ^= __NN; 
					params |= __NN;
				}
				else
				{
					params |= __ERR;
				}
			}
			else
			{
				params |= __ERR;
			}

			if(params & __ERR)
			{
				loadNumber = __NA;
			}
		}
		else if(len == 5)
		{
			if( ! StrCmp(argv[i], "--log") )
			{
				params |= __LOG;
				loadNumber |= (__XX | __NN);
			}
			else if( ! StrCmp(argv[i], "--pow") )
			{
				params |= __POW;
				loadNumber |= (__XX | __YY | __NN);
			}
			else
			{
				params |= __ERR;
			}
		}
		else
		{
			params |= __ERR;
		}
	}

	if( params & __ERR )
	{
		return -1;
	}
	else if( (params == (__LOG | __XX | __NN) ) && n>0 )
	{
		PrintLog(x, n);
	}
	else if( (params == (__POW | __XX | __YY | __NN) ) && n>0 )
	{
		PrintPow(x, y, n);
	}
	else
	{
		return -1;
	}

	return 0;
}

#ifdef MY_STRING_H

/**
 * Funkce pro zjištění déky textového řetězce.
 * @return : Vrací počet znaků v řetězci. (Platný řetězec končí znakem '\0')
 * @param s : Textový řetězec.
 */
int StrLen(char *s)
{
	int i;
	for(i = 0; s[i]; i++)
	{}
	return i;
}
/**
 * Fuknce pro porovnání dvou textových řetězců.
 * @return : Vrací 0 pokud jsou stejné, -1 pokud s1 je menší než s2, 1 pokud s2 je menší než s1.
 * @param s1 : Očekává se textový řetězec jehož poslední znak je \0.
 * @param s2 : Očekává se textový řetezec jehož poslední znak je \0.
 */
int StrCmp(char *s1, char *s2)
{
	int i = 0;

	while(s1[i])
	{
		if(!s2[i])
		{
			return 1;
		}
		if(s1[i] > s2[i])
		{
			return 1;
		}
		if(s1[i] < s2[i])
		{
			return -1;
		}

		i++;
	}
	if(s2[i])
	{
		return -1;
	}

	return 0;
}

//  0 - není číslo
//  1 - Je double
//  2 - Může být int
/**
 * Funkce, která zjišťuje zda vložený textový řetězec je číslo, či nikoliv
 * @return : Vrací 0 pokud textový řetězec není číslo. Vrací 1 pokud textový řetězec je desitiné číslo. Vrací 2 pokud číslo může být celočíselné.
 * @param s : Jde o textový řetězec zakončený znakem '\0'
 */
int IsDouble(char *s)
{
	int dot = 0;
	for(int i = 0; s[i]; i++)
	{
		if( (s[i]>='0' && s[i]<='9') )
		{}
		else if((s[i]=='.') )
		{
			if(dot)
			{
				return 0;
			}
			dot++;

		}
		else if(s[0]=='-' || s[0]=='+')
		{}
		else
		{
			return 0;
		}
	}
	
	if(dot)
	{
		return 1;
	}

	return 2;
}

#endif

#ifdef MY_MATH_H

double taylor_log(double x, unsigned int n)
{
	(void)x; (void)n;
	
	if(x < 0)
	{
		return NAN;
	}
	if(x == 0)
	{
		return -INFINITY;
	}

	double sum = 0;
	int sign;
	double multiply;

	if( x > 0.5 )
	{
		multiply = (x - 1)/x;
		sign = 1;
	}
	else
	{
		multiply = 1-x;
		sign = -1;
	}

	double tmpX = multiply;
	unsigned int i = 1;
	
	while(i <= n)
	{
		sum += (tmpX / i);
		tmpX *= multiply;
		i++;
	}
	sum *= sign;

	return sum;
}


double cfrac_log(double x, unsigned int n)
{
	(void)x; (void)n;

	if(x < 0)
	{
		return NAN;
	}
	if( x == 0 )
	{
		return -INFINITY;
	}
	if( x == 1 )
	{
		return 0;
	}

	//double z = (1.0 + x)/(1.0 - x);
	double z = (x-1)/(1+x);
	double z2 = z * z;
	unsigned int i = n - 1;
	long int a = n * 2 - 1;
	double sum = 0;

	while(i > 0)
	{
		sum = (i * i * z2)/(a - sum);
		a -= 2;
		i--;
	}

	return (2.0*z)/(a - sum);
}

/**
 * Funkce pro výpočet mocniny čísla.
 * @return : Vrací deseniné číslo.
 * @params version : Hodnota nastavuje jakou funkcí musí být zjištěn logaritmus potřebný k dalšímu výpočtu. 0 - Použije se funkce taylor_log. 1 - Použije se funkce cfrac_log.
 * @param x : Hodnota udávající základ mocniny.
 * @param y : Hodnota udávající exponent mocniny.
 * @param n : Hodnota udávající počet iterací, které mají proběhnou při výpočtu.
 */
double t_pow(int version,double x, double y, unsigned int n)
{
	(void)x; (void)y; (void)n;
	
	int sign = 1;
	(void)sign;
	if(x == 0)
	{
		return 0;
	}
	if(x < 0)
	{
		sign = -1;
		x = -x;

		if( (double)y != (double)(int)y )
		{
			return -NAN;
		}
	}

	unsigned int i = 1;
	unsigned int f = i;
	double sum = 1;
	double multiply = (version)? (y * cfrac_log(x,n)) : (y * taylor_log(x,n));
	double tmpX = multiply;
	(void)f;
	while(i <= n)
	{
	
		sum += tmpX;
		i++;
		tmpX = tmpX * multiply / i;
	//	i++;
	//	f = f*i;
	//	tmpX *= multiply;
	}
	
	if((int)y % 2 )
	{
		sum *= sign;
	}

	return sum;
}

/**
 * Fuknce je jen zkratka pro použití funkce t_pow s prvním parametrem 0.
 * Smysl funkce : Dodržení zadání
 */
double taylor_pow(double x, double y, unsigned int n)
{
	return t_pow(0,x,y,n);
}

/**
 * Fuknce je jen zkratka pro použití funkce t_pow s prvním parametrem 1.
 * Smysl funkce : Dodržení zadání
 */
double taylorcf_pow(double x, double y, unsigned int n)
{
	return t_pow(1,x,y,n);
}

/*
double DoubleAbs(double x)
{
	return (x < 0 )? -x : x;
}

double MySqrt(double a, int b)
{
	double x = 1;
	double y = a + x;
	double z = y / b;
	double old = z + 1;

	while(DoubleAbs(old - x) > EPS)
	{
		old = z;
		x = a / z;
		y = z + x;
		z = y / b;
	}

	return z;
}
double IntegerPow(double x, int y)
{
	double sum = 1;
	if(y>0)
	{
		for(int i = 0; i<y; i++)
		{
			sum *= x;
		}
	}
	else if(y<0)
	{
		for(int i = y; i<0; i++)
		{
			sum /= x;
		}
	}
	return sum;
}
double MyPow(double x, double y)
{
	int tmpy1 = (int)y;
	double tmpy2 = y - (double)tmpy1;
	return IntegerPow(x,tmpy1) * MySqrt(x,1/tmpy2);
}
*/
#endif

#ifdef MY_PRINT_H

/**
 * Funkce s vedlejším efektem - slouží k výpisu dat v daném formátu. Vypisuje hodnoty logaritmu pro zadané hodnoty.
 * @param x : Hodnota udávající číslo, které má být zlogaritmizováno.
 * @param n : Hodnota udávající počet iterací výpočtu.
 */
void PrintLog(double x, unsigned int n)
{
	printf(
		"       log(%g) = %.12g\n"
		" cfrac_log(%g) = %.12g\n"
		"taylor_log(%g) = %.12g\n",
		x,log(x),
		x,cfrac_log(x,n),
		x,taylor_log(x,n)
	);
	
	return;
}

/**
 * Funkce s vedlejším efektem - slouží k výpisu dat v daném formátu. Vypisuje číslo o základu x umocněné na y.
 * @param x : Hodnota udávající číslo, které má být umocněno. Tzv základ mocniny.
 * @param y : Hodnota udávající tzv. exponent.
 * @param n : Hodnota udávající počet iterací výpočtu.
 */
void PrintPow(double x, double y, unsigned int n)
{	
	printf(
		"         pow(%g,%g) = %.12g\n"
		"  taylor_pow(%g,%g) = %.12g\n"
		"taylorcf_pow(%g,%g) = %.12g\n",
		x,y,pow(x,y),
		x,y,taylor_pow(x,y,n),
		x,y,taylorcf_pow(x,y,n)
	);

	return;
}

/**
 * Funkce s vedlejším efektem - sloužící k výpisu nápovědy k programu
 */
void PrintHelp()
{
	printf(
	"|-------------------------------|\n"
	"| proj2 --log X N               |\n"
	"| proj2 --pow X Y N             |\n"
	"|-------------------------------|\n"
	"--log X N\tProgram vypíše přirozený logaritmus čísla X\n"
	"         \tvypočítaný v N intracích.\n"
	"--pow X Y N\tProgram vypíše umocněné číslo X na Y\n"
	"         \tvypočítané v N iteracích.\n"
	);
	return;
}

#endif

