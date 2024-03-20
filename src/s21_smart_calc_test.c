#include "s21_smart_calc.h"

#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

START_TEST(s21_smartcalc_1) {
  char str1[] = "1.055+2.045=";
  char str2[] = "1+2*(3+4/2   -(1+2))*2+1=";
  char str3[] = "-3*4+2=";
  char str4[] = "4^3^2=";
  char str5[] = "sqrt(sqrt(16))=";

  double value1 = smart_calc(str1);
  double value2 = smart_calc(str2);
  double value3 = smart_calc(str3);
  double value4 = smart_calc(str4);
  double value5 = smart_calc(str5);

  ck_assert_double_eq_tol(3.1, value1, 1e-6);
  ck_assert_double_eq_tol(10., value2, 1e-6);
  ck_assert_double_eq_tol(-10., value3, 1e-6);
  ck_assert_double_eq_tol(262144, value4, 1e-6);
  ck_assert_double_eq_tol(2, value5, 1e-6);
}
END_TEST

START_TEST(s21_smartcalc_2) {
  char *string1 =
      "5+(1+2)*4*cos(2*7.5-2)+sin(cos(2*5))-sqrt(2^5)+asin(0.5)+tan(50)=";
  char *string2 =
      "5+(1+2)*4*cos(2*7.5-2)+sin(cos(2*5))-sqrt(2mod5)+asin(0.5)+tan(50)=";
  char *string3 = "sqrt(4)+log(10)+ln(10)=";
  char *string4 = "sin(60)=";
  char *string5 = "asin(1/2)+acos(0)+atan(1)=";

  double value1 = smart_calc(string1);
  double value2 = smart_calc(string2);
  double value3 = smart_calc(string3);
  double value4 = smart_calc(string4);
  double value5 = smart_calc(string5);

  ck_assert_double_eq_tol(9.7401822, value1, 1e-6);
  ck_assert_double_eq_tol(13.9828229, value2, 1e-6);
  ck_assert_double_eq_tol(5.3025851, value3, 1e-6);
  ck_assert_double_eq_tol(-0.3048106, value4, 1e-6);
  ck_assert_double_eq_tol(2.8797933, value5, 1e-6);
}
END_TEST

START_TEST(s21_smartcalc_3) {
  char *string1 = "-(3.5+2.6)=";
  char *string2 = "5+(-1+3)*4=";
  char *string3 = "acos(0.9)=";
  char *string4 = "acos(-0.9)=";

  double value1 = smart_calc(string1);
  double value2 = smart_calc(string2);
  double value3 = smart_calc(string3);
  double value4 = smart_calc(string4);

  ck_assert_double_eq_tol(-6.1, value1, 1e-6);
  ck_assert_double_eq_tol(13, value2, 1e-6);
  ck_assert_double_eq_tol(0.451026812, value3, 1e-6);
  ck_assert_double_eq_tol(2.69056584, value4, 1e-6);
}
END_TEST

START_TEST(s21_smartcalc_4) {
  char *string1 = "-(3.5.63+2.6)=";
  char *string2 = "5)+(-1+3*44=";
  char *string3 = "5+(-1)+3*44)=";

  int value1 = s21_verifying(string1);
  int value2 = s21_verifying(string2);
  int value3 = s21_verifying(string3);

  ck_assert_int_eq(1, value1);
  ck_assert_int_eq(1, value2);
  ck_assert_int_eq(1, value3);
}
END_TEST

START_TEST(s21_smartcalc_5) {
  char *string1 = "acon(-0.9)=";
  char *string2 = "asis(-0.9)=";
  char *string3 = "atug(-0.9)=";
  char *string4 = "ayos(-0.9)=";
  char *string5 = "4mon3=";
  char *string6 = "sqrt4=";
  char *string7 = "lon(4)=";

  int value1 = s21_verifying(string1);
  int value2 = s21_verifying(string2);
  int value3 = s21_verifying(string3);
  int value4 = s21_verifying(string4);
  int value5 = s21_verifying(string5);
  int value6 = s21_verifying(string6);
  int value7 = s21_verifying(string7);

  ck_assert_int_eq(1, value1);
  ck_assert_int_eq(1, value2);
  ck_assert_int_eq(1, value3);
  ck_assert_int_eq(1, value4);
  ck_assert_int_eq(1, value5);
  ck_assert_int_eq(1, value6);
  ck_assert_int_eq(1, value7);
}
END_TEST

START_TEST(s21_smartcalc_6) {
  char *string1 = "*1-3=";
  char *string2 = "70/=";
  char *string3 = "cos(/50)=";
  char *string4 = "4+3/=";
  char *string5 = "4*mod3=";
  char *string6 = "4mod**=";

  int value1 = s21_verifying(string1);
  int value2 = s21_verifying(string2);
  int value3 = s21_verifying(string3);
  int value4 = s21_verifying(string4);
  int value5 = s21_verifying(string5);
  int value6 = s21_verifying(string6);

  ck_assert_int_eq(1, value1);
  ck_assert_int_eq(1, value2);
  ck_assert_int_eq(1, value3);
  ck_assert_int_eq(1, value4);
  ck_assert_int_eq(1, value5);
  ck_assert_int_eq(1, value6);
}
END_TEST

int main() {
  Suite *s1 = suite_create("Core");
  TCase *tc_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int a;

  suite_add_tcase(s1, tc_1);

  tcase_add_test(tc_1, s21_smartcalc_1);
  tcase_add_test(tc_1, s21_smartcalc_2);
  tcase_add_test(tc_1, s21_smartcalc_3);
  tcase_add_test(tc_1, s21_smartcalc_4);
  tcase_add_test(tc_1, s21_smartcalc_5);
  tcase_add_test(tc_1, s21_smartcalc_6);

  srunner_run_all(sr, CK_ENV);
  a = srunner_ntests_failed(sr);
  srunner_free(sr);

  return a == 0 ? 0 : 1;
}
