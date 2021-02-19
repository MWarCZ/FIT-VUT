
#include <simlib.h>
#include <cstdio>
// sprintf()

namespace borderer {
  char vertical = '-';
  char horizontal = '|';
  char cornerTL = '+';
  char cornerTR = '+';
  char cornerBL = '+';
  char cornerBR = '+';


  void Line(char left, char right, char center) {
    char s[100];
    for(unsigned i=0; i<60; i++) {
      s[i] = center;
    }
    s[0]=left;
    s[60-1]=right;
    s[60]='\0';

    Print("%s\n",s);
  }
  void Line(char left, char right, char* center) {
    Print("%c %-56s %c\n",left, center, right);
  }

  void PrintLine() {
    Line(horizontal,horizontal,vertical);
  }
  void PrintFirstLine() {
    Line(cornerTL,cornerTR,vertical);
  }
  void PrintLastLine() {
    Line(cornerBL,cornerBR,vertical);
  }
  void PrintString(char *string) {
    Line(horizontal,horizontal,string);
  }
  void PrintTwoString(char *string1, char *string2) {
    char string[100];
    sprintf(string, "%-27s  %-27s", string1, string2);
    Line(horizontal,horizontal,string);
  }
  void PrintTwoValue(char* name1, double value1, char* name2, double value2) {
    char s1[50], s2[50];
    sprintf(s1, "%s: %g", name1, value1);
    sprintf(s2, "%s: %g", name2, value2);
    PrintTwoString(s1,s2);
  }
  void PrintTwoValue(char* name1, unsigned long value1, char* name2, unsigned long value2) {
    char s1[50], s2[50];
    sprintf(s1, "%s: %lu", name1, value1);
    sprintf(s2, "%s: %lu", name2, value2);
    PrintTwoString(s1,s2);
  }
  void PrintTwoValue(char* name1, long value1, char* name2, long value2) {
    char s1[50], s2[50];
    sprintf(s1, "%s: %li", name1, value1);
    sprintf(s2, "%s: %li", name2, value2);
    PrintTwoString(s1,s2);
  }
  void PrintTwoValue(char* name1, int value1, char* name2, int value2) {
    PrintTwoValue(name1, (long)value1, name2, (long)value2);
  }
  void PrintTwoValue(char* name1,unsigned int value1, char* name2,unsigned int value2) {
    PrintTwoValue(name1, (unsigned long)value1, name2, (unsigned long)value2);
  }
  void PrintTwoValue(char* name1, char* value1, char* name2, char* value2) {
    char s1[50], s2[50];
    sprintf(s1, "%s: %s", name1, value1);
    sprintf(s2, "%s: %s", name2, value2);
    PrintTwoString(s1,s2);
  }

}
