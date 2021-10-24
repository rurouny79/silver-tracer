/*
 * silver2.h
 *
 *  Created on: 2015. 12. 11.
 *      Author: minseokk.kim
 */

#ifndef SILVER_H_
#define SILVER_H_

void silver_init(int start_log);

typedef struct {
	unsigned int tid;
    unsigned int level;
	char arrow;
	unsigned long long faddr;
	unsigned long long caddr;
}__attribute__((packed)) MYDATA;

#endif /* SILVER_H_ */
