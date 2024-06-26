#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"
// Necessary due to static functions in state.c
#include "state.c"

/* Look at asserts.c for some helpful assert functions */

int greater_than_forty_two(int x) { return x > 42; }

bool is_vowel(char c) {
  char *vowels = "aeiouAEIOU";
  for (int i = 0; i < strlen(vowels); i++) {
    if (c == vowels[i]) {
      return true;
    }
  }
  return false;
}

/*
  Example 1: Returns true if all test cases pass. False otherwise.
    The function greater_than_forty_two(int x) will return true if x > 42. False otherwise.
    Note: This test is NOT comprehensive
*/
bool test_greater_than_forty_two() {
  int testcase_1 = 42;
  bool output_1 = greater_than_forty_two(testcase_1);
  if (!assert_false("output_1", output_1)) {
    return false;
  }

  int testcase_2 = -42;
  bool output_2 = greater_than_forty_two(testcase_2);
  if (!assert_false("output_2", output_2)) {
    return false;
  }

  int testcase_3 = 4242;
  bool output_3 = greater_than_forty_two(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  return true;
}

/*
  Example 2: Returns true if all test cases pass. False otherwise.
    The function is_vowel(char c) will return true if c is a vowel (i.e. c is a,e,i,o,u)
    and returns false otherwise
    Note: This test is NOT comprehensive
*/
bool test_is_vowel() {
  char testcase_1 = 'a';
  bool output_1 = is_vowel(testcase_1);
  if (!assert_true("output_1", output_1)) {
    return false;
  }

  char testcase_2 = 'e';
  bool output_2 = is_vowel(testcase_2);
  if (!assert_true("output_2", output_2)) {
    return false;
  }

  char testcase_3 = 'i';
  bool output_3 = is_vowel(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  char testcase_4 = 'o';
  bool output_4 = is_vowel(testcase_4);
  if (!assert_true("output_4", output_4)) {
    return false;
  }

  char testcase_5 = 'u';
  bool output_5 = is_vowel(testcase_5);
  if (!assert_true("output_5", output_5)) {
    return false;
  }

  char testcase_6 = 'k';
  bool output_6 = is_vowel(testcase_6);
  if (!assert_false("output_6", output_6)) {
    return false;
  }

  return true;
}

/* Task 4.1 */

bool test_is_tail() {
  bool case1 = is_tail('a');
  bool case2 = is_tail('s');
  bool case3 = is_tail('d');
  bool case4 = is_tail('w');
  bool case5 = is_tail('A');
  bool case6 = is_tail('>');
  bool case7 = is_tail('x');
  bool case8 = is_tail('h');
  if (!assert_true("case1", case1)) return false;
  if (!assert_true("case2", case2)) return false;
  if (!assert_true("case3", case3)) return false;
  if (!assert_true("case4", case4)) return false;
  if (!assert_false("case5", case5)) return false;
  if (!assert_false("case6", case6)) return false;
  if (!assert_false("case7", case7)) return false;
  if (!assert_false("case8", case8)) return false;
  return true;
}

bool test_is_head() {
    bool case1 = is_head('a');
  bool case2 = is_head('s');
  bool case3 = is_head('d');
  bool case4 = is_head('w');
  bool case5 = is_head('A');
  bool case6 = is_head('>');
  bool case7 = is_head('x');
  bool case8 = is_head('h');
  if (!assert_true("case1", case1)) return false;
  if (!assert_true("case2", case2)) return false;
  if (!assert_true("case3", case3)) return false;
  if (!assert_true("case4", case4)) return false;
  if (!assert_false("case5", case5)) return false;
  if (!assert_false("case6", case6)) return false;
  if (!assert_false("case7", case7)) return false;
  if (!assert_false("case8", case8)) return false;
  return true;
}

bool test_is_snake() {
  bool case1 = is_snake('a');
  bool case2 = is_snake('s');
  bool case3 = is_snake('d');
  bool case4 = is_snake('w');
  bool case5 = is_snake('A');
  bool case6 = is_snake('W');
  bool case7 = is_snake('S');
  bool case8 = is_snake('D');
  bool case9 = is_snake('x');
  bool case10 = is_snake('^');
  bool case11 = is_snake('v');
  bool case12 = is_snake('>');
  bool case13 = is_snake('<');
  bool case14 = is_snake('q');
  bool case15 = is_snake('!');
  bool case16 = is_snake('X');
  if (!assert_true("case1", case1)) return false;
  if (!assert_true("case2", case2)) return false;
  if (!assert_true("case3", case3)) return false;
  if (!assert_true("case4", case4)) return false;
  if (!assert_true("case5", case5)) return false;
  if (!assert_true("case6", case6)) return false;
  if (!assert_true("case7", case7)) return false;
  if (!assert_true("case8", case8)) return false;
  if (!assert_true("case9", case9)) return false;
  if (!assert_true("case10", case10)) return false;
  if (!assert_true("case11", case11)) return false;
  if (!assert_true("case12", case12)) return false;
  if (!assert_true("case13", case13)) return false;
  if (!assert_false("case14", case14)) return false;
  if (!assert_false("case15", case15)) return false;
  if (!assert_false("case16", case16)) return false;
  return true;
}

bool test_body_to_tail() {
  char case1 = body_to_tail('^');
  char case2 = body_to_tail('<');
  char case3 = body_to_tail('v');
  char case4 = body_to_tail('>');
  char case5 = body_to_tail('x');
  char case6 = body_to_tail('W');
  char case7 = body_to_tail('s');
  char case8 = body_to_tail('j');
  if (!assert_equals_char("case1", 'w', case1)) return false;
  if (!assert_equals_char("case2", 'a', case2)) return false;
  if (!assert_equals_char("case3", 's', case3)) return false;
  if (!assert_equals_char("case4", 'd', case4)) return false;
  if (!assert_equals_char("case5", 'x', case5)) return false;
  if (!assert_equals_char("case6", 'W', case6)) return false;
  if (!assert_equals_char("case7", 's', case7)) return false;
  if (!assert_equals_char("case8", 'j', case8)) return false;
  return true;
  }

bool test_head_to_body() {
  char case1 = head_to_body('W');
  char case2 = head_to_body('A');
  char case3 = head_to_body('S');
  char case4 = head_to_body('D');
  char case5 = head_to_body('x');
  char case6 = head_to_body('w');
  char case7 = head_to_body('^');
  char case8 = head_to_body('j');
  if (!assert_equals_char("case1", '^', case1)) return false;
  if (!assert_equals_char("case2", '<', case2)) return false;
  if (!assert_equals_char("case3", 'v', case3)) return false;
  if (!assert_equals_char("case4", '>', case4)) return false;
  if (!assert_equals_char("case5", 'x', case5)) return false;
  if (!assert_equals_char("case6", 'w', case6)) return false;
  if (!assert_equals_char("case7", '^', case7)) return false;
  if (!assert_equals_char("case8", 'j', case8)) return false;
  return true;
}

bool test_get_next_row() {
  bool case1 = get_next_row('v');
  bool case2 = get_next_row('s');
  bool case3 = get_next_row('S');
  bool case4 = get_next_row('^');
  bool case5 = get_next_row('A');
  bool case6 = get_next_row('W');
  bool case7 = get_next_row('x');
  bool case8 = get_next_row('h');
  if (!assert_true("case1", case1)) return false;
  if (!assert_true("case2", case2)) return false;
  if (!assert_true("case3", case3)) return false;
  if (!assert_true("case4", case4)) return false;
  if (!assert_false("case5", case5)) return false;
  if (!assert_false("case6", case6)) return false;
  if (!assert_false("case7", case7)) return false;
  if (!assert_false("case8", case8)) return false;
  return true; }



bool test_get_next_col() {
  bool case1 = get_next_col('>');
  bool case2 = get_next_col('D');
  bool case3 = get_next_col(2);
  bool case4 = get_next_col('<');
  bool case5 = get_next_col('a');
  bool case6 = get_next_col('w');
  bool case7 = get_next_col('x');
  bool case8 = get_next_col('h');
  if (!assert_true("case1", case1)) return false;
  if (!assert_true("case2", case2)) return false;
  if (!assert_true("case3", case3)) return false;
  if (!assert_true("case4", case4)) return false;
  if (!assert_false("case5", case5)) return false;
  if (!assert_false("case6", case6)) return false;
  if (!assert_false("case7", case7)) return false;
  if (!assert_false("case8", case8)) return false;
  return true;
}


bool test_customs() {
  if (!test_greater_than_forty_two()) {
    printf("%s\n", "test_greater_than_forty_two failed.");
    return false;
  }
  if (!test_is_vowel()) {
    printf("%s\n", "test_is_vowel failed.");
    return false;
  }
  if (!test_is_tail()) {
    printf("%s\n", "test_is_tail failed");
    return false;
  }
  if (!test_is_head()) {
    printf("%s\n", "test_is_head failed");
    return false;
  }
  if (!test_is_snake()) {
    printf("%s\n", "test_is_snake failed");
    return false;
  }
  if (!test_body_to_tail()) {
    printf("%s\n", "test_body_to_tail failed");
    return false;
  }
  if (!test_head_to_body()) {
    printf("%s\n", "test_head_to_body failed");
    return false;
  }
  if (!test_get_next_row()) {
    printf("%s\n", "test_get_next_row failed");
    return false;
  }
  if (!test_get_next_col()) {
    printf("%s\n", "test_get_next_col failed");
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  init_colors();

  if (!test_and_print("custom", test_customs)) {
    return 0;
  }

  return 0;
}
