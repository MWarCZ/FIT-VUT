
# 4. Uhlíková stopa v zemědělství, lesnictví a zpracovatelském průmyslu

Zadání [zde](zadani.md).

## Make 

- `make` 
	- Zkompilovani projektu.
- `make run`
	- Spusteni jedne konkretni prednastavene simulace.
	- Vystup zvolen na zkraceny.
- `make r ARGS="<parametry>"`
	- Spusteni simulace s pozadovanym scenarem experimentu.
	- Dostupne scenare: 1,2,3,4,5,6,7,8,9,a,b,c,d,e,f,g,h,i,j,k,l,m.
	- Pro kratky vypis vysledku slouzi prepinac -s .
- `make vrun ARGS="<parametry>"`
	- Spousti simulaci pres valgrind.
- `make clean`
	- Vymaze slozku se zkompilovanymi soubory.

## Priklad pouziti
```
make
...

make run
...

make r ARGS="-s b"
...

make clean
...
```

