#ifndef _TREE_HPP_
#define _TREE_HPP_

#include <simlib.h>
#include "global.hpp"
#include "timeout.hpp"


class Tree: public Process {
  public:
    Tree();
    Tree(Store *insideStore);
    ~Tree();
    void Behavior() override;
    void endLifeCycle1();
    void endLifeCycle2();
    enum States {
      YOUNG = 0,
      GROWN = 1,
      OLD = 2,
      DEATH = 3
    };
    double co2;
    unsigned long cycles;
    States state;
    Timeout<Tree> *treeLifeCycle;
    Store *insideStore;
};

#endif
