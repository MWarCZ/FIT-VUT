
#include <simlib.h>
#include "global.hpp"
#include "seedling.hpp"
#include "tree.hpp"
#include "plantingTrees.hpp"

void PlantingTrees::Behavior() {
  Seedling *seedling;
  while(forest.Free() && GrownSeeding.Length()>0) {
    seedling = (Seedling *)GrownSeeding.GetFirst();
    seedling->Cancel();
    numberOfUsedSeedlings++;
    // Enter(Forest);
    (new Tree(&forest))->Activate();
  }
}

void PlantingTreesGenerator::Behavior() {
  (new PlantingTrees())->Activate();
  Activate(Time+12);
}
