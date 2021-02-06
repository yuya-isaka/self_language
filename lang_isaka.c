#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

static char *p;
static char func[26][100];

__attribute__((noreturn)) static void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

static void skip() {
  while (isspace(*p))
    p++;
}

static void read_until(char until, char *function) {
  for(; *p != until; p++, function++){
    *function = *p;
  }
  p++;
  *function = '\0';
}

static void expect(char c) {
  if (*p != c) {
    error("invalid expected  %c", p);
    exit(1);
  }
  p++;
}

static int eval(int *arg);

static int eval_string(char *function, int *newval) {
  char *orig = p;
  p = function;
  int val;
  val = eval(newval);
  p = orig;
  return val;
}

static int eval(int *arg) {
  skip();
  
  if ('P' == *p) {
    p++;
    expect('(');
    int val = eval(arg);
    expect(')');
    printf("%d\n", val);
    while (*p)
      val = eval(arg);
    return val;
  }
  

  if ('a' <= *p && *p <= 'z') {
    return arg[*p++ - 'a'];
  }

  if ('A' <= *p && *p <= 'Z' && p[1] == '[') {
    char code = *p;
    p += 2;
    read_until(']', func[code - 'A']);
    return eval(arg);
  }
  
  if ('A' <= *p && *p <= 'Z' && p[1] == '(') {
    char code = *p;
    p += 2;
    int newval[26];
    int i = 0;
    while (*p != ')')
      newval[i++] = eval(arg);
    expect(')');
    return eval_string(func[code - 'A'], newval);
  }

  
  if (isdigit(*p)) {
    int val = *p++ - '0';
    while (isdigit(*p))
      val = val * 10 + (*p++ - '0');
    return val;
  }

  if (strchr("+-*/", *p)) {
    int op = *p++;
    int x = eval(arg);
    int y = eval(arg);
    switch (op) {
    case '+': return x + y;
    case '-': return x - y;
    case '*': return x * y;
    case '/': return x / y;
    }
  }

  error("invalid character: %c", p);
}

int main(int argc, char **argv) {
  p = argv[1];
  while (*p)
    printf("%d\n", eval(0));
  return 0;
}
