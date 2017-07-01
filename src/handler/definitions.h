#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define INFO_HANDLERS "info_handlers.txt"
#define INFO_CONTROL "info_control.txt"
#define INFO_WORKERS "info_workers.txt"
#define INFO_LINE_LENGTH (64)
#define INFO_DELIMITERS " /\t\n\v\f\r"

#define BACKLOG (10)

#define LOGGING_FILENAME "handler"
#define MAX_FILENAME_LENGTH (32)

#define INPUT_LENGTH (256)
#define INPUT_DELIMITERS " \t\n\v\f\r"

#define LIST_STR "list"
#define SHOW_STR "show"

#define LIST_CMD (1)
#define SHOW_CMD (2)

#define MAX_KEY_LENGTH (32 + 1)
#define MAX_VALUE_LENGTH (128 + 1)

#define FOUR_BYTES (4)
#define TWO_BYTES (2)
#define HEADER_SIZE (4 * FOUR_BYTES)

#define PUT_STR "PUT"
#define PUT_ACK_STR "PUT_ACK"
#define GET_STR "GET"
#define GET_ACK_STR "GET_ACK"
#define DEL_STR "DEL"
#define DEL_ACK_STR "DEL_ACK"

#define CODE_NONE_STR "CODE_NONE"
#define CODE_SUCCESS_STR "CODE_SUCCESS"
#define CODE_EXIST_STR "CODE_EXIST"
#define CODE_NOT_EXIST_STR "CODE_NOT_EXIST"

#define PUT_CMD (1)
#define GET_CMD (3)
#define DEL_CMD (5)

#define PUT_ACK_CMD (2)
#define GET_ACK_CMD (4)
#define DEL_ACK_CMD (6)

#define CODE_NONE (0)
#define CODE_SUCCESS (1)
#define CODE_NOT_EXIST (2)
#define CODE_EXIST (3)

#endif // DEFINITIONS_H
