/*
 * json.h
 *
 *  Created on: Aug 29, 2019
 *      Author: jcaf
 */

#ifndef JSON_H_
#define JSON_H_

	typedef struct _JSON
	{
		char *name;
		char *strval;
	}JSON;

	void json_cFloatArr(float *v, int size, char *outbuff);
	int8_t jsonDecode(char *stream, int16_t streamSize, JSON *json);

#endif /* JSON_H_ */
