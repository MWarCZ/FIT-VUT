
# ESP8266: Ovládání LED

- **Vypracoval:**
  - **Jméno:** Miroslav Válka
  - **Login:** xvalka05

## Zadání

S pomocí modulu NodeMCU a programovacího prostředí pro Arduiono navrhněte vestavný systém umožňující ovládat připojené LED. Zařízení bude vzužívat Wifi v režimu AP, na které bude možné se připojit pomocí mobilního telefonu.

## Popis ovládání

### Aplikace

Ovládat chování displeje je možné za pomocí připravené aplikace.
Aplikace davá k dispozici:
1) Ovládat LEDky v matici
   - Nastavit konkrétní diody na displeji, které mají svítit.
   - Je možné takto vytvořit statický obrázek.
2) Nahrát text
   - Nahrát text, který se bude zobrazovat na displeji.
   - Je možné nastavit typ přechodu mezi písmeny.
   - Nastavit lze i doba mezi zobrazenim jednotlivich snimků na displej.
3) Vybrat interní animaci
   - Odešle požadavek o nastavení animace, které je uložena v displeji.
   - Je taktéž možné nastavit typ přechodu snímku animace.
   - Taktéž lze nastavit i doba mezi zobrazovanými sníkmy na displeji.
4) Nastavení
   - Nastavení orientace řádků
   - Nastavení orientace sloupců
   - Toto umožnuje otočit vykreslování o 180% či zrcadlově převrátit.
5) Vytvořit vlastní animaci
   - TATO FUNKCE NENI ZCELA DOKONCENA A ODLADENA
   - Umožnuje vytvořit vlastni animaci z jednotlivých snímků.
   - Taktéž je možné upravovt typ přechodu a dobu zobrazování jednotlivých snímků.

> Aplikace je jen webová stránka, která je převedena do mobilní aplikace za pomocí platformy Apache Cordova.

### API NodeMCU serveru

WI-FI AP: `xvalka05`
IP NodeMCU: `192.168.4.1`
API:
- `/opt/` - Nastavení parametrů displeje
  - `cols={Cokoliv}` - Prohození orientace vykreslovaných sloupců.
  - `rows={Cokoliv}` - Prohození orientace vykreslovaných řádků.
  - `rate={Číslo>0}` - Manipulace s frekvenci obnovování displeje.
    - Toto je jen experimentálni.

- `/text/` - Nastavení textu pro zobrazování na displej.
  - `t={Text: A-Z,a-z,0-9,.?!;,+-=*/}` - Text, který se bude zobrazovat.
  - `d={Číslo>0}` - Doba trvání snímků.
  - `style={0-2}` - Styl animace mezi snimky animace.

- `/load/` - Vybrání interní animace pro zobrazování na displej.
  - `n={0-9}` - Vybrana interni animace.
  - `d={Číslo>0}` - Doba trvání snímků.
  - `style={0-2}` - Styl animace mezi snimky animace.

- `/set/` - Nastavení vlastních snímků pro zobrazování na displej.
  - `s={0-69}` - Číslo snímku, který se bude upravovat.
  - `d={Číslo>0}` - Trvání vybraného snímku.
  - `r0={0-255}` - Nastavení všech sloupců řádku č.0.
    - 8-bitové číslo, kde kazdý bit odpovídá jednomu sloupci
    - Společné pro parametry `r0` až `r7`
  - `r{0-7}={0-255}` - Nastavení všech sloupců řádku č.{0-7}.
  - `l={1-70}` - Počet snímků přes které bude rotovat animace.
  - `style={0-2}` - Styl animace mezi snimky animace.

## Externi schema zapojeni

### Použité komponenty
- NodeMCU v3 (Vývojová deska)
- KINGBRIGHT TC23-11SRWA (Maticový LED panel 8x8 - displej)
- 74HC4051 DIP16 TEXAS INSTRUMENTS (8-kanálový analogový multiplexor)
- Nepájivé pole
- Drátky

### Zapojení pinů

| NodeMCU | TC23-11SRWA |
|---------|-------------|
|      D0 | 16          |
|      D1 | 15          |
|      D2 | 11          |
|      D3 |  6          |
|      D4 | 10          |
|      D5 |  4          |
|      D6 |  3          |
|      D7 | 14          |

| NodeMCU  | 74HC4051      |
|----------|---------------|
|       D8 | 11 (S0 - LSB) |
|  D9 (RX) | 10 (S1)       |
| D10 (TX) |  9 (S2 - HSB) |
|       V3 | 16 (Vcc)      |
|       V3 | 3 (A)         |
|      GND | 8 (GND)       |
|      GND | 7 (Vee)       |
|      GND | 6 (E)         |

|TC23-11SRWA | 74HC4051 |
|------------|----------|
|          9 | 13 (A0)  |
|         14 | 14 (A1)  |
|          8 | 15 (A2)  |
|         12 | 12 (A3)  |
|          1 |  1 (A4)  |
|          7 |  5 (A5)  |
|          2 |  2 (A6)  |
|          5 |  4 (A7)  |

> Čísla pinů a označení vychází z oficiální dokumentace k daným komponentám.


## Způsob řešení zadani (klíčové části implementace, převzate kody, knihovny)

### Server - NodeMCU

Pro řešení zadání jsem zvolil, že budu ovládat maticový displej s LED diodami 8x8.

NodeMCU nemá 16 programovatelných volných pinů a tak NodeMCU pomocí 8 pinů ovládá hodnoty sloupců displeje a za pomocí 3 pinů adresuje multiplexor pro výběr řádku displeje (Celkem 11 pinů).

Led diodi nesvídí stále, ale dochází k rychlému blikání jednotlivích LED diod, kdy v jeden moment svítí jen jedna nebo žádná. Byl vyzkoušen i způsob, kdy v jeden moment svítí více diod v řádku, ale tento způsob měl problém s nestálou intenzitou svetla vydávané diodami (8 svítících diod svítí méně než 2 svítící diody => Způsobeno limitací napajení.)

Smyčka programu v NodeMCU je optimalizována, aby k překreslování snímků na displej nedocházelo zbytečně často a byl zde dostatečný prostor pro obstarávání komunikace s klienským zařízením zkrze Wi-Fi AP (Mezi každou překreslovanou diodou nikoliv jen mezi celími snímky).

Vyrkreslovací funkce postupně vykresluje na displej uložené snímky. Snímky nesou informaci o sloupci, řádku a o tom zda daná dioda má svítit. Zde došlo k prostorové optimalizaci a jeden řádek displeje je reprezentován 8-bitovým číslem, kde každý bit představuje jeden sloupec a nese informaci o svícení diody.

### Klient - Aplikace/Web

Aplikace pro ovládání displeje je webovou aplikací, kterou je možné portovat do mobilních telefonů za pomocí platformy `Apache Cordova`.
Aplikace je vyvýjena na frameworku Vue.js a používá vizuální knihovnu `Framework7` a ikony z knihovny `material-design-icons`.

> Odevzdávány jsou jen kódy k aplikaci.
> Pro zkompilování kódů je nutné mít node.js a balíčkovací systém npm nebo yarn.
> Pro port webové aplikace do aplikace pro mobilní zařízení je nutné:
> Vytvořit prázdný projekt Cordova (Např. ve Visual Studio 2017),
> Zkompilovat webovou aplikaci aplikaci (`npm build` nebo `yarn build`),
> Zkopírovat obsah vzniklé složky `dist/` do `www/` v Cordova projektu,
> Následně je možní aplikaci portnout a vytvořit tak například APK balíček.

## Shrnutí

Známé problémy/nedostatky:

- Ne zdela funkční vytváření vlastních animací.
  - Při zvědšujícím se počtu snímků dochází k nadměrné komunikaci či k příliž datově objemné komunikaci, jejíž důsledkem je neuspěch či dokonce restart serveru.

- Malé množství typů přechodů mezi snímky
  - Dostupné je Přeblikávání, Horizontální posouvální z prava do leva, Vertikální posouvání z dola na horu.

- Nedostatečný jas diod
  - Bylo by zapotřebí zařídit externí napájení na větší napětí a proud.

- Není možné využívat RX a TX k jejich původnímu účelu
  - RX a TX jsou využívány k adresaci řádků displeje.
