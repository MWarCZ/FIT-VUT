
# IPK - Počítačové komunikace a sítě
## Projekt 1: _Klient-server pro získání informace o uživatelích_
### Vypracoval: _Miroslav Válka_

---

## Obsah
- [Serverová aplikace](#server)
- [Klientská aplikace](#client)
- [Kompilace](#make)
- [Známé chyby](#errors)
---

## <a id="server"></a> Serverová aplikace
### Použití: 
```
./ipk-server [-p port] [-6]
```
### Přepínače:
- __-p port__  _( Povinný )_
  - port - Číslo portu na kterém bude server komunikovat.
- __-6__ _( Nepovinný )_
  - Nastavuje server pro komunikaci skrze IPv6. Pokud není nastaven komunikuje skrze IP verze 4.

> Aplikace __ipk-server__ je spustitelná jak na Linuxu _(server merlin)_ , tak na BSD _(server eva)_ .

---

## <a id="client"></a> Klientská aplikace
### Použití: 
```
./ipk-client [-h host] [-p port] [-n login|-f login|-l prefix]
```
### Přepínače:
- __-h host__  _( Povinný )_
  - host - Doménové jméno nebo IP adresa serveru se kterým se bude komunikovat.
- __-p port__  _( Povinný )_
  - port - Číslo cílového portu serveru na kterém komunikuje.
- __-n login__  _( Povinně volitelný z: -n | -f | -l )_
  - login - Žádost o zaslání celého jména a dalších informací uživatele s daným loginem.
- __-f login__  _( Povinně volitelný z: -n | -f | -l )_
  - login - Žádost o zaslání domovského adresáře uživatele s daným loginem.
- __-l prefix__  _( Povinně volitelný z: -n | -f | -l )_
  - prefix - Žádost o zaslání seznamu loginů uživatelů na serveru, které začínají na zadaný prefix. Prefix může být i prázdný.

> Aplikace __ipk-client__ je spustitelná jak na Linuxu _(server merlin)_ , tak na BSD _(server eva)_ .

---

## <a id="make"></a> Kompilace
Pro kompilaci stačí použít nástroj `make`. Dodaný [Makefile](Makefile) je plně kompatibilní s GNU make a částečně kompatibilní s BSD make.
### Cíle:
- Výchozí cíl: `all`
- `all` - Provede zkompilování jak klientské tak serverové aplikace.
- `debug` - Provede zkompilování jak klientské tak serverové aplikace s makrem DEBUG.
- `ipk-client` - Zkompiluje klientskou aplikaci.
- `ipk-server` - Zkompiluje serverovou aplikaci.
- `clean` - Smaže zkompilované binární soubory.
> Všechny cíle až na `debug` jsou kompatibilní s GNU i BSD make. 
> Cíl `debug` je kompatibilní jen s GNU make.

---

## <a id="errors"></a> Známé chyby
1) Přijímání dat, která jsou delší než 1023 znaků. Tato chyba může nastat při posílání informacích o uživateli nebo pokud někdo bude mít login delší než 1023 znaků.
2) U serverové aplikace občas dochází k memory leaku. Důvod neznámí - nepodařilo se mi najít žádný důvod, proč se tak stává, takže jediná možnost je memory leak způsobený nějakou knihovnou.

---
