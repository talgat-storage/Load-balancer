#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/** Individual **/
#define INPUT_SET_STR "set"
#define INPUT_CON_STR "connect"
#define INPUT_PUT_STR "put"
#define INPUT_GET_STR "get"
#define INPUT_DEL_STR "del"

#define SET_CMD (7)
#define CON_CMD (8)

#define BALANCER_PORT "5131"

#define LOGGING_FILENAME "client"
/** ---------- **/

#define MAX_KEY_LENGTH (32 + 1)
#define MAX_VALUE_LENGTH (128 + 1)

#define INPUT_LENGTH (256)
#define INPUT_DELIMITERS " \t\n\v\f\r"
#define DOUBLE_QUOTE "\""

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

#define MAX_FILENAME_LENGTH (32)

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

#define FOUR_BYTES (4)
#define TWO_BYTES (2)
#define HEADER_SIZE (4 * FOUR_BYTES)

#endif // DEFINITIONS_H
