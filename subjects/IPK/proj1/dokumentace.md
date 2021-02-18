
# IPK - Počítačové komunikace a sítě
## Projekt 1: _Klient-server pro získání informace o uživatelích_
### Vypracoval: _Miroslav Válka_

---

## <a id="obsah"></a> Obsah
 - [Obsah](#obsah)
 - [Stručné zadání projektu](#zadani)
 - [Aplikační protokol](#protokol)
   - [Popis](#protokol_popis)
   - [Typy zpráv](#protokol_typy)
   - [Příklady posílaných zpráv](#protokol_priklady_zprav)
   - [Příklady komunikace](#protokol_priklady_komunikace)

---

## <a id="zadani"></a> Stručné zadání projektu 
- Navrhněte vlastní aplikační protokol pro přenos informací  o uživatelích na straně serveru ke klientovi.
- Naprogramujte klientskou a serverovou aplikaci, které pomocí vymyšleného protokolu spolu budou spolupracovat.

---
## <a id="protokol"></a> Aplikační protokol
### <a id="protokol_popis"></a> Popis
Protokol využívá TCP komunikaci skrze kterou je posílán formátovaný textový řetězec. Tyto formátované textové řetězce nesou různé informace. Mohou se skládat z následujících částí: 
- ___Jméno protokolu__ s pevně danou velikosti_
- ___Verze protokolu__ s pevně danou velikosti_
- ___Oddělovačů částí__ s pevně danou velikostí_
- ___Kladnou zprávou__ s pevně danou velikostí_
- ___Negativní zprávou__ s pevně danou velikostí_
- ___Názvu akce__ s pevně danou velikosti_
- ___Dalšími daty__ s dynamickou velikostí_

### <a id="protokol_typy"></a> Typy zpráv
- [Žádost o data](#protokol_priklad_zadost)  
  - Posílá klient serveru.
  - Formát: `název_protokolu:verze_protokolu:akce:data`
- [Kladná odpověď](#protokol_priklad_zadost)
  - Server ukončuje posílání dat touto odpovědí pokud proběhlo úspěšně.
  - Klient potvrzuje touto odpovědí každá data, která obdrží.
  - Formát: `název_protokolu:kladna_zpráva`
- [Negativní odpověď](#protokol_priklad_negativni)
  - Server posílání touto odpověď pokud nerozuměl požadavku nebo pokud mu nemohl vyhovět.
  - Formát: `název_protokolu:negativni_zpráva`
- [Prostá data](#protokol_priklad_prostadata)
  - Holý nestrukturovaný textový řetězec.
 
### <a id="protokol_priklady_zprav"></a> Příklady posílaných zpráv
#### <a id="protokol_priklad_zadost"></a> Žádost o data
- Informace o uživately:  `mwarcz:01:n:xlogin00`
- Domovský adresář uživatele:  `mwarcz:01:f:xlogin00`
- Seznam uživatelských loginů s prefixem: `mwarcz:01:l:prefix`

#### <a id="protokol_priklad_kladna"></a> Kladná odpověď
- Potvrzeni/Ukončení uspěšné přenesených dat:`mwarcz:ok`

#### <a id="protokol_priklad_negativni"></a> Negativní odpověď
- Neúspěšné zpracování požadavku:  `mwarcz:ko`

#### <a id="protokol_priklad_prostadata"></a> Prostá data
- Data bez formátu: `xlogin00`, `Jmeno Prijmeni,Dalsi informace`, `/homes/xlogin00/`

### <a id="protokol_priklady_komunikace"></a> Příklady komunikace

#### Vypsání uživatelských loginů začínajících na 'x'.
```
./ipk-client -p 3000 -h server.ipk -l x
```
> 1) Klient odešle: `mwarcz:01:l:x`
> 2) Server zpracuje: `mwarcz`, `01`, `l`, `x`
> 3) Server pošle data a čeká na potvrzení: `xadamn01`
> 4) Klient přijme data, odešle potvrzení a čeká na další data: `mwarcz:ok`
> 5) Server obdrží potvrzení a opakuje postup dokud má data k posílání (viz. 3.).
> 6) Server nemá žádná další data k posílání a oznámí klientovi, že skončil: `mwarcz:ok`
> 7) Klient přijme data, Zjistí, že jde o zprávu `mwarcz:ok`, ukončí čekání na data a pokračuje ve svém běhu.

####  Úspěšné vypsání domovského adresáře uživatele 'xlogin00'. 
```
./ipk-client -p 3000 -h server.ipk -f xlogin00
```
> 1) Klient odešle: `mwarcz:01:f:xlogin00`
> 2) Server zpracuje: `mwarcz`, `01`, `f`, `xlogin00`
> 3) Server pošle data a čeká na potvrzení: `/home/xlogin00/`
> 4) Klient přijme data, odešle potvrzení a čeká na další data: `mwarcz:ok`
> 5) Server posílání a oznámí klientovi, že skončil: `mwarcz:ok`
> 6) Klient přijme data, Zjistí, že jde o zprávu `mwarcz:ok`, ukončí čekání na data a pokračuje ve svém běhu.

####  Neúspěšné vypsání domovského adresáře uživatele 'xlogin00'. 
```
./ipk-client -p 3000 -h server.ipk -f xlogin00
```
> 1) Klient odešle: `mwarcz:01:f:xlogin00`
> 2) Server zpracuje: `mwarcz`, `01`, `f`, `xlogin00`
> 3) Server nenajde uživatele a vrací odpověď: `mwarcz:ko`
> 4) Klient přijme data, Zjistí, že jde o zprávu `mwarcz:ko`, ukončí čekání na data a vypíše chybovou hlášku.

## <a id="zdroje"></a> Zdroje informaci
1) Části kódů byli inspirovány z [demo příkladů](https://wis.fit.vutbr.cz/FIT/st/course-files-st.php?file=%2Fcourse%2FIPK-IT%2Fother%2FDemo_C.zip&cid=11963), které nám jsou k dispozici. Tyto programy napsal [Doc. Ing.  Ondřej Ryšavý, Ph.D.](http://www.fit.vutbr.cz/~rysavy/).
2) Manuálové stránky na serveru merlin a eva. 
  - _Pokud něco je zde v manuálu, tak pravděpodobně to na těchto platformách bude fungovat._
3. Různé rady z webu [Stack Overflow](https://stackoverflow.com/) při opravování funkčnosti programů.
  - např. Mizející zprávy send, BSD make

---
