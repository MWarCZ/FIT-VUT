
#include <simlib.h>
#include "global.hpp"
#include "seedling.hpp"
#include "plantingTrees.hpp"
#include "needTrees.hpp"
#include "scenarios.hpp"
#include "borderer.hpp"

void SelectPreinit(char c) {
  switch(c) {
    case '1': scenarios::Preinit_Experiment_K1();
      break;
    case '2': scenarios::Preinit_Experiment_K2();
      break;
    case '3': scenarios::Preinit_Experiment_K3();
      break;
    case '4': scenarios::Preinit_Experiment_K4();
      break;
    case '5': scenarios::Preinit_Experiment_K5();
      break;
    case '6': scenarios::Preinit_Experiment_K6();
      break;
    case '7': scenarios::Preinit_Experiment_K7();
      break;
    case '8': scenarios::Preinit_Experiment_K8();
      break;
    case '9': scenarios::Preinit_Experiment_K9();
      break;
    case 'a':
    case 'A': scenarios::Preinit_Experiment_A();
      break;
    case 'b':
    case 'B': scenarios::Preinit_Experiment_B();
      break;
    case 'c':
    case 'C': scenarios::Preinit_Experiment_C();
      break;
    case 'd':
    case 'D': scenarios::Preinit_Experiment_D();
      break;
    case 'e':
    case 'E': scenarios::Preinit_Experiment_E();
      break;
    case 'f':
    case 'F': scenarios::Preinit_Experiment_F();
      break;
    case 'g':
    case 'G': scenarios::Preinit_Experiment_G();
      break;
    case 'h':
    case 'H': scenarios::Preinit_Experiment_H();
      break;
    case 'i':
    case 'I': scenarios::Preinit_Experiment_I();
      break;
    case 'j':
    case 'J': scenarios::Preinit_Experiment_J();
      break;
    case 'k':
    case 'K': scenarios::Preinit_Experiment_K();
      break;
    case 'l':
    case 'L': scenarios::Preinit_Experiment_L();
      break;
    case 'm':
    case 'M': scenarios::Preinit_Experiment_M();
      break;
    default: scenarios::Preinit_Experiment_K1();
  }
}
void SelectPrerun(char c) {
  switch(c) {
    case '1': scenarios::Prerun_Experiment_K1();
      break;
    case '2': scenarios::Prerun_Experiment_K2();
      break;
    case '3': scenarios::Prerun_Experiment_K3();
      break;
    case '4': scenarios::Prerun_Experiment_K4();
      break;
    case '5': scenarios::Prerun_Experiment_K5();
      break;
    case '6': scenarios::Prerun_Experiment_K6();
      break;
    case '7': scenarios::Prerun_Experiment_K7();
      break;
    case '8': scenarios::Prerun_Experiment_K8();
      break;
    case '9': scenarios::Prerun_Experiment_K9();
      break;
    case 'a':
    case 'A': scenarios::Prerun_Experiment_A();
      break;
    case 'b':
    case 'B': scenarios::Prerun_Experiment_B();
      break;
    case 'c':
    case 'C': scenarios::Prerun_Experiment_C();
      break;
    case 'd':
    case 'D': scenarios::Prerun_Experiment_D();
      break;
    case 'e':
    case 'E': scenarios::Prerun_Experiment_E();
      break;
    case 'f':
    case 'F': scenarios::Prerun_Experiment_F();
      break;
    case 'g':
    case 'G': scenarios::Prerun_Experiment_G();
      break;
    case 'h':
    case 'H': scenarios::Prerun_Experiment_H();
      break;
    case 'i':
    case 'I': scenarios::Prerun_Experiment_I();
      break;
    case 'j':
    case 'J': scenarios::Prerun_Experiment_J();
      break;
    case 'k':
    case 'K': scenarios::Prerun_Experiment_K();
      break;
    case 'l':
    case 'L': scenarios::Prerun_Experiment_L();
      break;
    case 'm':
    case 'M': scenarios::Prerun_Experiment_M();
      break;
    default: scenarios::Prerun_Experiment_K1();
  }
}

int main(int argc, char *argv[]) {
  char selected = '0';
  bool shortLog = false;
  for(int i = 1; i<argc; i++) {
    if(argv[i][0]=='-'){
      shortLog = argv[i][1]=='s';
    } else {
      selected = argv[i][0];
    }
  }
  // if(argc>1) {
  //   selected = argv[1][0];
  // }
  Print("START: %c\n", selected);

  SelectPreinit(selected);
  Init(0,TIME_END);
  SelectPrerun(selected);
  (new PlantingSeedlingsGenerator())->Activate();
  (new PlantingTreesGenerator())->Activate();
  (new NeedTrees())->Activate();
  Run();

  //(new Gener)->Activate();
  //(new ProcessX)->Activate();
  //(new GeneratorSeedling)->Activate();

  // (new Tree())->Activate();
  // for(int i = 0; i<10; i++) {
  //   (new Tree())->Activate();
  // }
  // (new NeedTrees())->Activate();
  // for(int i = 0; i< 50; i++) {
  //   Print("%3i : %li\n",i,(long)Normal(6, 1.8));
  // }
  // (new PlantingSeedling())->Activate();
  // Run();

  // Statistiky
  Print("STATISTIKY:\n");
  Print("Doba simulace: %g mesicu = %g let\n\n",
    TIME_END, TIME_END/12 );
  PrintOutput::Forest(!shortLog);
  PrintOutput::TreeNursery(!shortLog);
  PrintOutput::NeedTrees(!shortLog);

  Print("KONEC!\n");
  return 0;
}

