#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/** Individual **/
#define INFO_FILENAME "info_handlers.txt"
#define INFO_LINE_LENGTH (64)
#define INFO_DELIMITERS " /\t\n\v\f\r"

#define PORT "5131"
#define BACKLOG (10)

#define MAX_FILENAME_LENGTH (32)
#define LOGGING_FILENAME "load_balancer"

#define INPUT_LENGTH (32)
#define LIST_STR "list"
/** ---------- **/

#define MAX_KEY_LENGTH (32 + 1)
#define MAX_VALUE_LENGTH (128 + 1)

#define FOUR_BYTES (4)
#define TWO_BYTES (2)
#define HEADER_SIZE (4 * FOUR_BYTES)

#define PUT_CMD (1)
#define PUT_ACK_CMD (2)
#define GET_CMD (3)
#define GET_ACK_CMD (4)
#define DEL_CMD (5)
#define DEL_ACK_CMD (6)

#define CODE_NONE (0)
#define CODE_SUCCESS (1)
#define CODE_NOT_EXIST (2)
#define CODE_EXIST (3)

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

#endif // DEFINITIONS_H
