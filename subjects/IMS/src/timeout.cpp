
#include <simlib.h>
#include "timeout.hpp"

template <class T>
Timeout<T>::Timeout(T *pClass, void (T::*pClassCallback)() ):
  Event(), pClass(pClass),
  pClassCallback(pClassCallback)
{ }

template <class T>
void Timeout<T>::Behavior() {
  (pClass->*pClassCallback)();
}
