#ifndef _TIMEOUT_HPP_
#define _TIMEOUT_HPP_

#include <simlib.h>

// new TreeTimeout(this, &Tree::Timeout)

template<class T>
class Timeout: public Event {
  public:
    Timeout(T *tree, void (T::*pClassCallback)());
    void Behavior() override;
    T *pClass;
    void (T::*pClassCallback)();
};


// Eliminace chyby neznameho symbolu.
class Tree;
template class Timeout<Tree>;
class Seedling;
template class Timeout<Seedling>;

#endif
