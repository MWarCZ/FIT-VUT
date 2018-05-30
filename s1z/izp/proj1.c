
/*****************************
 * IZP - Projekt c.1         *
 * Vytvoril: Miroslav Valka  *
 * xvalka05                  *
 *****************************/

/***************************************
 * Definovaná makra pro podmínky ifdef *
 ***************************************/
 
/* Funkce pro práci s textem a znaky */
#define MY_STRING_H

/* Funkce pro převody čísel na text a opačně */
#define MY_CONVERT_H

/* Jedna funkce, zpracovávající požadavky vyplývající z parametru -S. ! Pozor pouze zakrývá text, který nesplňuje požadavky. */
//#define PRINT_SS_V1

/* Jedna funkce, zpracovávající požadavky vyplývající z parametru -S. ! Pozor funguje jen v normě C99 a novější. Využívá deklaraci velikosti pole pomocí parametru. Výslený text je ořezaná verze vstupního textu. */
#define PRINT_SS_V2

/* Přidává programu další možný parametr -X. Má stejnou funkci jako -x, ale výstupní HEX číslice jsou velkými písmeny. */
//#define PARAM_XX

/**********************
 * Prilozene knihovny *
 **********************/
#include <stdio.h>
#include <ctype.h>

/* 
 * Enum TParams
 * Jde o kolekci představující všechny možné parametry, které může program dostat.
 * Jedna proměná může nabývat více hodnot -> proto jsou hodnoty 2 na n
 */
typedef enum 
{
	_NA = 0, // Nebyl zadán parametr
	_ER = 1, // Chyba
	_S = 2,  // Načten parametr -s
	_N = 4,  // Načten parametr -n
	_X = 8,  // Načten parametr -x
	_R = 16, // Načten parametr -r
	_SS = 32, // Načten parametr -S
	_XX = 64 // Načten parametr -X
} TParams;

/*********************
 * Prototypy funkci  *
 *********************/

/*
 * Prototypy funkcí pracujících s převody textu na čísla a opačně
 */
#ifdef MY_CONVERT_H
int GetValueDigit(char digit, int numberSystem);
char GetDigit(int value);
void IntToString(unsigned int number, int numberSystem, char *outString, int outCount, int addZeros);
int StringToInt(char *stringNumber, int numberSystem);
#endif

/*
 * Prototypy funkcí pro základní práci s textem
 */
#ifdef MY_STRING_H
char UpCase(char c);
char LowerCase(char c);
int StrLen(char *s);
void UpString(char *inString, const int inCount, char *outString, const int outCount);
#endif

/*
 * Prototypy funkcí pro zpracování parametrů programu
 */
int MainWork(int argc, char **argv);

/*
 * Prototypy funkcí pro tisk požadovaných dat v daném formátu
 */
void PrintHelp();
void PrintFormatedData(int _SValue, int _NValue);
void PrintData(int BigChars);
void PrintReverseData();
void PrintBiggerStrings(const int _SSValue);
	

/******************
 *  MAIN funkce   *
 ******************/
int main(int argc, char **argv)
{
	if( MainWork(argc, argv) )
	{
		PrintHelp();
	}
	
	return 0;
}

/*
 * Funkce se stará o zpracovávání parametrů a následné spuštění sprvných funkcí
 * @return : Vrací hodnotu -1 pokud byly zadány neplatné argumenty nebo špatný formát argumentů. Jinak vrací 0.
 * @param argc : Udává velikost pole obsahující textové řeťězce
 * @param argv : Obsahuje pole textových řeťezců
 */
int MainWork(int argc, char **argv)
{
	TParams enteredParams = _NA;
	TParams loadNumber = _NA;
	int _SValue=-1,_NValue=-1,_SSValue=-1;

	for(int i = 0; i < argc; i++)
	{
		char *s = argv[i];
		if(loadNumber)
		{
			int tmpValue = StringToInt(s,10);
			if(tmpValue == -1)
			{
				enteredParams |= _ER;
				
			}

			if(loadNumber == _S)
			{
				_SValue = tmpValue;
			}
			else if(loadNumber == _N)
			{
				_NValue = tmpValue;
			}
			else if(loadNumber == _SS)
			{
				_SSValue = tmpValue;
			}

			loadNumber = _NA;
		}
		else if(StrLen(s) == 2)
		{
			if(s[0]=='-')
			{
				switch(s[1])
				{
					case 's': 
						enteredParams |= _S;
						loadNumber |= _S;
						break;
					case 'n': 
						enteredParams |= _N;
						loadNumber |= _N;
						break; 
					case 'x': 
						enteredParams |= _X;
						break;
					case 'r': 
						enteredParams |= _R;
						break;
					case 'S': 
						enteredParams |= _SS;
						loadNumber = _SS;
						break;
				#ifdef PARAM_XX
					case 'X':
						enteredParams |= _XX;
						break;
				#endif
					default: 
						enteredParams |= _ER;
						break;
				}
			}

		}
		else if(i==0)
		{
			// Pokud bych chtěl načítat jméno souboru
			// Je možné podmínku zrušit a cyklus procházet id 1
		}
		else
		{
			enteredParams |= _ER;
		}
	}
	
	if(enteredParams & _ER)
	{
		return -1;
	}

	if(enteredParams == _NA)
	{
		PrintFormatedData(-1,-1);
	}
	else if(enteredParams == _SS && _SSValue != -1 )
	{
		PrintBiggerStrings(_SSValue);
	}
	else if(enteredParams == _R )
	{
		PrintReverseData();
	}
	else if(enteredParams == _X)
	{
		PrintData(0);
	}
#ifdef PARAM_XX
	else if(enteredParams == _XX)
	{
		PrintData(1);
	}
#endif
	else if(enteredParams == _S && _SValue != -1)
	{	
		PrintFormatedData(_SValue,-1);
	}
	else if(enteredParams == _N && _NValue != -1)
	{
		PrintFormatedData(-1,_NValue);
	}
	else if(enteredParams == (_S | _N) && _SValue != -1 && _NValue != -1)
	{
		PrintFormatedData(_SValue,_NValue);
	}
	else
	{
		return -1;
	}

	return 0;
}

/*
 * Funkce s vedlejším efektem - sloužící k výpisu nápovědy k programu
 */
void PrintHelp()
{
	printf(
	"|-------------------------------|\n"
	"| proj1 [-s Number] [-n Number] |\n"
	"| proj1 -x                      |\n"
	"| proj1 -r                      |\n"
	"| proj -S Number                |\n"
	"|-------------------------------|\n"
	"-s N\tVynechá N znaků, než začne tisknout.\n"
	"-n N\tVytiskne N znaků.\n"
	"-x\tVytiskne poze HEX výpis bez formátování.\n"
	"-r\tNačítá HEX znaky, které převádí na bajty a tiskne znaky dané hodnoty.\n"
	"\tOstatní znaky ignoruje.\n"
	"-S N\tTiskne posloupnosti, které vypadají jako řetězec delší nebo rovno N znaků.\n"
	);
	return;
}

/*
 * Funkce s vedlejším efektem - slouží k načtení dat a k následnému výpisu dat v daném formátu.
 * @param _SValue : Hodnota udávající počet znaků, které se mají přeskočit 
 * @param _NValue : Hodnota udávající počet znaků, které se mají vypsat. Hodnota -1 znamená "Čti dokud nedojdeš na konec vstupních dat."
 */
void PrintFormatedData(int _SValue, int _NValue)
{
	int c = 'a', i = 0;
	char buffer[17], tmpArray[9];
	unsigned int address = 0;
	int tmp_NValue = (_NValue == -1)? 1 : _NValue;
	
	while ( _SValue > 0 && (c = getchar()) != EOF)
	{
		_SValue--;
		address++;
	}
	
	while( c != EOF && tmp_NValue > 0)
	{
		// Načítání max 16 bajtů. Konec když je EOF nebo když není potřeba další znak
		while( i < 16 && tmp_NValue > 0 && (c = getchar()) != EOF)
		{
			buffer[i] = c;
			i++;

			// Pokud byl zadám počet znaků, které se mají vytisknout
			if(_NValue > -1) 
			{
				tmp_NValue--;
			}
		}
		
		// Ošetření na nepřečtení žádného dalšího znaku ke zpracování (př. čtení prázdného souboru)
		if(i > 0) 
		{
			// Vytisknutí adresy
			IntToString(address, 16, tmpArray, 9, 1);
			printf("%s ",tmpArray);
			
			// Tisknutí jednotlivých bajtů
			for(int j = 0; j < i; j++)
			{
				IntToString(buffer[j], 16, tmpArray, 3, 1);
				printf("%s ",tmpArray);
			}
			// Doplnění mezer pro zachování zarovnání
			for(int j = i; j < 16; j++)
			{
				printf("   ");
			}
			
			putchar('|');
			
			// Tisknutí načtených znaků (netiskuntelný znak = .)
			for(int j = 0; j < i; j++)
			{
				int tmpChar = ( isprint(buffer[j]) )? buffer[j] : '.';
				putchar(tmpChar);
			}
			// Doplnění mezer pro zachování zarovnání
			for(int j = i; j < 16; j++)
			{
				putchar(' ');
			}
			putchar('|');
			putchar('\n');

			address += i;
			i = 0;
		}
	}
	
	return;
}

/*
 * Funkce s vedlejším efektem - slouží k načtení dat a k následnému výpisu zpracovaných dat ve formátu HEX číslic. 1 Bajt = 2 HEX znaky.
 * @param BigChars : Určuje zda bude výstupní text velkými písmeny. Hodnota 0 - text bude malými písmeny. Nenulová hodnota - text bude vrlkými písmeny.
 */
void PrintData(int BigChars)
{
	int c = 'a';
	char tmpArray[3];
	while( (c = getchar()) != EOF )
	{
		IntToString(c,16,tmpArray,3,1);
		if(BigChars)
		{
			UpString(tmpArray,3,tmpArray,3);
		}
		printf("%s",tmpArray);
	}
	putchar('\n');
	return;
}
#ifdef PARAM_XX

#endif

/*
 * Funkce s vedlejším efektem - slouží ke čtení dat a vytisknutí zpracovaných dat. Převede textový HEX vstup na znaky, které vytiskne. Znaky, které nejsou HEX cifry jsou přeskočeny.
 */
void PrintReverseData()
{
	int c = 'a', i = 0, tmpChar;
	char tmpArray[3] = { '\0' };
	while( c != EOF)
	{
		// Načtení maximálně 2 bajtů
		while( i < 2 && (c = getchar()) != EOF)
		{
			// Pokud znak je HEX číslicí
			if( GetValueDigit( LowerCase(c), 16 ) != -1)
			{
				tmpArray[i] = LowerCase(c);
				i++;
			}
		}
		tmpArray[i] = '\0';
		// Převedení a vytisknutí znaku
		tmpChar = StringToInt(tmpArray,16);
		if(tmpChar != -1 && c != EOF)
		{
			putchar(tmpChar);
		}
		i = 0;
	}

	return;
}

#ifdef PRINT_SS_V1
/*
 * Funkce s vedlejším efektem - slouží k načtení dat a následnému výpisu dat v daném formátu. Netiskne textové řetězce, které jsou kradší než zadaný parametr.
 * @param _SSValue : Hodnota minimální velikosti textového řetězce, který může být vypsán.
 */
void PrintBiggerStrings(const int _SSValue)
{
	int c = 'a', i = 0;
	while( c != EOF)
	{
		while( (c = getchar()) != EOF)
		{
			if(c == '\n' || c == '\0')
			{
				break;
			}
			putchar(c);
			i++;
		}
		
		// Pokud je vytisknutý text menší než požadovaná velikost tak se smaže, jinak se odřádkuje
		if(i < _SSValue)
		{
			for(; i > 0; i--)
			{
				printf("\b \b");
			}
		}
		else
		{
			i = 0;
			putchar('\n');
		}
	}
}
#endif

#ifdef PRINT_SS_V2
void PrintBiggerStrings(const int _SSValue)
{
	int c='a', i = 0;
	while(c != EOF)
	{
		char pole[_SSValue+1];
		while( i<_SSValue && (c = getchar())!=EOF )
		{
			if(c=='\n' || c=='\0')
			{
				i = -1;
			}
			else
			{
				pole[i]=c;
			}
			
			i++;
			if(i==_SSValue)
			{
				pole[i]='\0';
				printf("%s",pole);
				while( (c = getchar())!=EOF && c!='\0' && c!='\n')
				{
					putchar(c);
				}
				putchar('\n');
				i = 0;
			}
		}
	}
	return;
}
#endif

#ifdef MY_CONVERT_H

/*
 * Funkce pro převod znaku číslice na číselnou hodnotu. 
 * @returm : Vrací číselnou hodnotu číslice. Vrací -1 pokud se nejedná o platnou číslici.
 * @params digit : Jde o znak číslice, pro kterou má být zjištěna její hodnota.
 * @params numberSystem : Jde o hodnotu určující číselný systém pro danou číslici. Číselný systém může být maximálně HEXadecimální ( 2 = BIN, 10 = DEC, 16 = HEX )
 */
int GetValueDigit(char digit, int numberSystem)
{
	char *allDigit = "0123456789abcdef";
	int i = 0;

	while(allDigit[i] != '\0' && i < numberSystem)
	{
		if(allDigit[i] == digit)
		{
			return i;
		}
		i++;
	}
	return -1;
}

/*
 * Funkce vrací znak číslice pro danou hodnotu
 * @return : Vrací číslici (znak), která představuje hodnotu v parametru. Podporované číslice: 0-9 a a-f.
 * @param value : Číselná hodnota hledané číslice.
 */
char GetDigit(int value)
{
	char *allDigit = "0123456789abcdef";
	if(value >= 0 && value < 16)
	{
		return allDigit[value];
	}
	return 0;
}

/*
 * Funkce pro převod textu na číslo.
 * @return : Vrací čísolnou hodnotu převedeného čísla. Vrací -1 pokud text nebyl číslo pro daný numerický systém.
 * @param stringNumber : Textový řetězec, který má byt převeden na číslo.
 * @param numberSystem : Jde o hodnotu určující číselný systém. Číselný systém může být maximálně HEXadecimální ( 2 = BIN, 10 = DEC, 16 = HEX )
 */
int StringToInt(char *stringNumber, int numberSystem)
{
	int x = 0;
	int i = 0;
	while(stringNumber[i] != '\0')
	{
		int tmp = GetValueDigit(stringNumber[i], numberSystem);
		if(tmp == -1)
		{
			return tmp;
		}
		x = x * numberSystem + tmp;
		i++;
	}
	return x;
}

/*
 * Funkce pro převod čísla na text
 * @param number : Číselná hodnota, která má být převedena na text.
 * @param numberSystem : Jde o hodnotu určující číselný systém. Číselný systém může být maximálně HEXadecimální ( 2 = BIN, 10 = DEC, 16 = HEX )
 * @param outString : Jde o ukazatel na pole znaků do kterého bude uloženo převedené číslo.
 * @param outCount : Hodnota uvádějící velikost pole outString.
 * @param addZeros : Pokud obsahuje nenulovou hodnotu, tak doplní znakem '0' zbylá místa v poly.
 */
void IntToString(unsigned int number, int numberSystem, char *outString, const int outCount, int addZeros)
{
	unsigned int tmpNumber = number;
	int i = 0;
	while(i < (outCount-1) && tmpNumber > 0) 
	{
		int x = tmpNumber % numberSystem;
		tmpNumber /= numberSystem;
		outString[i] = GetDigit(x);
		i++;
	}
	
	while(i < (outCount-1) && addZeros)
	{
		outString[i] = '0';
		i++;
	}

	outString[i] = '\0';

	int last = i/2 ;
	i--;
	int j = 0;
	while(j < last && i >= 0)
	{
		int tmp = outString[j];
		outString[j] = outString[i];
		outString[i]=tmp;
		i--;
		j++;
	}

	return;
}

#endif


#ifdef MY_STRING_H

/*
 * Funkce převede malé písmeno na velké.
 * @return : Vrací velké písmeno.
 * @param c : Znak který má být převeden na velké písmeno.
 */
char UpCase(char c)
{
	if( (c >= 'a') && (c <= 'z') )
	{
		return c - ('a'-'A') ;
	}
	return c;
}

/*
 * Funkce převede velké písmeno na malé.
 * @return : Vrací malé písmeno.
 * @param c : Znak který má být převeden na malé písmeno.
 */
char LowerCase(char c)
{
	if( (c >= 'A') && (c <= 'Z') )
	{
		return c + ('a'-'A') ;
	}
	return c;
}

/*
 * Funkce pro zjištění déky textového řetězce.
 * @return : Vrací počet znaků v řetězci. (Platný řetězec končí znakem '\0')
 * @param s : Textový řetězec.
 */
int StrLen(char *s)
{
	int i = 0;
	while(s[i] != '\0')
	{
		i++;
	}
	return i; 
}

/*
 * Funkce převádí písmena ve vstupním textu na velká a ukládá je do výstupního textu.
 * @param inString : Vstupní textový řetězec.
 * @param inCount : Délka vstupního textového řetězce.
 * @param outString : Výstupní textový řetězec.
 * @param outCount : Velikost výstupního textového řetězce.
 */
void UpString(char *inString, const int inCount, char *outString, const int outCount)
{
	int min = (inCount > outCount)? outCount : inCount;

	for(int i = 0; i < min; i++)
	{
		outString[i]=UpCase(inString[i]);
	}

	return;
}
#endif

