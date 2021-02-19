# ESP8266: ovládání LED (IoT, WiFi AP pro mobilní telefon), W

# Společná část popisu:

Každý student se, prostřednictvím IS FIT, přihlásí na jedno z vypsaných  témat projektů (viz Projekt / Varianty termínu). 

Přihlašování začíná ve středu 26.9. od 20 hod.
Není-li u projektu (explicitně) uvedeno jinak, projekty jsou (implicitně) vypsány k individuálnímu, tj. samostatnému řešení každou z přihlášených osob s doporučeným implementačním jazykem C. 

## Odevzdání a hodnocení řešení projektů

- Nutnou podmínkou pro získání bodů za projekt je vypracování projektu v souladu se zadáním a odevzdání řešení projektu v jediném ZIP archívu do IS FIT. Má-li projekt více řešitelů (tj. je-li explicitně označen jako skupinový projekt), zvolí si tito mezi sebou jednoho, kterého odevzdáním archívu pověří.
- V případě skupinových projektů uložte do výše zmíněného ZIP archívu také soubor info.txt, který bude obsahovat jména, příjmení a loginy řešitelů projektu. U každého řešitele bude uvedeno, jaký byl jeho procentuální podíl na řešení projektu jako celku. U každého z řešitelů skupinového projektu budou navíc v info.txt shrnuty i) informace o částech projektu, na jejichž řešení pracoval, ii) realizační výstupy vyprodukované řešitelem při řešení projektu a iii) příslušné pracovní aktivity a jejich výstupy ve vztahu k řešením projektu jako celku. V případě, že tento soubor odevzdán nebude nebo v něm budou zmíněné náležitosti u některých řešitelů chybět, pak body za projekt nebudou těmto řešitelům uděleny. 
- Pouhá funkčnost dle zadání, shromáždění materiálů dle zadání a splnění dalších náležitostí zadání je nutnou, ale ne dostačující podmínkou pro získání plného počtu bodů za projekt. Dostačující podmínka se považuje za splněnou je-li projekt vyřešen pečlivě a kvalitně, čímž se rozumí zejména vhodná dekompozice daného problému na podproblémy, efektivita implementace, přehlednost zdrojových textů, dále vhodnost, účelnost a dostatečnost komentářů ve zdrojových textech, přehlednost, logická struktura a ilustrativnost odevzdané dokumentace.
- První řádky každého souboru, který byl vytvořen či modifikován pro účely řešení projektu, musí obsahovat následující informace: jméno, příjmení a login autora změn, stručný popis změn provedených v souboru včetně odhadu podílu (v %) změn vzhledem k původnímu obsahu souboru (v případě 100% autorství explicitně uveďte "originál") a datum provedení poslední změny v souboru.

## Řešení skupinových projektů

- Pro minimalizaci problémů spojených s řešením skupinových projektů se doporučuje, aby se řešitelský tým skupinového projektu sešel co nejdříve a nadále se scházel v pravidelných intervalech.
- Lhůta pro zjištění práceschopnosti týmu je čtvrtek, 18. 10. 2018. Do tohoto termínu (včetně) můžete informovat vedoucího projektu o neschopnosti práce daného týmu, o vystoupení z něj a požádat o změnu zadání na jiné (oznámíte které) s tím, že tato změna již bude definitivní. V tomto případě (tj. při změně zadání) je doporučeno zvolit si zadání individuální. 

## Dotazy k projektům

- Pro pokládání dotazů souvisejících s projekty využijte fóra Dotazy k projektům, které bylo k tomuto účelu zřízeno. 
- Dotazy ke konkrétním tématům projektů zodpoví příslušní vedoucí, jejichž bližší identifikaci naleznete v detailním popisu k projektu (Vede: ...), zkráceně pak i u názvu projektu (za názvem projektu je za oddělující čárkou uvedeno zkrácené příjmení vedoucího projektu:
    - B = M. Bidlo (bidlom@fit.vutbr.cz, L330),
    - N = J. Nevoral (inevoral@fit.vutbr.cz, L323),
    - S = J. Strnadel, (strnadel@fit.vutbr.cz, L332),
    - Š = V. Šimek (simekv@fit.vutbr.cz, L323),
    - W = M. Wiglasz (iwiglasz@fit.vutbr.cz, L307).
Případné dotazy obecné povahy zasílejte garantovi předmětu. Předmět dotazu musí začínat řetězcem IMP_proj následovaným mezerou; jinak hrozí, že na něj nebude reagováno.

## Popis varianty:
Vede: M. Wiglasz

S pomocí modulu NodeMCU a programovacího prostředí pro Arduino navrhněte vestavný systém umožňující ovládat připojené LED. Zařízení bude využívat WiFi v režimu AP, na které bude možné se připojit pomocí mobilního telefonu.

### Požadavky implementace:
1. Zajistěte, aby se systém choval za všech okolností přirozeně a byly rozumným způsobem ošetřeny vstupy.
2. Kromě ovládání jednotlivých světel umožněte i spuštění nějaké sekvence (střídavé blikání, rotace a podobně). Řízení těchto sekvencí, časování a podobně musí být řešeno v procesoru (t.j. klient odešle přes webové rozhraní příkaz pro rozblikání světel a vlastní blikání dělá procesor).
3. Bude hodnota i kvalita uživatelského prostředí (můžete použít např. Framework7).

Moduly ESP8266 k projektu bude možné zapůjčit od vedoucího.

* * *

Následující informace a požadavky jsou společné pro všechna zadání od M. Wiglasze.
V případě nejasností se řiďte obecnými pokyny k projektům nebo vzneste dotaz.

A. Případné detaily nespecifikované v zadání řešte dle vlastního uvážení.

B. Podmínkou hodnocení projektu je osobní demonstrace funkčnosti řešení vyučujícímu. Demonstrace řešení s obhajobou proběhnou po termínu odevzdání, informace k tomu budou včas sděleny mailem.

C. Dílčí složkou pro hodnocení je též stručná dokumentace poskytující základní informace o projektu a způsobu řešení. Ta by měla obsahovat zejména:

1. Odstavec s textovým popisem projektu - o co se jedná, účel, funkce.
2. Popis ovládání (význam použitých kláves/tlačítek kitu , popis terminálových příkazů apod.).
3. Externí schéma(ta) zapojení, spojení s kitem, fotografie ukazující navržená externí/pomocná zařízení, případně informace o specifických nastavení kitu.
4. Stručný popis způsobu řešení - stačí jen info o klíčových částech implementovaných řešitelem, případně zmínka o převzatých kódech, jejich modifikaci či externích knihovnách (je-li třeba).
5. Závěrečné shrnutí - zhodnocení řešení, známé problémy a nedostatky, sdělení o případných nedokončených/neimplementovaných částech s ohledem na zadání.

Není podstatný ani tak rozsah dokumentace, ale to, zda obsahuje podstatné informace o projektu. Preferuji formát pdf nebo prostý text (pokud postačuje).

D. Odevzdávejte jediný archiv (zip nebo tar.gz) pojmenovaný Vaším loginem, který bude obsahovat adresář opět nazvaný podle loginu a v něm dokumentaci a příslušné zdrojové soubory uspořádané tak, jak je obvyklé ve vývojovém prostředí použitého kitu (např. pro Qdevkit u FITkitu, projekt z KDS apod.).
