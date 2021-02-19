#ifndef _PLANTING_TREES_HPP_
#define _PLANTING_TREES_HPP_

#include <simlib.h>
#include "global.hpp"

class PlantingTrees: public Process {
  public:
    void Behavior() override;
};

class PlantingTreesGenerator: public Event {
  public:
    void Behavior();
};

#endif
