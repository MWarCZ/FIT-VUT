
#include <simlib.h>
#include "global.hpp"
#include "needTrees.hpp"
#include "tree.hpp"

void NeedTree::Behavior() {
  Tree *tree = nullptr;
  if(OldForest.Length()) {
    tree = (Tree *)OldForest.GetFirst();
    // Print("C: %li, D: %g\n",tree->cycles,tree->co2);
    // Sber statistyk ...
    ageUsedOldTree(tree->cycles);
    co2UsedOldTree(tree->co2);
    tree->Cancel();
  } else if(GrownForest.Length()) {
    tree = (Tree *)GrownForest.GetFirst();
    // Print("C: %li, D: %g\n",tree->cycles,tree->co2);
    // Sber statistyk ...
    ageUsedGrownTree(tree->cycles);
    co2UsedGrownTree(tree->co2);
    tree->Cancel();
  } else {
    // Sber statistyk ...
    numberOfUnhappyNeedTree++;
  }
}

NeedTrees::NeedTrees(): Event() {}
void NeedTrees::Behavior() {
  // double next = Normal(6, 1.8);
  // double next = Uniform(1,12);
  double next = TIME_TO_NEXT_TREE_NEED;

  // next = (next<0)?-next:next;
  // Print("\t%g : %g\n",Time,next);
  Activate(Time+next);
  unsigned long howMany = 0;
  if(TREE_PER_NEED_MIN < TREE_PER_NEED_MAX) {
    howMany = Uniform(TREE_PER_NEED_MIN,TREE_PER_NEED_MAX);
  } else if (TREE_PER_NEED_MIN == TREE_PER_NEED_MAX) {
    howMany = TREE_PER_NEED_MAX;
  }
  for(unsigned long i=0; i<howMany; i++) {
    (new NeedTree())->Activate();
  }
}
