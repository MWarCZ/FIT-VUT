
#include <simlib.h>
#include "global.hpp"
#include "timeout.hpp"
#include "seedling.hpp"

Seedling::Seedling(Store *insideStore):
  co2(0), cycles(0),
  state(Seedling::YOUNG),
  seedlingLifeCycle(nullptr),
  insideStore(insideStore) {}
Seedling::Seedling(): Seedling(nullptr) {}

Seedling::~Seedling() {
  // Print("\tS ~D\n");
  // Print("\t%g : %g : %g > %g\n", StartTime, Time, EndTime, NextTime);
  if(Time == EndTime && NextTime > EndTime) { return; }
  if(insideStore != nullptr) {
    Leave(*insideStore);
  }
  if(seedlingLifeCycle != nullptr) {
    seedlingLifeCycle->Cancel();
  }
}

void Seedling::Behavior() {
  if(insideStore != nullptr) {
    Enter(*insideStore);
  }
  // Print("\tS Start\n");
  seedlingLifeCycle = new Timeout<Seedling>(this, &Seedling::endGrowing);
  seedlingLifeCycle->Activate(Time+SEEDLING_GROW_TIME);
  while(this->state == Seedling::YOUNG) {
    Wait(TIME_STEP);
    this->cycles++;
    this->co2+=CO2_INCREMENT_CARE_SEEDLING_PER_TIME_STEP;
    sumTreeNurseryCO2+=CO2_INCREMENT_CARE_SEEDLING_PER_TIME_STEP;
  }
  seedlingLifeCycle->Cancel();
  seedlingLifeCycle = nullptr;
  // Print("\tS Grown\n");

  if(Random()>RATION_SUCCESSFULLY_GROWN_SEEDLING) {
    this->state = Seedling::DEATH;
    numberOfUnhealthySeedlings++;
    // Print(">U S Unhealty\n");
    return;
  }
  // Print(">H S Healty\n");

  seedlingLifeCycle = new Timeout<Seedling>(this, &Seedling::endOvergrowing);
  seedlingLifeCycle->Activate(Time+SEEDLING_OVERGROW_TIME);
  Into(GrownSeeding);
  while(this->state == Seedling::GROWN) {
    Wait(TIME_STEP);
    this->cycles++;
    this->co2+=CO2_INCREMENT_CARE_SEEDLING_PER_TIME_STEP;
    sumTreeNurseryCO2+=CO2_INCREMENT_CARE_SEEDLING_PER_TIME_STEP;
  }
  Out(); // Opusti GrownSeedingNursery
  numberOfUnusedSeedlings++;
  // Print("\tS Old\n");
  this->state = Seedling::DEATH;

}
void Seedling::endGrowing() {
  this->state = Seedling::GROWN;
}
void Seedling::endOvergrowing() {
  this->state = Seedling::OLD;
}


void PlantingSeedlings::Behavior() {
  // Print("Free %lu\n", treeNursery.Free());
  // return;
  for(unsigned long i = 0; i<treeNursery.Free(); i++) {
    (new Seedling(&treeNursery))->Activate();
  }
}

void PlantingSeedlingsGenerator::Behavior() {
  (new PlantingSeedlings())->Activate();
  Activate(Time+12/2);
}

