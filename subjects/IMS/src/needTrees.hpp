#ifndef _NEED_TREES_HPP_
#define _NEED_TREES_HPP_

#include <simlib.h>
#include "global.hpp"

class NeedTree: public Process {
  public:
    void Behavior() override;
};
class NeedTrees: public Event {
  public:
    NeedTrees();
    void Behavior() override;
};

#endif
