//
// Created by Stanislav Lakhtin on 10/02/2020.
//

#include "edJSON.h"

static const char * DIGITS = "0123456789aAbBcCdDeEfF";
static const char * COMMAS = "\"\'";

#define isspace(value) (value == ' ' || value ==

edjson_err_t as_int( const json_element_value_t * data, int * value) {
  int sz = strlen(data->raw_value);
  char* str = malloc(sz);
  memcpy(str, data->raw_value, sz);
  for (int i = 0; i < sz; i++) {
    if (strchr(COMMAS, str[i]))
      str[i] = ' ';
  }
  *value = atoi(str);
  free(str);
  return EDJSON_OK;
}

edjson_err_t as_string( const json_element_value_t * data, char * buffer ) {
  int sz = strlen(data->raw_value);
  buffer = malloc(sz);
  memcpy(buffer, data->raw_value, sz);
  for (int i = 0; i < sz; i++) {
    if (strchr(COMMAS, buffer[i]))
      buffer[i] = ' ';
  }
  return EDJSON_OK;
}