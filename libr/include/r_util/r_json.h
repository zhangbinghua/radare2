/* radare - LGPL - Copyright 2020 - thestr4ng3r, Yaroslav Stavnichiy */

#ifndef R_JSON_H
#define R_JSON_H

#include <r_types.h>

#ifdef  __cplusplus
extern "C" {
#endif

/*
 * r_json is a JSON parsing API,
 * heavily based on nxjson by Yaroslav Stavnichiy <yarosla@gmail.com>,
 * which is available under LGPLv3 or later.
 *
 * r_json does NOT format json, it only parses. To format json, see pj.h instead.
 * It operates in-place, which means the parsed string will be MODIFIED.
 * This means all string values in RJson point directly into the input string,
 * removing the need to copy them.
 *
 * It also supports both line and block style comments.
 */

typedef enum r_json_type_t {
	R_JSON_NULL,
	R_JSON_OBJECT,  // properties can be found in child nodes
	R_JSON_ARRAY,   // items can be found in child nodes
	R_JSON_STRING,  // value can be found in str_value field
	R_JSON_INTEGER, // value can be found in num.u_value/num.s_value field
	R_JSON_DOUBLE,  // value can be found in num.dbl_value field
	R_JSON_BOOL     // value can be found in num.u_value field
} RJsonType;

typedef struct r_json_t {
	RJsonType type;             // type of json node, see above
	const char *key;            // key of the property; for object's children only
	union {
		const char *text_value; // text value of STRING node
		struct {
			union {
				ut64 u_value;   // the value of INTEGER or BOOL node
				st64 s_value;
			};
			double dbl_value;   // the value of DOUBLE node
		} num;
		struct {                // children of OBJECT or ARRAY
			int length;
			struct nx_json *first;
			struct nx_json *last;
		} children;
	};
	struct r_json_t *next;    // points to next child
} RJson;

typedef int (*nx_json_unicode_encoder)(unsigned int codepoint, char *p, char **endp);

extern nx_json_unicode_encoder nx_json_unicode_to_utf8;

const RJson *nx_json_parse(char *text, nx_json_unicode_encoder encoder);

const RJson *nx_json_parse_utf8(char *text);

void nx_json_free(const RJson *js);

const RJson *nx_json_get(const RJson *json, const char *key); // get object's property by key
const RJson *nx_json_item(const RJson *json, int idx); // get array element by index


#ifdef  __cplusplus
}
#endif

#endif  /* NXJSON_H */
