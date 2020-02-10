//
// Created by Stanislav Lakhtin on 10/02/2020.
//

#ifndef edJSON_H
#define edJSON_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef int8_t edjson_err_t;     //  user errors bigger than 0, 0 -- OK, system error lower than 0

#define EDJSON_OK                             0x00
#define EDJSON_ERR_NO_DIGIT_SYMBOL            -1
#define EDJSON_ERR_ARRAY_CANT_PARSE_TO_VALUE  -10

typedef enum {
  JSON_ROOT,
  JSON_STRING,
  JSON_DOUBLE,
  JSON_INT,
  JSON_BOOLEAN,
  JSON_ARRAY,
} json_element_type_t;

typedef struct {
  char* raw_value;
} json_element_value_t;

typedef edjson_err_t ( * edjson_to_int ) ( const json_element_value_t * data, int * value );
typedef edjson_err_t ( * edjson_to_string ) ( const json_element_value_t * data, char * buffer );
//typedef edjson_err_t ( * edjson_to_double ) ( json_element_value_t * data );
//typedef edjson_err_t ( * edjson_to_boolean ) ( json_element_value_t * data );

typedef struct {
  json_element_type_t;
  char* name;
} json_element_t;

typedef edjson_err_t ( * edjson_to_begin ) ( void );
typedef edjson_err_t ( * edjson_read_next ) ( uint16_t size );
typedef edjson_err_t ( * edjson_error_handler ) ( void );

typedef edjson_err_t ( * on_start_element ) ( void );
typedef edjson_err_t ( * on_end_element ) ( void );
typedef edjson_err_t ( * on_element_name ) (json_element_t node);
typedef edjson_err_t ( * on_element_value ) (json_element_t node, json_element_value_t value);

typedef struct {
  edjson_to_begin init;
  edjson_read_next read;
  edjson_error_handler error;
} json_parser_t;

#ifdef __cplusplus
extern "C"
{
#endif

edjson_err_t as_int( const json_element_value_t * data, int * value );
edjson_err_t as_string( const json_element_value_t * data, char * buffer );


#ifdef __cplusplus
}
#endif

#endif //edJSON_H
