#ifndef _GLOBAL_HPP_
#define _GLOBAL_HPP_

#include <simlib.h>

// Pokusy
extern Store SkladX;


////////////////////////////
// STORE & FACILITY      //
//////////////////////////

extern unsigned long MAX_CAPACITY_TREE_NURSERY;
extern Store treeNursery;
void SetMaxCapacityTreeNursery(double area_ha, double spacing_m);

extern unsigned long MAX_CAPACITY_FOREST;
extern Store forest;
void SetMaxCapacityForest(double area_ha, double spacing_m);


///////////////////////
// SIMULATION       //
/////////////////////

// Časová jednotka za jakou bude proveden/počítán přírůstek CO2, věk, atd...
extern double TIME_STEP;

extern double TIME_END;

////////////////////////
// CO2               //
//////////////////////

// CO2 vyprodukovane stromovou skolkou
extern double sumTreeNurseryCO2;
// CO2 z likvidace => pocet spatnych sazenic * konstanta

extern double sumYoungForestCO2;
extern double sumGrownForestCO2;
extern double sumOldForestCO2;
#define sumForestCO2 (sumYoungForestCO2+sumGrownForestCO2+sumOldForestCO2)

void ResetSumCO2();

//////////////////////////
// SEEDLING            //
////////////////////////

// Doba potrebná pro vypěstování sazenice (roky)
extern double SEEDLING_GROW_TIME;
// Doba po kterou jsou sazenice použitelné. Poté jsou přestárlé.
extern double SEEDLING_OVERGROW_TIME;
// Poměr úspěšnosti vypěstování použitelné sazenice (%)
extern double RATION_SUCCESSFULLY_GROWN_SEEDLING;

// Přírustek CO2 za starost o sazenice za časovy krok.
extern double CO2_INCREMENT_CARE_SEEDLING_PER_TIME_STEP;

// Počet neuspěšne vypestovaných sazenic
extern unsigned long numberOfUnhealthySeedlings;
// Počet pouzitych sazenic (v lese)
extern unsigned long numberOfUsedSeedlings;
// Počet příliž přestárlých sazenic
extern unsigned long numberOfUnusedSeedlings;

extern Queue GrownSeeding;

void ResetSeedling();

////////////////////
// TREE          //
//////////////////

// Od sazenice po dospely strom.
// Doba dospívání. Počet časových kroků.
extern double TREE_LIFE_CYCLE_1_TIME_STEPS;
// Pohlceni oxidu uhličitého za časový krok.
extern double TREE_LIFE_CYCLE_1_CO2_PER_TIME_STEP;

// Od dospělého stromu po starý strom.
// Životnost dospelého stromu. Počet časových kroků.
extern double TREE_LIFE_CYCLE_2_TIME_STEPS;
// Pohlceni oxidu uhličitého za časový krok.
extern double TREE_LIFE_CYCLE_2_CO2_PER_TIME_STEP;

// Starý strom.
// Pohlceni oxidu uhličitého za časový krok.
extern double TREE_LIFE_CYCLE_3_CO2_PER_TIME_STEP;

extern double RATION_TREE_MORTALITY;

// Mladé stromy
extern Queue YoungForest;
// Dospělé stromy
extern Queue GrownForest;
// Staré stromy
extern Queue OldForest;

extern Stat ageDeathTree;
extern Stat co2DeathTree;

extern Stat ageUsedOldTree;
extern Stat co2UsedOldTree;

extern Stat ageUsedGrownTree;
extern Stat co2UsedGrownTree;

extern unsigned long numberOfUnhappyNeedTree;

void ResetTree();

////////////////////
// NEED TREE     //
//////////////////

extern unsigned long TREE_PER_NEED_MIN;
extern unsigned long TREE_PER_NEED_MAX;
extern double TIME_TO_NEXT_TREE_NEED;

//////////////////////////////////
// POMOCNÉ ENTiTY A FUNKCE     //
////////////////////////////////


namespace PrintOutput {
  void Forest(bool verbose);
  void Forest();
  void TreeNursery(bool verbose);
  void TreeNursery();

  void NeedTrees(bool verbose);
}

#endif
