#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bst.h"

void read_to_data(data_t *data, string_t input);
void strip_trailing_newline(string_t string);
int counter(int delta, int key);

int main(int argc, char **argv) {

  node_t *dict1 = NULL;
  data_t data;
  string_t input = malloc(sizeof(char) * (MAX_LINE_LEN + 1));
  string_t input_file = argv[1], output_file = argv[2];

  // File I/O
  FILE *fin = fopen(input_file, "r");
  // Overwrite output.txt and reopen for appending
  fclose(fopen("output.txt", "w"));
  FILE *fout = fopen(output_file, "a");

  while (fgets(input, MAX_LINE_LEN, fin) != NULL) {
    read_to_data(&data, input);
    dict1 = insert(dict1, data);
  }

  int found_match = FALSE;
  while (fgets(input, MAX_STR_LEN, stdin) != NULL) {
    // Prepare input string for search
    found_match = FALSE;
    strip_trailing_newline(input);

    // Reset counter for comparisons
    counter(0, RESET);
    search(dict1, input, fout, &found_match);
    // Output number of comparisons to stdout
    fprintf(stdout, "%s --> %d\n", input, counter(0, RETURN));

    if (found_match == FALSE) {
      fprintf(fout, "%s --> NOT FOUND\n\n", input);
    }
  }

  free_dict(dict1);
  fclose(fin), fclose(fout);

  return 0;
}

void read_to_data(data_t *data, string_t input) {
  /* Read a csv line from stdin into a data structure's members.
  */

  string_t* member = (string_t*)data;
  char c, *temp_str = malloc(sizeof(char) * MAX_STR_LEN);
  int letter_idx = 0, input_idx = 0;

  // Iterate input string characters, store chars up to , in temp_str
  for (input_idx = 0; input_idx < strlen(input); input_idx++) {
  c = input[input_idx];

    // If comma or end of line reached, copy temp string into struct
    if (c == ',' || c == '\n') {
      temp_str[letter_idx] = '\0';
      //  Only chars up to and including '\0' in temp_str are copied.
      *member = malloc(sizeof(char) * (strlen(temp_str) + 1));
      strcpy(*member, temp_str);
      letter_idx = 0, member++;
    }
    // Otherwise, add character to current string
    else {
      temp_str[letter_idx++] = c;
    }
  }
  free(temp_str);
  return;
}

void strip_trailing_newline(string_t string) {
  /* Removes the newline character at the end of a string. Also Removes
     carriage returns.
  */
  int end = strlen(string);
  if (strlen(string) > 2 && string[end - 2] == '\r') {
    string[end - 2] = '\0';
  } else if (string[end - 1] == '\n') {
    string[end - 1] = '\0';
  }
  return;
}

int counter(int delta, int key) {
  /* Counter program for keeping track of comparisons in a section of code.
  Different cases for key value:
  - INCREMENT: increment counter
  - RESET: reset counter
  - RETURN: return counter value
  */
  static int counter = 0;

  if (key == INCREMENT) {
    counter += delta;
  } else if (key == RETURN){
    return counter;
  } else if (key == RESET) {
    counter = 0;
  } else {
    printf("Error! Invalid usage of counter function.\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}