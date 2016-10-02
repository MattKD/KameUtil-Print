# KameUtil-Print
C++11 template print functions with position arguments

Basic usage:
```
#include "print.h"

int main()
{
  //prints: "Hello world! I'm Matt. How's the weather?\n"
  print("Hello {1}! I'm {0}. How's the {2}?\n", "Matt", "world", "weather");
  return 0;
}
```
---
The position specifier must be in the format "{[IDX]}". IDX must be in range 0-9999, and cannot cannot contain spaces or leading 0's. If "{}" is given without an index then it is 1 plus the last given or 0 if it's the first. For example: 
```
"{} {} {}" = "{0} {1} {2}"
"{0} {} {}" = "{0} {1} {2}"
"{1} {} {}" = "{1} {2} {3}"
"{1} {0} {}" = "{1} {0} {1}"
```
---
There is no printf style formatting, but iostream style iomanip can be used as position args:
```
printf("Pi = {1}{0}\n", 3.14159265, std::setprecision(8));
```
---
Functions:
```
stream_print.h:
template <class Ostream, class CharT, class ...Args>
bool streamPrint(OStream &os, const CharT *fmt, const Args &...args);

print.h:
template <class ...Args>
bool print(const char *fmt, const Args &...args);

template <class ...Args>
bool print(const wchar_t *fmt, const Args &...args);

template <class ...Args>
bool print(std::ostream &os, const char *fmt, const Args &...args);

template <class ...Args>
bool print(std::wostream &os, const wchar_t *fmt, const Args &...args);
```
print without a passed std::ostream uses std::cout or std::wcout. 
Custom stream classes with an overloaded operator<< can be used with streamPrint.

---
See print_test.cpp for more examples, and https://github.com/MattKD/cpp_print_tests for performance comparisons.
