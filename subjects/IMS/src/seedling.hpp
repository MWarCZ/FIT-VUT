#ifndef _SEEDLING_HPP_
#define _SEEDLING_HPP_

#include <simlib.h>
#include "global.hpp"
#include "timeout.hpp"

// Sazenice ve stromové školce.
class Seedling: public Process {
  public:
    Seedling();
    Seedling(Store *insideStore);
    ~Seedling();
    void Behavior() override;
    void endGrowing();
    void endOvergrowing();
    enum States {
      YOUNG = 0,
      GROWN = 1,
      OLD = 2,
      DEATH = 3
    };
    double co2;
    unsigned long cycles;
    States state;
    Timeout<Seedling> *seedlingLifeCycle;
    Store *insideStore;
};

// Generátor nových sazenic.
// Pokud je volná kapacita školky, tak nech pěstovat další sazenici.
class PlantingSeedlings: public Process {
  public:
    void Behavior() override;
};

class PlantingSeedlingsGenerator: public Event {
  public:
    void Behavior();
};

#endif
