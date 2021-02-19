
#include <simlib.h>

#include "global.hpp"
#include "scenarios.hpp"
#include "helpers.hpp"

namespace scenarios {

  void ResetSimulation() {
    ResetSumCO2();
    ResetSeedling();
    ResetTree();
  }

  void SetTreeWildCherry() {
    // 45 let => 5 -- 40
    TREE_LIFE_CYCLE_1_TIME_STEPS = 12.0*5;
    TREE_LIFE_CYCLE_1_CO2_PER_TIME_STEP = 10.466;
    TREE_LIFE_CYCLE_2_TIME_STEPS = 12.0*40;
    TREE_LIFE_CYCLE_2_CO2_PER_TIME_STEP = 10.466;
    TREE_LIFE_CYCLE_3_CO2_PER_TIME_STEP = 10.466;
    RATION_TREE_MORTALITY = 0.05;
  }
  void SetTreeOak() {
    // 100 let => 10 -- 90
    TREE_LIFE_CYCLE_1_TIME_STEPS = 12.0*10;
    TREE_LIFE_CYCLE_1_CO2_PER_TIME_STEP = 8.747;
    TREE_LIFE_CYCLE_2_TIME_STEPS = 12.0*90;
    TREE_LIFE_CYCLE_2_CO2_PER_TIME_STEP = 8.747;
    TREE_LIFE_CYCLE_3_CO2_PER_TIME_STEP = 8.747;
    RATION_TREE_MORTALITY = 0.05;
  }
  void SetTreePoplar() {
    // 25 let => 3 -- 22
    TREE_LIFE_CYCLE_1_TIME_STEPS = 12.0*3;
    TREE_LIFE_CYCLE_1_CO2_PER_TIME_STEP = 8.074;
    TREE_LIFE_CYCLE_2_TIME_STEPS = 12.0*22;
    TREE_LIFE_CYCLE_2_CO2_PER_TIME_STEP = 8.074;
    TREE_LIFE_CYCLE_3_CO2_PER_TIME_STEP = 8.074;
    RATION_TREE_MORTALITY = 0.05;
  }
  void SetNuresery() {
    // Roztec 0.69 m
    CO2_INCREMENT_CARE_SEEDLING_PER_TIME_STEP = -0.0284625;

    SEEDLING_GROW_TIME = 12.0*2;
    SEEDLING_OVERGROW_TIME = 12.0*4;
    RATION_SUCCESSFULLY_GROWN_SEEDLING = 0.8;
  }
  void SetNeedTrees_Zero() {
    TREE_PER_NEED_MIN = 0;
    TREE_PER_NEED_MAX = 0;
    TIME_TO_NEXT_TREE_NEED = 12;
  }

  void Preinit_Experiment_K1() {
    ResetSimulation();
    TIME_END = 12;
    SetMaxCapacityTreeNursery(0.0, 1);
    SetMaxCapacityForest(0.35, 10.4);
    TIME_STEP = 1;
    SetTreeWildCherry();
    SetNuresery();
    SetNeedTrees_Zero();
  }
  void Prerun_Experiment_K1() {
    helpers::AddGrownTree(MAX_CAPACITY_FOREST);
  }

  void Preinit_Experiment_K2() {
    ResetSimulation();
    TIME_END = 12;
    SetMaxCapacityTreeNursery(0.0, 1);
    SetMaxCapacityForest(0.6, 12.6);
    TIME_STEP = 1;
    SetTreeOak();
    SetNuresery();
    SetNeedTrees_Zero();
  }
  void Prerun_Experiment_K2() {
    helpers::AddGrownTree(MAX_CAPACITY_FOREST);
  }

  void Preinit_Experiment_K3() {
    ResetSimulation();
    TIME_END = 12;
    SetMaxCapacityTreeNursery(0.0, 1);
    SetMaxCapacityForest(0.27, 8.0);
    TIME_STEP = 1;
    SetTreePoplar();
    SetNuresery();
    SetNeedTrees_Zero();
  }
  void Prerun_Experiment_K3() {
    helpers::AddGrownTree(MAX_CAPACITY_FOREST);
  }

  void Preinit_Experiment_K4() {
    Preinit_Experiment_K1();
    TIME_END = 12*10;
  }
  void Prerun_Experiment_K4() {
    Prerun_Experiment_K1();
  }

  void Preinit_Experiment_K5() {
    Preinit_Experiment_K2();
    TIME_END = 12*10;
  }
  void Prerun_Experiment_K5() {
    Prerun_Experiment_K2();
  }

  void Preinit_Experiment_K6() {
    Preinit_Experiment_K3();
    TIME_END = 12*10;
  }
  void Prerun_Experiment_K6() {
    Prerun_Experiment_K3();
  }


  void Preinit_Experiment_K7() {
    ResetSimulation();
    TIME_END = 12*45;
    SetMaxCapacityTreeNursery(0.0, 1);
    SetMaxCapacityForest(0.1, 10.0);
    TIME_STEP = 1;
    SetTreeWildCherry();
    SetNuresery();
    SetNeedTrees_Zero();
  }
  void Prerun_Experiment_K7() {
    helpers::AddYoungTree(MAX_CAPACITY_FOREST);
  }

  void Preinit_Experiment_K8() {
    ResetSimulation();
    TIME_END = 12*100;
    SetMaxCapacityTreeNursery(0.0, 1);
    SetMaxCapacityForest(0.1, 10.0);
    TIME_STEP = 1;
    SetTreeOak();
    SetNuresery();
    SetNeedTrees_Zero();
  }
  void Prerun_Experiment_K8() {
    helpers::AddYoungTree(MAX_CAPACITY_FOREST);
  }

  void Preinit_Experiment_K9() {
    ResetSimulation();
    TIME_END = 12*25;
    SetMaxCapacityTreeNursery(0.0, 1);
    SetMaxCapacityForest(0.1, 10.0);
    TIME_STEP = 1;
    SetTreePoplar();
    SetNuresery();
    SetNeedTrees_Zero();
  }
  void Prerun_Experiment_K9() {
    helpers::AddYoungTree(MAX_CAPACITY_FOREST);
  }


  void Preinit_Experiment_NeedTree(int tree, unsigned long max) {
    ResetSimulation();
    TIME_END = 12*300;

    if(tree == 1) {
      SetMaxCapacityForest(1.0, 10.4);
      SetMaxCapacityTreeNursery(0.025, 5.2);
      SetTreeWildCherry();
    }
    else if(tree == 2) {
      SetMaxCapacityForest(1.0, 12.6);
      SetMaxCapacityTreeNursery(0.025, 6.3);
      SetTreeOak();
    }
    else {
      SetMaxCapacityForest(1.0, 8);
      SetMaxCapacityTreeNursery(0.025, 4);
      SetTreePoplar();
    }

    TIME_STEP = 1;

    SetNuresery();
    // SetNeedTrees_Zero();
    TREE_PER_NEED_MIN = 1;
    TREE_PER_NEED_MAX = max;
    TIME_TO_NEXT_TREE_NEED = 12;
  }
  // Tresen
  void Preinit_Experiment_A() {
    Preinit_Experiment_NeedTree(1,2);
  }
  void Prerun_Experiment_A() {
    helpers::AddYoungTree(MAX_CAPACITY_FOREST/2);
  }
  void Preinit_Experiment_B() {
    Preinit_Experiment_NeedTree(1,3);
  }
  void Prerun_Experiment_B() {
    Prerun_Experiment_A();
  }
  void Preinit_Experiment_C() {
    Preinit_Experiment_NeedTree(1,4);
  }
  void Prerun_Experiment_C() {
    Prerun_Experiment_A();
  }
  void Preinit_Experiment_D() {
    Preinit_Experiment_NeedTree(1,5);
  }
  void Prerun_Experiment_D() {
    Prerun_Experiment_A();
  }
  void Preinit_Experiment_E() {
    Preinit_Experiment_NeedTree(1,6);
  }
  void Prerun_Experiment_E() {
    Prerun_Experiment_A();
  }
  void Preinit_Experiment_F() {
    Preinit_Experiment_NeedTree(1,7);
  }
  void Prerun_Experiment_F() {
    Prerun_Experiment_A();
  }

  // dub
  void Preinit_Experiment_G() {
    Preinit_Experiment_NeedTree(2,0);
  }
  void Prerun_Experiment_G() {
    Prerun_Experiment_A();
  }
  void Preinit_Experiment_H() {
    Preinit_Experiment_NeedTree(2,1);
  }
  void Prerun_Experiment_H() {
    Prerun_Experiment_A();
  }
  void Preinit_Experiment_I() {
    Preinit_Experiment_NeedTree(2,2);
  }
  void Prerun_Experiment_I() {
    Prerun_Experiment_A();
  }

  // topol
  void Preinit_Experiment_J() {
    Preinit_Experiment_NeedTree(3,8);
  }
  void Prerun_Experiment_J() {
    Prerun_Experiment_A();
  }
  void Preinit_Experiment_K() {
    Preinit_Experiment_NeedTree(3,10);
  }
  void Prerun_Experiment_K() {
    Prerun_Experiment_A();
  }
  void Preinit_Experiment_L() {
    Preinit_Experiment_NeedTree(3,12);
  }
  void Prerun_Experiment_L() {
    Prerun_Experiment_A();
  }
  void Preinit_Experiment_M() {
    Preinit_Experiment_NeedTree(3,14);
  }
  void Prerun_Experiment_M() {
    Prerun_Experiment_A();
  }
}
