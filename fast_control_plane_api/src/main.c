/*
 * main.c
 *
 *  Created on: Jan 12, 2016
 *      Author: xiong
 */

#include "fast_api.h"
#include "nmac_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

u_int8_t mac[3][6] = 
{
	{0x68, 0xf7, 0x28, 0xd9, 0xbf, 0x71}, /*port 3*/
	{0x00, 0x16, 0x41, 0xe0, 0x5e, 0x55}, /*port 1*/
	{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, /*FF*/
};


void add_flow(u_int8_t dst[6],u_int8_t src[6],u_int16_t frame_type,u_int8_t inport,u_int8_t outport,u_int8_t idx)
{
	struct sw_flow *flow = malloc(sizeof(struct sw_flow));
	memset((char *)flow,0,16);
	flow->valid = 1;
	flow->input_port = inport;
	flow->action = outport;
	flow->frame_type = htons(frame_type);
	memcpy(flow->src_mac, src , 6);
	memcpy(flow->dst_mac, dst , 6);
	if ( add_flow_entry(flow, idx) == 0){
		printf("add succ\n");
	}
	else
		printf("add failed!\n");
	free(flow);
}

int main() {
	int oxm_ofb_match_fields[40];
	int ofp_action_type[28];
	int capacity;
	int idx = 0;
	if (hw_properties(oxm_ofb_match_fields, ofp_action_type, &capacity) == 0) {
		printf("init suc\n");
	}

	add_flow(mac[0],mac[1],0x0800,1,3,idx++);
	add_flow(mac[1],mac[0],0x0800,3,1,idx++);
	add_flow(mac[2],mac[0],0x0806,3,1,idx++);
	add_flow(mac[2],mac[1],0x0806,1,3,idx++);
	add_flow(mac[0],mac[1],0x0806,1,3,idx++);
	add_flow(mac[1],mac[0],0x0806,3,1,idx++);
	return 0;
}
