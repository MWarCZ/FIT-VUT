
#include <simlib.h>
#include "global.hpp"
#include "tree.hpp"
#include "timeout.hpp"


Tree::Tree(Store *insideStore):
  co2(0), cycles(0),
  state(Tree::YOUNG),
  treeLifeCycle(nullptr),
  insideStore(insideStore) {}
Tree::Tree(): Tree(nullptr) {}

Tree::~Tree() {
  // Print("T: %g -- %g -- %g\n", Time, EndTime, NextTime);
  if(Time == EndTime && NextTime > EndTime) { return; }
  if(insideStore != nullptr) {
    Leave(*insideStore);
  }
  if(treeLifeCycle != nullptr) {
    treeLifeCycle->Cancel();
  }
}
void Tree::Behavior() {
  // Print("ST: %g, %g\n", TIME_STEP, TREE_LIFE_CYCLE_1_TIME_STEPS);
  if(insideStore != nullptr) {
    Enter(*insideStore);
  }
  /////////////////
  // Young Tree //
  ///////////////
  treeLifeCycle = new Timeout<Tree>(this, &Tree::endLifeCycle1);
  // treeLifeCycle->Activate(Time+1);
  treeLifeCycle->Activate(Time+TREE_LIFE_CYCLE_1_TIME_STEPS);
  Into(YoungForest);
  while(this->state == Tree::YOUNG) {
    // Print("1) T: %g, C: %li, CO2: %g\n",Time,cycles,co2);
    Wait(TIME_STEP);
    this->cycles++;
    this->co2+=TREE_LIFE_CYCLE_1_CO2_PER_TIME_STEP;
    sumYoungForestCO2+=TREE_LIFE_CYCLE_1_CO2_PER_TIME_STEP;
  }
  Out(); // Opusti YoungForest
  treeLifeCycle->Cancel();
  treeLifeCycle = nullptr;
  // Print("%i: %li => %g\n", state, cycles, co2);
  /////////////////
  // Grown Tree //
  ///////////////
  treeLifeCycle = new Timeout<Tree>(this, &Tree::endLifeCycle2);
  // treeLifeCycle->Activate(Time+1);
  treeLifeCycle->Activate(Time+TREE_LIFE_CYCLE_2_TIME_STEPS);
  Into(GrownForest);

  while(this->state == Tree::GROWN) {
    // Print("2) T: %g, C: %li, CO2: %g\n",Time,cycles,co2);
    Wait(TIME_STEP);
    this->cycles++;
    this->co2+=TREE_LIFE_CYCLE_2_CO2_PER_TIME_STEP;
    sumGrownForestCO2+=TREE_LIFE_CYCLE_2_CO2_PER_TIME_STEP;
  }
  Out(); // Opusti GrownForest
  treeLifeCycle->Cancel();
  treeLifeCycle = nullptr;
  // Print("%i: %li => %g\n", state, cycles, co2);
  ///////////////
  // Old Tree //
  /////////////
  Into(OldForest);
  while(this->state == Tree::OLD) {
    // Print("3) T: %g, C: %li, CO2: %g\n",Time,cycles,co2);
    Wait(TIME_STEP);
    this->cycles++;
    this->co2+=TREE_LIFE_CYCLE_2_CO2_PER_TIME_STEP;
    sumOldForestCO2+=TREE_LIFE_CYCLE_2_CO2_PER_TIME_STEP;
    if(Random()<RATION_TREE_MORTALITY) {
      this->state = Tree::DEATH;
    }
  }
  Out(); // Opusti OldForest
  ageDeathTree(this->cycles);
  co2DeathTree(this->co2);
  // Print("END: %i: %li => %g\n", state, cycles, co2);

}
void Tree::endLifeCycle1() {
  this->state = Tree::GROWN;
}
void Tree::endLifeCycle2() {
  this->state = Tree::OLD;
}

