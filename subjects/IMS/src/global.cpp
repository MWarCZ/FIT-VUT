
#include <simlib.h>
#include <cstdio>
#include "global.hpp"
#include "borderer.hpp"

// Pokusy
Store SkladX("Sklad X", 10);
// increments CO2
// const double INC_CO2_YOUNG_TREE = -1.0;
// const double INC_CO2_OLD_TREE = -1.0;
// Queue WaitingOnGrownTree("Cekajici zajemci na strom.");
// Queue GrownTrees("Dospele stromy, dostupne v lese.");


// Uhlik uchovany stromem na eqivalentní CO2: 1 C = 3.67 CO2
// a = rok => tj CO2/a = CO2 za rok
//
// Les (Sekvekace CO2):
// +-------------------+---------+------------------+
// | Strom             | Třešeň  | Dub     | Topol  |
// |-------------------|---------|---------|--------|
// | MaxVěk a          |  45     | 100     | 25     |
// | Počet pcs         |  32     |  38     | 42     |
// | ObsahC tC/a       |   1.1   |   1.1   |  1.1   |
// | CO2 eq tCO2/a     |   4.037 |   4.037 |  4.037 |
// | CO2/pcs kCO2/a    | 126.156 | 106.237 | 96.119 |
// | CO2/pcs kCO2/a/12 |  10.513 |   8.853 |  8.010 |
// +-------------------+---------+---------+--------+
//
// Školka (Produkce CO2):
// +--------------+--------------------------+
// | Rozloha      | 6900 ha                  |
// | Sazenic      | 2000000 pcs/a            |
// | CO2          | 99 kgCO2/ha              |
// |--------------|--------------------------|
// | CO2/pcs/a    | 0.34155 kgCO2/psc/a      |
// | CO2/pcs/a/12 | 0.0284625 kgCO2/psc/a/12 |
// +--------------+--------------------------+


// if(isInQueue()) Out();

////////////////////////
// STORE & FACILITY  //
//////////////////////


unsigned long MAX_CAPACITY_TREE_NURSERY = 1;
Store treeNursery("Stromová školka (Kapacita)", MAX_CAPACITY_TREE_NURSERY);
// Prevod rozlohy a roztece na pocet stromu
void SetMaxCapacityTreeNursery(double area_ha, double spacing_m) {
  MAX_CAPACITY_TREE_NURSERY=area_ha*10000/(spacing_m*spacing_m)+0.5;
  treeNursery.SetCapacity(MAX_CAPACITY_TREE_NURSERY);
}

unsigned long MAX_CAPACITY_FOREST = 1;
Store forest("Lesa (Kapacita)", MAX_CAPACITY_FOREST);
// Prevod
void SetMaxCapacityForest(double area_ha, double spacing_m) {
  MAX_CAPACITY_FOREST=area_ha*10000/(spacing_m*spacing_m)+0.5;
  forest.SetCapacity(MAX_CAPACITY_FOREST);
}

///////////////////////
// SIMULATION       //
/////////////////////

// Časová jednotka za jakou bude proveden/počítán přírůstek CO2, věk, atd...
double TIME_STEP = 1;
double TIME_END = 12*5;

////////////////////////
// CO2               //
//////////////////////

// CO2 vyprodukovane stromovou skolkou
double sumTreeNurseryCO2 = 0.0;
// CO2 z likvidace => pocet spatnych sazenic * konstanta

double sumYoungForestCO2 = 0;
double sumGrownForestCO2 = 0;
double sumOldForestCO2 = 0;

void ResetSumCO2() {
  sumTreeNurseryCO2 = 0;
  sumYoungForestCO2 = 0;
  sumGrownForestCO2 = 0;
  sumOldForestCO2 = 0;
}

//////////////////////////
// SEEDLING            //
////////////////////////

// Doba potrebná pro vypěstování sazenice (mesic,..)
double SEEDLING_GROW_TIME = 12*2;
// Doba po kterou jsou sazenice použitelné. Poté jsou přestárlé.
double SEEDLING_OVERGROW_TIME = 12*2;
// Poměr úspěšnosti vypěstování použitelné sazenice (%)
double RATION_SUCCESSFULLY_GROWN_SEEDLING = 0.8;

// Přírustek CO2 za starost o sazenice za časovy krok.
double CO2_INCREMENT_CARE_SEEDLING_PER_TIME_STEP = -0.0284625;

// Počet neuspěšne vypestovaných sazenic
unsigned long numberOfUnhealthySeedlings = 0;
// Počet pouzitych sazenic (v lese)
unsigned long numberOfUsedSeedlings = 0;
// Počet příliž přestárlých sazenic
unsigned long numberOfUnusedSeedlings = 0;

Queue GrownSeeding("Vyrostle sazenice");

void ResetSeedling() {
  numberOfUnhealthySeedlings = 0;
  numberOfUsedSeedlings = 0;
  numberOfUnusedSeedlings = 0;
  GrownSeeding.Clear();
}

////////////////////
// TREE          //
//////////////////

// Od sazenice po dospely strom.
// Doba dospívání. Počet časových kroků.
double TREE_LIFE_CYCLE_1_TIME_STEPS = 12*2;
// Pohlceni oxidu uhličitého za časový krok.
double TREE_LIFE_CYCLE_1_CO2_PER_TIME_STEP = 10;

// Od dospělého stromu po starý strom.
// Životnost dospelého stromu. Počet časových kroků.
double TREE_LIFE_CYCLE_2_TIME_STEPS = 12*2;
// Pohlceni oxidu uhličitého za časový krok.
double TREE_LIFE_CYCLE_2_CO2_PER_TIME_STEP = 10;

// Starý strom.
// Pohlceni oxidu uhličitého za časový krok.
double TREE_LIFE_CYCLE_3_CO2_PER_TIME_STEP = 10;
double RATION_TREE_MORTALITY = 0.1;

// Mladé stromy
Queue YoungForest("Mlady les");
// Dospělé stromy
Queue GrownForest("Vzrostly les");
// Staré stromy
Queue OldForest("Stary les");

Stat ageDeathTree("Stari mrtvych stromu");
Stat co2DeathTree("CO2 mrtvych stromu");

Stat ageUsedOldTree("Stari pouzitych/prodanych starych stromu");
Stat co2UsedOldTree("CO2 pouzitych/prodanych starych stromu");

Stat ageUsedGrownTree("Stari pouzitych/prodanych vzrostlych stromu");
Stat co2UsedGrownTree("CO2 pouzitych/prodanych vzrostlych stromu");

unsigned long numberOfUnhappyNeedTree = 0;

void ResetTree() {
  numberOfUnhappyNeedTree = 0;
  YoungForest.Clear();
  GrownForest.Clear();
  OldForest.Clear();
  ageDeathTree.Clear();
  co2DeathTree.Clear();
  ageUsedOldTree.Clear();
  co2UsedOldTree.Clear();
  ageUsedGrownTree.Clear();
  co2UsedGrownTree.Clear();
}

////////////////////
// NEED TREE     //
//////////////////

unsigned long TREE_PER_NEED_MIN = 0;
unsigned long TREE_PER_NEED_MAX = 0;
double TIME_TO_NEXT_TREE_NEED = 12;


//////////////////////////////////
// POMOCNÉ ENTYTY A FUNKCE     //
////////////////////////////////

namespace PrintOutput {

  void Forest(bool verbose) {

    borderer::PrintFirstLine();
    borderer::PrintString((char*)"Les [stromy]");
    borderer::PrintLine();
    borderer::PrintTwoValue(
      (char*)"Mlady", YoungForest.Length(),
      (char*)"Dospely", GrownForest.Length() );
    borderer::PrintTwoValue(
      (char*)"Stary", OldForest.Length(),
      (char*)"Celkem", YoungForest.Length()+
      GrownForest.Length()+OldForest.Length() );
    borderer::PrintLine();

    borderer::PrintString((char*)"Les CO2 [kgCO2]");
    borderer::PrintLine();
    borderer::PrintTwoValue(
      (char*)"Mlady", sumYoungForestCO2,
      (char*)"Dospely", sumGrownForestCO2 );
    borderer::PrintTwoValue(
      (char*)"Stary", sumOldForestCO2,
      (char*)"Celkem", sumForestCO2 );
    borderer::PrintLine();

    borderer::PrintString((char*)"Les C [kgC]");
    borderer::PrintLine();
    borderer::PrintTwoValue(
      (char*)"Mlady", sumYoungForestCO2/3.67,
      (char*)"Dospely", sumGrownForestCO2/3.67 );
    borderer::PrintTwoValue(
      (char*)"Stary", sumOldForestCO2/3.67,
      (char*)"Celkem", sumForestCO2/3.67 );
    borderer::PrintLastLine();

    if(verbose) {
      Print("\n");

      forest.Output();
      borderer::PrintLine();
      YoungForest.Output();
      GrownForest.Output();
      OldForest.Output();
      borderer::PrintLine();
      ageDeathTree.Output();
      co2DeathTree.Output();
      ageUsedOldTree.Output();
      co2UsedOldTree.Output();
      ageUsedGrownTree.Output();
      co2UsedGrownTree.Output();
    }

    Print("\n\n");
  }
  void Forest() {
    Forest(true);
  }
  void TreeNursery(bool verbose) {

    borderer::PrintFirstLine();
    borderer::PrintString((char*)"Stromova skolka [sazenice]");
    borderer::PrintLine();

    borderer::PrintTwoValue(
      (char*)"Pestovane", (unsigned int)(treeNursery.Used()-GrownSeeding.Length()),
      (char*)"Cekajici", GrownSeeding.Length() );
    borderer::PrintTwoValue(
      (char*)"Povedene", numberOfUsedSeedlings+numberOfUnusedSeedlings,
      (char*)"Nepovedene", numberOfUnhealthySeedlings );
    borderer::PrintTwoValue(
      (char*)"Pouzite", numberOfUsedSeedlings,
      (char*)"Nepouzite", numberOfUnusedSeedlings );
    borderer::PrintLine();

    borderer::PrintString((char*)"Stromova skolka CO2");
    borderer::PrintLine();
    borderer::PrintTwoValue(
      (char*)"kgCO2", sumTreeNurseryCO2,
      (char*)"kgC", sumTreeNurseryCO2/3.67 );
    borderer::PrintLastLine();

    if(verbose) {
      Print("\n");
      treeNursery.Output();
      GrownSeeding.Output();
    }
    Print("\n\n");
  }
  void TreeNursery() {
    TreeNursery(true);
  }


  void NeedTrees(bool verbose) {
    (void)verbose;
    borderer::PrintFirstLine();
    borderer::PrintString((char*)"Poptavka pro stromech");
    borderer::PrintLine();

    borderer::PrintTwoValue(
      (char*)"Vydanych vzrostlych", ageUsedGrownTree.Number(),
      (char*)"Vydanych starych", ageUsedOldTree.Number() );
    borderer::PrintTwoValue(
      (char*)"Vydanych celkem", (unsigned long)(
        ageUsedOldTree.Number() + ageUsedGrownTree.Number()
      ),
      (char*)"Nevydanych celkem", numberOfUnhappyNeedTree );

    if(co2DeathTree.Number()) {
      borderer::PrintTwoValue(
        (char*)"Mrtvych stomu", (double)ageDeathTree.Number(),
        (char*)"kgCO2 mrtvych", co2DeathTree.Number()*co2DeathTree.MeanValue() );
    } else {
      borderer::PrintTwoValue(
        (char*)"Mrtvych stomu", (double)ageDeathTree.Number(),
        (char*)"kgCO2 mrtvych", 0.0 );
    }
    borderer::PrintLastLine();

    Print("\n\n");
  }

}

