#ifndef _HELPERS_HPP_
#define _HELPERS_HPP_

#include "seedling.hpp"
#include "tree.hpp"

namespace helpers {
  void AddTree(unsigned long howMany, Tree::States howType);
  void AddYoungTree(unsigned long howMany);
  void AddGrownTree(unsigned long howMany);
  void AddOldTree(unsigned long howMany);
  void AddSeedling(unsigned long howMany, Seedling::States howType);
  void AddYoungSeedling(unsigned long howMany);
  void AddGrownSeedling(unsigned long howMany);
}

#endif
