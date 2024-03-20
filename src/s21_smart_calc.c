#include "s21_smart_calc.h"

double smart_calc(char *string1) {
  char line[300] = {0};
  s21_correct_string(string1, line);
  int errors = s21_correct_num_act(line);
  if (errors == 0) {
    errors = s21_verifying(line);
  }
  double result;
  if (errors == 0) {
    result = s21_pars(line);
  } else {
    result = NAN;
  }
  return result;
}

void s21_correct_string(char *str, char *cstr) {
  int n = s21_len(str, 61);
  int i = 0;
  int j = 0;
  while (i < n) {
    if (i == 0 && str[i] == 45) {
      char *cch = strchr(FUNCTIONS, str[1]);
      if (cch != NULL || str[1] == 40) {
        cstr[0] = 40;
        cstr[1] = 48;
        cstr[2] = 45;
        cstr[3] = 49;
        cstr[4] = 41;
        cstr[5] = 42;
        j = 6;
        i = 1;
      } else {
        cstr[0] = str[0];
        i++;
        j++;
      }
    } else if (str[i] == ' ' ||
               ((i == 0 || str[i - 1] == 40) && str[i] == 43)) {
      i++;
    } else {
      cstr[j] = str[i];
      i++;
      j++;
    }
  }
}

double s21_pars(char *line) {
  double res;
  int i = 0;
  int length = s21_len(line, 61);
  Output_string *list = NULL;
  Stack *opn = NULL;
  while (i < length) {
    char *ach = strchr(NUMBERS, line[i]);
    char *bch = strchr(SIGNS, line[i]);
    char *cch = strchr(FUNCTIONS, line[i]);
    if (ach != NULL) {
      i = num_lexeme(line, i, &list);
    } else if (bch != NULL) {
      i = sign_lexeme(line, i, &opn, &list);
    }
    if (cch != NULL) {
      i = sign_func(line, i, &opn, &list);
    }
    /* если знак равно "=" */
    if (line[i] == 61) {
      res = s21_calculation(list, opn);
      i = i + 2;
    }
  }
  return res;
}

int s21_len(char *str, char c) {
  char *equals = strchr(str, c);
  int s_length = 0;
  if (equals != NULL) {
    s_length = equals - str + 1;
  }
  return s_length;
}

int num_lexeme(char *str, int a, Output_string **plist) {
  int s_length = s21_len(str, 61);
  char *buf = (char *)calloc((s_length + 1 - a), sizeof(char));
  buf[0] = str[a];
  int j = 1;
  a++;
  while (j < s_length) {
    char *ach1 = strchr(NUMBERS, str[a]);
    if (ach1 != NULL) {
      buf[j] = str[a];
      j++;
      a++;
    } else {
      j = s_length + 1;
    }
  }
  double num_lex = 0.0;
  char *nstr = buf;
  num_lex = strtod(nstr, &nstr);
  free(buf);
  s21_push_num(plist, num_lex);
  return a;
}

int sign_lexeme(char *str, int a, Stack **opn, Output_string **plist) {
  int type = 0;
  int priority = 0;
  if (str[a] == SIGNS[0]) {
    type = 1; /* sum */
    priority = 1;
  } else if (str[a] == SIGNS[1]) {
    if (a == 0 || str[a - 1] == 40) {
      a = num_lexeme(str, a + 1, plist) - 1;
      (*plist)->val = ((*plist)->val) * (-1);
    } else {
      type = 2; /* sub */
      priority = 1;
    }
  } else if (str[a] == SIGNS[2]) {
    type = 3; /* mul */
    priority = 2;
  } else if (str[a] == SIGNS[3]) {
    type = 4; /* div */
    priority = 2;
  } else if (str[a] == SIGNS[4]) {
    type = 5; /* pow */
    priority = 3;
  } else if (str[a] == SIGNS[5]) {
    type = 6; /* ( */
    priority = 5;
  } else {
    type = 7; /* ) */
    priority = 5;
  }
  if (type != 7 && type != 0) {
    s21_add_stack(plist, opn, type, priority);
  } else {
    if (type != 0) s21_add_brackets(plist, opn);
  }
  a++;
  return a;
}

int sign_func(char *str, int a, Stack **opn, Output_string **plist) {
  int type = 0;
  if (str[a] == FUNCTIONS[0]) {
    type = 8; /* mod */
    a = a + 3;
  } else if (str[a] == FUNCTIONS[1]) {
    type = 9; /* cos */
    a = a + 3;
  } else if (str[a] == FUNCTIONS[2]) {
    if (str[a + 1] == 105) {
      type = 10; /* sin */
      a = a + 3;
    } else {
      type = 11; /* sqrt */
      a = a + 4;
    }
  } else if (str[a] == FUNCTIONS[3]) {
    type = 12; /* tan */
    a = a + 3;
  } else if (str[a] == FUNCTIONS[4]) {
    if (str[a + 1] == FUNCTIONS[1]) {
      type = 13; /* acos */
    } else if (str[a + 1] == FUNCTIONS[2]) {
      type = 14; /* asin */
    } else {
      type = 15; /* atan */
    }
    a = a + 4;
  } else if (str[a] == FUNCTIONS[5]) {
    if (str[a + 1] == 110) {
      type = 16; /* ln */
      a = a + 2;
    } else if (str[a + 1] == 111) {
      type = 17; /* log */
      a = a + 3;
    }
  }
  int priority = 4;
  if (type == 8)
    priority = 2;
  else if (type == 11)
    priority = 3;
  s21_add_stack(plist, opn, type, priority);
  return a;
}

void s21_add_stack(Output_string **plist, Stack **top, int type, int priority) {
  if (*top == NULL || type == 6 || (*top)->priority < priority) {
    s21_push_sign(top, type, priority);
  } else if ((*top)->priority >= priority && (*top)->type != 6) {
    if ((*top)->type != 5 && (*top)->type != type) {
      while (((*top)->priority >= priority) || (*top)->priority != 5) {
        int top_priority = (*top)->priority;
        int action = s21_pop_sign(top);
        double a = s21_pop_num(plist);
        double res;
        if (top_priority < 4 && action != 11) {
          double b = s21_pop_num(plist);
          res = s21_maths_2args(a, b, action);
          s21_push_num(plist, res);
        } else {
          res = s21_maths_1arg(a, action);
          s21_push_num(plist, res);
        }
        if (*top == NULL || (*top)->type == 6 || (*top)->priority < priority)
          break;
      }
      if (type != 7) {
        s21_push_sign(top, type, priority);
      } else {
        s21_pop_sign(top);
      }
    } else {
      s21_push_sign(top, type, priority);
    }
  } else {
    s21_push_sign(top, type, priority);
  }
}

void s21_add_brackets(Output_string **plist, Stack **top) {
  if ((*top)->type != 6) {
    while ((*top)->type != 6) {
      int priority = (*top)->priority;
      int action = s21_pop_sign(top);
      double a = s21_pop_num(plist);
      double res;
      if (priority < 4 && action != 11) {
        double b = s21_pop_num(plist);
        res = s21_maths_2args(a, b, action);
      } else {
        res = s21_maths_1arg(a, action);
      }
      s21_push_num(plist, res);
    }
    s21_pop_sign(top);
  } else {
    s21_pop_sign(top);
  }
}

void s21_push_num(Output_string **plist, double digit) {
  Output_string *p = malloc(sizeof(Output_string));
  p->val = digit;
  p->next = *plist;
  *plist = p;
}

double s21_pop_num(Output_string **plist) {
  Output_string *p = *plist;
  double res = p->val;
  *plist = p->next;
  free(p);
  return res;
}

void s21_push_sign(Stack **top, int s_act, int order) {
  Stack *p = malloc(sizeof(Stack));
  p->type = s_act;
  p->priority = order;
  p->next = *top;
  *top = p;
}

int s21_pop_sign(Stack **top) {
  Stack *p = *top;
  int res = p->type;
  *top = p->next;
  free(p);
  return res;
}

double s21_calculation(Output_string *plist, Stack *top) {
  double res;
  while (top != NULL) {
    int priority = top->priority;
    int action = s21_pop_sign(&top);
    double a = s21_pop_num(&plist);
    if (priority < 4 && action != 11) {
      double b = s21_pop_num(&plist);
      res = s21_maths_2args(a, b, action);
      s21_push_num(&plist, res);
    } else {
      res = s21_maths_1arg(a, action);
      s21_push_num(&plist, res);
    }
  }
  double total = s21_pop_num(&plist);
  return total;
}

double s21_maths_2args(double last, double penultimate, int action) {
  double res;
  if (action == 1) {
    res = last + penultimate;
  } else if (action == 2) {
    res = penultimate - last;
  } else if (action == 3) {
    res = penultimate * last;
  } else if (action == 4) {
    res = penultimate / last;
  } else if (action == 5) {
    res = pow(penultimate, last);
  } else {
    res = fmod(penultimate, last);
  }
  return res;
}

double s21_maths_1arg(double last, int action) {
  double res;
  if (action == 9) {
    res = cos(last);
  } else if (action == 10) {
    res = sin(last);
  } else if (action == 11) {
    res = sqrt(last);
  } else if (action == 12) {
    res = tan(last);
  } else if (action == 13) {
    res = acos(last);
  } else if (action == 14) {
    res = asin(last);
  } else if (action == 15) {
    res = atan(last);
    return res;
  } else if (action == 16) {
    res = log(last);
  } else {
    res = log10(last);
  }
  return res;
}

int s21_verifying(char *str) {
  int length = s21_len(str, 61);
  int err = 0;
  int i = 0;
  int bracket1 = 0;
  int bracket2 = 0;
  if (length < 2) err = 1;
  if (str[0] == 42 || str[0] == 47) {
    err = 1;
  } else {
    while (i < length) {
      if (err != 0) i = 500;
      if (str[i] == 40) {
        err = s21_brackets(str, i);
        if (err == 0) bracket1++;
      }
      if (str[i] == 41) {
        bracket2++;
        if (str[i + 1] == 40) err = 1;
      }
      if ((str[i] > 41 && str[i] < 48) || str[i] == 94) {
        err = s21_sings(str, i);
      }
      if (str[i] > 47 && str[i] < 58) {
        err = s21_dots(str, i);
      }
      if (str[i] < 40 || str[i] > 57) {
        i = s21_symbols(str, i);
        if (i > 499) err = 1;
      }
      i++;
    }
    if (bracket1 != bracket2) err = 1;
  }
  return err;
}

int s21_sings(char *line, int a) {
  int error = 0;
  if (a == 0 && line[a + 1] == 61) {
    error = 1;
  }
  if ((line[a + 1] > 41 && line[a + 1] < 48) || line[a + 1] == 94 ||
      line[a + 1] == 61) {
    error = 1;
  }
  return error;
}

int s21_dots(char *line, int a) {
  int error = 0;
  int dot = 0;
  while (strchr(NUMBERS, line[a]) != NULL) {
    if (line[a] == 46) {
      dot++;
    }
    if (dot > 1) {
      error = 1;
      break;
    }
    a++;
  }
  return error;
}

int s21_symbols(char *line, int a) {
  if (strchr(NOT_USE, line[a]) != NULL) {
    a = 500;
  }
  if (line[a] < 40 || (line[a] > 57 && line[a] < 94) ||
      (line[a] > 94 && line[a] < 97) || line[a] > 116) {
    if (line[a] != 61) {
      a = 400;
    }
  }
  if (strchr(FUNCTIONS, line[a]) != NULL) {
    if (line[a] == FUNCTIONS[0]) {
      a = s21_check_mod(line, a);
    } else {
      if (a != 0) a = s21_not_num(line, a - 1);
      if (a < 256) a = s21_check_funcktions(line, a);
    }
  }
  return a;
}

int s21_not_num(char *str, int i) {
  int error = 0;
  if (strchr(NUMBERS, str[i]) != NULL) {
    error = 1;
  }
  if (str[i] == 41) error = 1;
  if (error == 1) {
    i = 500;
  } else {
    i = i + 1;
  }
  return i;
}

int s21_check_mod(char *str, int i) {
  int error = 0;
  if (i == 0) error = 1;
  if ((str[i + 1] != 'o') || (str[i + 2] != 'd') || (str[i + 3] == '-')) {
    error = 1;
  }
  if (strchr(SIGNS, str[i - 1]) != NULL) {
    if (str[i - 1] != 41) {
      error = 1;
    } else if (strchr(SIGNS, str[i + 4]) != NULL) {
      if (str[i + 4] != 40) error = 1;
    }
  }
  if (error > 0) {
    i = 500;
  } else {
    i = i + 2;
  }
  return i;
}

int s21_check_funcktions(char *str, int i) {
  if (str[i] == FUNCTIONS[1]) {
    i = s21_check_cos(str, i);
  } else if (str[i] == FUNCTIONS[2]) {
    i = s21_check_s(str, i);
  } else if (str[i] == FUNCTIONS[3]) {
    i = s21_check_tan(str, i);
  } else if (str[i] == FUNCTIONS[4]) {
    i = s21_check_a(str, i);
  } else {
    i = s21_check_log(str, i);
  }
  return i;
}

int s21_check_cos(char *str, int i) {
  int error = 0;
  if (str[i + 1] != 'o' || str[i + 2] != 's' || str[i + 3] != 40) {
    error = 1;
  }
  if (error > 0) {
    i = 500;
  } else {
    i = i + 2;
  }
  return i;
}

int s21_check_s(char *str, int i) {
  if (str[i + 1] == 'i') {
    if (str[i + 2] != 'n' || str[i + 3] != 40) {
      i = 500;
    } else {
      i = i + 2;
    }
  } else if (str[i + 1] == 'q') {
    if (str[i + 2] != 'r' || str[i + 3] != 't' || str[i + 4] != 40) {
      i = 500;
    } else {
      i = i + 3;
    }
  } else {
    i = 500;
  }
  return i;
}

int s21_check_tan(char *str, int i) {
  if (str[i + 1] != 'a' || str[i + 2] != 'n' || str[i + 3] != 40) {
    i = 500;
  } else {
    i = i + 2;
  }
  return i;
}

int s21_check_a(char *str, int i) {
  if (str[i + 1] == FUNCTIONS[1]) {
    i = s21_check_cos(str, i + 1);
  } else if (str[i + 1] == FUNCTIONS[2] && str[i + 2] != 'q') {
    i = s21_check_s(str, i + 1);
  } else if (str[i + 1] == FUNCTIONS[3]) {
    i = s21_check_tan(str, i + 1);
  } else {
    i = 500;
  }
  return i;
}

int s21_check_log(char *str, int i) {
  if (str[i + 1] == 'o') {
    if (str[i + 2] != 'g' || str[i + 3] != 40) {
      i = 500;
    } else {
      i = i + 2;
    }
  } else if (str[i + 1] == 'n') {
    if (str[i + 2] != 40) {
      i = 500;
    } else {
      i = i + 1;
    }
  } else {
    i = 500;
  }
  return i;
}

int s21_brackets(char *str, int i) {
  int error = 0;
  if (str[i + 1] == 42 || str[i + 1] == 47) {
    error = 1;
  }
  int s_length = s21_len(str, 61);
  int num = 0;
  int bracket2 = 0;
  for (int n = i + 1; n < s_length; n++) {
    if (strchr(NUMBERS, str[n]) != NULL) num = 1;
    if (str[n] == 40) bracket2--;
    if (str[n] == 41) bracket2++;
    if (bracket2 == 1) n = 500;
  }
  if (num != 1 || bracket2 != 1) {
    error = 1;
  }
  return error;
}

int s21_correct_num_act(char *str) {
  int actions[] = {43, 42, 47, 94, 109};
  int num[] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57};
  int firsr_a = s21_first_loc(str, actions, 5);
  int first_num = s21_first_loc(str, num, 10);
  int error = 0;
  if (firsr_a - first_num < 0) {
    error = 1;
  }
  if (error == 0) {
    int last_a = s21_last_loc(str, actions, 5);
    int last_num = s21_last_loc(str, num, 10);
    if (last_num - last_a <= 0) {
      error = 1;
    }
  }
  return error;
}

int s21_first_loc(char *line, int *symbols, int x) {
  int res = 500;
  char *ach;
  for (int i = 0; i < x; i++) {
    ach = strchr(line, symbols[i]);
    int pos_a = 500;
    if (ach != NULL) {
      pos_a = ach - line;
    } else {
      pos_a = 500;
    }
    if (pos_a < res) {
      res = pos_a;
    }
  }
  return res;
}

int s21_last_loc(char *line, int *symbols, int x) {
  int res = 0;
  char *ach;
  for (int i = 0; i < x; i++) {
    ach = strrchr(line, symbols[i]);
    int pos_a = 0;
    if (ach != NULL) {
      pos_a = ach - line;
    } else {
      pos_a = -1;
    }
    if (pos_a > res) {
      res = pos_a;
    }
  }
  return res;
}
