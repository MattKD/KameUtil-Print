#include "print.h"
#include <fstream>
#include <sstream>
#include <iomanip>

static bool error_found = false;
void logError_(bool expr, const char *msg, const char *file, int line)
{
  if (!expr) {
    std::cerr << "Error: " << msg << ", in file " << file 
      << ", line " << line << "\n";
    error_found = true;
  }
}

#define logError(expr) logError_(expr, #expr, __FILE__, __LINE__)

// Custom stream
struct StrStream {
  StrStream(std::string &s) : str{s} { }

  StrStream& operator<<(char c) 
  { 
    str.push_back(c); 
    return *this; 
  }

  StrStream& operator<<(const char *s) 
  { 
    str.append(s); 
    return *this; 
  }

  StrStream& operator<<(const std::string &s) 
  { 
    str.append(s); 
    return *this; 
  }

  StrStream& operator<<(int n) 
  { 
    const int tmp_size = 32;
    char tmp[tmp_size];
    snprintf(tmp, tmp_size, "%d", n);
    str.append(tmp);
    return *this;
  }

  StrStream& operator<<(double d)
  { 
    const int tmp_size = 2048;
    char tmp[tmp_size];
    snprintf(tmp, tmp_size, "%g", d);
    str.append(tmp);
    return *this;
  }

  std::string &str;
};

template <class ...Args>
std::string sprint(const char *fmt, const Args &...args)
{
  std::string str;
  StrStream ss(str);
  if (KameUtil::streamPrint(ss, fmt, args...)) {
    return str;
  }
  return std::string("");
}

int main()
{
  using KameUtil::print;
  using std::ostream;
  using std::cout;
  using std::string;

  // Using iomanip for formatting
  logError(print("Pi = {1}{0}, Pi = {2}{0}\n\n", 3.14159265,
    std::setprecision(2), std::setprecision(7)));

  const char *fmt = "Hello {1}, did you hear about {}? "
    "Go visit the {0} guild to learn more about it. "
    "Wow you have {3} gold! Go buy something too!\n\n";
  string name = "Grimlock";
  string role = "Warrior";
  const char *topic = "the attack";
  int gold = 100;

  std::ofstream fout("out.txt");
  std::stringstream ss;

  logError(print(fmt, role, name, topic, gold));
  logError(print(fout, fmt, role, name, topic, gold));
  logError(print(ss, fmt, role, name, topic, gold));

  string str = ss.str();
  ss.str("");
  cout << str;
  fout << str;

  const char *fmt2 = "{1} こんにちは、{} について知りますか？ "
    "それについての詳細を聞くために{0}ギルドを訪れる行きます。"
    "うわー、{} は {3} の金を持っています！ "
    "あまりにも何かを買いに行きます！\n\n";

  // prints garbage on Windows console, but works with mintty
  logError(print(fmt2, role, name, topic, gold));
  logError(print(fout, fmt2, role, name, topic, gold));
  logError(print(ss, fmt2, role, name, topic, gold));

  str = ss.str();
  ss.str("");
  cout << str;
  fout << str;

  // wchar_t tests
  {
    std::wofstream wfout("wout.txt");
    std::wstringstream wss;
    KameUtil::print(L"wprint test\n");
    KameUtil::print(wfout, L"wfprint test\n");
    KameUtil::print(wss, L"wsprint test\n");
    std::wstring wstr = wss.str();
    std::wcout << wstr << std::endl;
    wfout << wstr << std::endl;
  }

  // Test custom stream class with valid index input
  logError(sprint("{}", 0, 1) == "0"); // too many args is fine
  logError(sprint("foo {1} {} {0}", "bar", 123, "baz") 
    == "foo 123 baz bar");
  logError(sprint("{10}", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    "foo", 11) == "foo");
  logError(sprint("{{", 0, 1) == "{"); // "{{" escapes '{'
  logError(sprint("{{}", 0, 1) == "{}"); // "{{" escapes '{'

  // Test invalid index input
  logError(!print(ss, "{00}", 0, 1)); // no leading 0's
  logError(!print(ss, "{01}", 0, 1)); // no leading 0's
  logError(!print(ss, "{ }", 0, 1)); // no spaces allowed inside "{}"
  logError(!print(ss, "{1 }", 0, 1)); // no spaces allowed inside "{}"
  logError(!print(ss, "{0 0}", 0, 1)); // no spaces allowed inside "{}"
  logError(!print(ss, "{0 1}", 0, 1)); // no spaces allowed inside "{}"
  logError(!print(ss, "{1 0}", 0, 1)); // no spaces allowed inside "{}"
  logError(!print(ss, "{-1}", 0, 1)); // no negative positions 
  logError(!print(ss, "{a}", 0, 1)); // only digits inside "{}"
  logError(!print(ss, "{1a}", 0, 1)); // only digits inside "{}"
  logError(!print(ss, "{a1}", 0, 1)); // only digits inside "{}"
  logError(!print(ss, "{2}", 0, 1)); // invalid position

  if (!error_found) {
    cout << "All tests passed\n";
  }
  return 0;
}

