
# 53. Velká Éra Pirátů

Piráti dopili rum a chtěji vytvořit informační systém, který by zefektivnil jejich rabování. Pirátské posádky mají svá unikátní jména, své Jolly Rogery (tzn. vlajky) a sestávají (pochopitelně) z bandy pirátů. Jednotliví piráti, mimo svého jméno (nicméně existuje řada bezejmenných pirátů) a přezdívky (např. Černovous), jsou charakterizováni svou pozicí v posádce (navigator, kuchař, doktor, kormidelník, .), věkem, barvou vousů, časem stráveným v posádce a seznamem charakteristik (chybějící oko, papoušek na rameni, dřevěná noha,.). 

Každá posádka má svého (hlavního) kapitána a vlastní buď jednu loď, nebo celé flotily sestávající z více lodí. Každá loď i flotila má pak svého divizního kapitána (může to být i kapitán celé posádky), a je charakterizována typem (karavela, brigantina,.) a přístavem ve kterém kotví, přičemž kapacita každé lodi pro posádku je omezená. Piráti se můžou plavit maximálně na jedné lodi. 

U přístavů uchováváme informace o tom, zda se jedná o teritorium specifické pirátské posádky, nebo o neutrální uzemí a název (polo)ostrovu, kde se nachází a kapacitu ukotvených lodí. Pirátské posádky dále vytváří vzájemné aliancie. Tyto aliance mají dohodnutý jeden přístav (může se jednat o teritorium jedné z posádek), ve kterém probíhají alianční schůzky. Jednotlivé posádky i celé aliance pak mezi sebou mohou bojovat. 

U těchto bitev evidujeme, zda probíhaly v přístavu (a případně v kterém) nebo na volném moři a dále počet ztrát (stačí kvantitativně) a konkrétní loďě co se v bitvě zapojily. Systém navíc umožňuje kapitánům posádek vyzvat alianční posádky o pomoc (při chystané bitvě; předpokládejte formu jednoduchého broadcastu). 

Pro jednoduchost předpokládejte, že POUZE kapitán posádky může manipulovat v IS s údaji o svých posádkách a lodích (tedy divizní a flotilní kapitáni mají v systému stejná práva jako řádoví piráti a jejich speciální privilegia a povinnosti se projevují pouze ve reálných událostech, jako jsou bitvy a plavby).
