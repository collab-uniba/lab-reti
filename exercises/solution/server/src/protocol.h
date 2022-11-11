/*
 * protocol.h
 *
 *  Created on: 11 nov 2022
 *      Author: fabio
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define PROTO_PORT 60000
#define BUFFER_SIZE 64
#define QLEN 6

typedef struct {
	char string1[BUFFER_SIZE];
	char string2[BUFFER_SIZE];
} msg;

#endif /* PROTOCOL_H_ */
