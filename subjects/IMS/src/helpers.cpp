
#include <simlib.h>
#include "global.hpp"
#include "helpers.hpp"

#include "tree.hpp"
#include "seedling.hpp"

namespace helpers {

  void AddTree(unsigned long howMany, Tree::States howType) {
    Tree *tree = nullptr;
    for(unsigned long i = 1; i<=howMany; i++) {
      if(i > forest.Free()) {
        return;
      }
      tree = new Tree(&forest);
      tree->state = howType;
      tree->Activate();
    }
  }

  void AddYoungTree(unsigned long howMany) {
    AddTree(howMany, Tree::YOUNG);
  }
  void AddGrownTree(unsigned long howMany) {
    AddTree(howMany, Tree::GROWN);
  }
  void AddOldTree(unsigned long howMany) {
    AddTree(howMany, Tree::OLD);
  }

  void AddSeedling(unsigned long howMany, Seedling::States howType) {
    Seedling *seedling = nullptr;
    for(unsigned long i = 1; i<=howMany; i++) {
      if(i > treeNursery.Free()) {
        return;
      }
      seedling = new Seedling(&treeNursery);
      seedling->state = howType;
      seedling->Activate();
    }
  }
  void AddYoungSeedling(unsigned long howMany) {
    AddSeedling(howMany, Seedling::YOUNG);
  }
  void AddGrownSeedling(unsigned long howMany) {
    AddSeedling(howMany, Seedling::GROWN);
  }
}
