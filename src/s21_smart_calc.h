#ifndef SRC_S21_SMART_CALC_H_
#define SRC_S21_SMART_CALC_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  double val;
  struct Node *next;
} Output_string;

typedef struct Actions {
  int type;
  int priority;
  struct Actions *next;
} Stack;

#define NUMBERS "1234567890."
#define SIGNS "+-*/^()"
#define FUNCTIONS "mcstal"
#define NOT_USE "befhjkp"

double smart_calc(char *string);
void s21_correct_string(char *str, char *cstr);
int s21_len(char *str, char c);
int s21_verifying(char *str);
void s21_push_num(Output_string **plist,
                  double digit); /* добавить элемент в стек чисел */
double s21_pop_num(Output_string **plist); /* извлечь элемент из стека чисел */
void s21_push_sign(Stack **top, int s_act,
                   int order); /* добавить элемент в стек мат.операций */
int s21_pop_sign(Stack **top); /* извлечь элемент из стека мат.операций */
double s21_pars(char *line);
int num_lexeme(char *str, int a, Output_string **plist);
int sign_lexeme(char *str, int a, Stack **opn, Output_string **plist);
int sign_func(char *str, int a, Stack **opn, Output_string **plist);
int s21_sings(char *line, int a);
/* рядом два любых символа из define SIGNS */
int s21_dots(char *line, int a); /* две точки в 1 числе */
// int is_empty(Output_string *list);
int s21_symbols(char *line,
                int a); /* проверка символов, кроме NUMBERS и SIGNS */
int s21_not_num(char *str, int i);
int s21_check_mod(char *str, int i); /* проверка ввода "mod" */
int s21_check_funcktions(char *str,
                         int i); /* проверка ввода всех функций, кроме "mod" */
int s21_check_cos(char *str, int i); /* проверка ввода "cos" */
int s21_check_s(char *str, int i); /* проверка ввода "sin", "sqrt" */
int s21_check_tan(char *str, int i); /* проверка ввода "tan" */
int s21_check_a(char *str, int i); /* проверка ввода "asin", "acos", "atan" */
int s21_check_log(char *str, int i); /* проверка ввода "ln", "log" */
int s21_brackets(char *str, int i); /* проверка выражения в скобках */
int s21_correct_num_act(char *str);
/* проверка наличия цифр в строке после знака */
int s21_first_loc(char *line, int *symbols,
                  int x); /* первое вхождение символа в строке */
int s21_last_loc(char *line, int *symbols,
                 int x); /* последнее вхождение символа в строке */
double s21_calculation(Output_string *plist,
                       Stack *top); /* вычисление после ввода знака "=" */
double s21_maths_2args(double last, double penultimate,
                       int action); /* вычисление функций с 2-мя аргументами*/
double s21_maths_1arg(double last,
                      int action); /* вычисление функций с 1-м аргументом*/
void s21_add_stack(Output_string **plist, Stack **top, int type,
                   int priority); /* dobavit*/
void s21_add_brackets(Output_string **plist, Stack **top);

#endif  // SRC_S21_SMART_CALC_H_
