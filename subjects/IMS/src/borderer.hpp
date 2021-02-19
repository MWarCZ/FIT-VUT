#ifndef _BORDERER_HPP_
#define _BORDERER_HPP_
/**
 * Formatovane vypisy.
 */
namespace borderer {

  extern char vertical;
  extern char horizontal;
  extern char cornerTL;
  extern char cornerTR;
  extern char cornerBL;
  extern char cornerBR;

  void Line(char left, char right, char center);
  void Line(char left, char right, char* center);
  void PrintFirstLine();
  void PrintLastLine();
  void PrintLine();
  void PrintString(char *string);
  void PrintTwoString(char *string1, char *string2);
  void PrintTwoValue(char* name1, double value1,
    char* name2, double value2);
  void PrintTwoValue(char* name1, unsigned long value1,
    char* name2, unsigned long value2);
  void PrintTwoValue(char* name1, long value1,
    char* name2, long value2);
  void PrintTwoValue(char* name1, int value1,
    char* name2, int value2);
  void PrintTwoValue(char* name1, unsigned int value1,
    char* name2, unsigned int value2);
  void PrintTwoValue(char* name1, char* value1,
    char* name2, char* value2);


}

#endif
