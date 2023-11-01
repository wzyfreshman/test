/********************************************************************
* Description: interpl.cc
*   Mechanism for queueing NML messages, used by the interpreter and
*   canonical interface to report list of NML statements from program
*   files to HME.
*
*   Derived from a work by Fred Proctor & Will Shackleford
*
* Author:
* License: GPL Version 2
* System: Linux
*    
* Copyright (c) 2004 All rights reserved.
*
* Last change:
********************************************************************/


#include <string.h>		/* memcpy() */

#include "rcs.hh"		// LinkedList
#include "interpl.hh"		// these decls
#include "emc.hh"
#include "emcglb.h"
#include "linklist.hh"
#include "nmlmsg.hh"            /* class NMLmsg */
#include "rcs_print.hh"

#include "emc_nml.hh"

NML_INTERP_LIST interp_list;	/* NML Union, for interpreter */

MiniMove minimove;

NML_INTERP_LIST::NML_INTERP_LIST()
{
    linked_list_ptr = new LinkedList;

    next_line_number = 0;
    line_number = 0;
}

NML_INTERP_LIST::~NML_INTERP_LIST()
{
    if (NULL != linked_list_ptr) {
	delete linked_list_ptr;
	linked_list_ptr = NULL;
    }
}

int NML_INTERP_LIST::append(NMLmsg & nml_msg)
{
    /* check for invalid data */
    if (0 == nml_msg.type) {
	rcs_print_error
	    ("NML_INTERP_LIST::append : attempt to append 0 type\n");
	return -1;
    }

    if (NULL == linked_list_ptr) {
	return -1;
    }

    if (nml_msg.size > MAX_NML_COMMAND_SIZE - 64) {
	rcs_print_error
	    ("NML_INTERP_LIST::append : command size is too large.");
	return -1;
    }
    if (nml_msg.size < 4) {
	rcs_print_error
	    ("NML_INTERP_LIST::append : command size is invalid.");
	return -1;
    }
#ifdef DEBUG_INTERPL
    if (sizeof(temp_node) < MAX_NML_COMMAND_SIZE + 4 ||
	sizeof(temp_node) > MAX_NML_COMMAND_SIZE + 16) {
	rcs_print_error
	    ("NML_INTERP_LIST::append : assumptions about NML_INTERP_LIST_NODE have been violated.\n");
	return -1;
    }
#endif

    if(nml_msg.type == EMC_TRAJ_CIRCULAR_MOVE_TYPE ||
            nml_msg.type == EMC_TRAJ_LINEAR_MOVE_TYPE) {

        if(nml_msg.type == EMC_TRAJ_LINEAR_MOVE_TYPE) {
            EMC_TRAJ_LINEAR_MOVE line = static_cast<EMC_TRAJ_LINEAR_MOVE&>(nml_msg);
            minimove.cmdtype[minimove.currentnum] = LINEAR;
            minimove.minx[minimove.currentnum] = line.end.tran.x;
            minimove.miny[minimove.currentnum] = line.end.tran.y;
            minimove.minz[minimove.currentnum] = line.end.tran.z;
            minimove.mina[minimove.currentnum] = line.end.a;
            minimove.minb[minimove.currentnum] = line.end.b;
            minimove.minc[minimove.currentnum] = line.end.c;
            minimove.minu[minimove.currentnum] = line.end.u;
            minimove.minv[minimove.currentnum] = line.end.v;
            minimove.minw[minimove.currentnum] = line.end.w;
            minimove.minlinenum[minimove.currentnum] = next_line_number;
            minimove.vel[minimove.currentnum] = line.vel;
            minimove.acc[minimove.currentnum] = line.acc;
            minimove.gmodetype[minimove.currentnum] = line.gmodetype;
            minimove.dynamiccomp[minimove.currentnum] = line.dynamiccomp;
           //printf("line=%d,a=%.3f,b=%.3f,c=%.3f\n",next_line_number,
          //         line.end.a,line.end.b,line.end.c);

            minimove.type[minimove.currentnum] = 0;
            minimove.center[minimove.currentnum].x = 0;
            minimove.center[minimove.currentnum].y = 0;
            minimove.center[minimove.currentnum].z = 0;
            minimove.normal[minimove.currentnum].x = 0;
            minimove.normal[minimove.currentnum].y = 0;
            minimove.normal[minimove.currentnum].z = 0;
            minimove.turn[minimove.currentnum] = 0;
            minimove.ini_maxvel[minimove.currentnum] = 0;
            minimove.feed_mode[minimove.currentnum] = 0;
            minimove.currentnum ++;
            //printf("now count is %d\n",minimove.currentnum);
        }
        if(nml_msg.type == EMC_TRAJ_CIRCULAR_MOVE_TYPE) {
            EMC_TRAJ_CIRCULAR_MOVE circle = static_cast<EMC_TRAJ_CIRCULAR_MOVE&>(nml_msg);
            minimove.cmdtype[minimove.currentnum] = CIRCLE;
            minimove.minx[minimove.currentnum] = circle.end.tran.x;
            minimove.miny[minimove.currentnum] = circle.end.tran.y;
            minimove.minz[minimove.currentnum] = circle.end.tran.z;
            minimove.mina[minimove.currentnum] = circle.end.a;
            minimove.minb[minimove.currentnum] = circle.end.b;
            minimove.minc[minimove.currentnum] = circle.end.c;
            minimove.minu[minimove.currentnum] = circle.end.u;
            minimove.minv[minimove.currentnum] = circle.end.v;
            minimove.minw[minimove.currentnum] = circle.end.w;
            minimove.minlinenum[minimove.currentnum] = next_line_number;
            minimove.vel[minimove.currentnum] = circle.vel;
            minimove.acc[minimove.currentnum] = circle.acc;

            minimove.type[minimove.currentnum] = circle.type;
            minimove.center[minimove.currentnum].x = circle.center.x;
            minimove.center[minimove.currentnum].y = circle.center.y;
            minimove.center[minimove.currentnum].z = circle.center.z;
            minimove.normal[minimove.currentnum].x = circle.normal.x;
            minimove.normal[minimove.currentnum].y = circle.normal.y;
            minimove.normal[minimove.currentnum].z = circle.normal.z;
            minimove.turn[minimove.currentnum] = circle.turn;
            minimove.ini_maxvel[minimove.currentnum] = circle.ini_maxvel;
            minimove.feed_mode[minimove.currentnum] = circle.feed_mode;
            minimove.gmodetype[minimove.currentnum] = 0;
            minimove.dynamiccomp[minimove.currentnum] = circle.dynamiccomp;
            minimove.currentnum ++;
            //printf("now count is %d\n",minimove.currentnum);
        }
        if(maxlinearnum <= minimove.currentnum) {
            sendMiniMove();
        }
        return 0;
    } else {
        if(minimove.currentnum != 0) {
            sendMiniMove();
        }
    }

    // fill in the NML_INTERP_LIST_NODE
    temp_node.line_number = next_line_number;
    memcpy(temp_node.command.commandbuf, &nml_msg, nml_msg.size);

    // stick it on the list
    linked_list_ptr->store_at_tail(&temp_node,
                   nml_msg.size + sizeof(temp_node.line_number) +
                   sizeof(temp_node.dummy) + 32 + (32 -  nml_msg.size % 32), 1);

    if (EMC_DEBUG & EMC_DEBUG_INTERP_LIST) {
	rcs_print
	    ("NML_INTERP_LIST::append(nml_msg{size=%ld,type=%s}) : list_size=%d, line_number=%d\n",
	     nml_msg.size, emc_symbol_lookup(nml_msg.type),
	     linked_list_ptr->list_size, temp_node.line_number);
    }

    return 0;
}

// sets the line number used for subsequent appends
int NML_INTERP_LIST::set_line_number(int line)
{
    next_line_number = line;

    return 0;
}

int NML_INTERP_LIST::append(NMLmsg * nml_msg_ptr)
{
    /* check for invalid data */
    if (NULL == nml_msg_ptr) {
	rcs_print_error
	    ("NML_INTERP_LIST::append : attempt to append NULL msg\n");
	return -1;
    }

    if (0 == nml_msg_ptr->type) {
	rcs_print_error
	    ("NML_INTERP_LIST::append : attempt to append 0 type\n");
	return -1;
    }

    if (nml_msg_ptr->size > MAX_NML_COMMAND_SIZE - 64) {
	rcs_print_error
	    ("NML_INTERP_LIST::append : command size is too large.");
	return -1;
    }
    if (nml_msg_ptr->size < 4) {
	rcs_print_error
	    ("NML_INTERP_LIST::append : command size is invalid.");
	return -1;
    }
#ifdef DEBUG_INTERPL
    if (sizeof(temp_node) < MAX_NML_COMMAND_SIZE + 4 ||
	sizeof(temp_node) > MAX_NML_COMMAND_SIZE + 16 ||
	((void *) &temp_node.line_number) >
	((void *) &temp_node.command.commandbuf)) {
	rcs_print_error
	    ("NML_INTERP_LIST::append : assumptions about NML_INTERP_LIST_NODE have been violated.");
	return -1;
    }
#endif

    if (NULL == linked_list_ptr) {
	return -1;
    }

    if(minimove.currentnum != 0) {
        sendMiniMove();
    }

    // fill in the NML_INTERP_LIST_NODE
    temp_node.line_number = next_line_number;
    memcpy(temp_node.command.commandbuf, nml_msg_ptr, nml_msg_ptr->size);

    // stick it on the list
    linked_list_ptr->store_at_tail(&temp_node,
				   nml_msg_ptr->size +
				   sizeof(temp_node.line_number) +
				   sizeof(temp_node.dummy) + 32 + (32 -
								   nml_msg_ptr->
								   size %
								   32), 1);

    if (EMC_DEBUG & EMC_DEBUG_INTERP_LIST) {
	rcs_print
	    ("NML_INTERP_LIST::append(nml_msg_ptr{size=%ld,type=%s}) : list_size=%d, line_number=%d\n",
	     nml_msg_ptr->size, emc_symbol_lookup(nml_msg_ptr->type),
	     linked_list_ptr->list_size, temp_node.line_number);
    }

    return 0;
}

NMLmsg *NML_INTERP_LIST::get()
{
    NMLmsg *ret;
    NML_INTERP_LIST_NODE *node_ptr;

    if (NULL == linked_list_ptr) {
	line_number = 0;
	return NULL;
    }

    node_ptr = (NML_INTERP_LIST_NODE *) linked_list_ptr->retrieve_head();

    if (NULL == node_ptr) {
	line_number = 0;
	return NULL;
    }
    // save line number of this one, for use by get_line_number
    line_number = node_ptr->line_number;

    // get it off the front
    ret = (NMLmsg *) ((char *) node_ptr->command.commandbuf);

    return ret;
}

void NML_INTERP_LIST::clear()
{
    if (NULL != linked_list_ptr) {
	linked_list_ptr->delete_members();
    }
    minimove.clear();
}

void NML_INTERP_LIST::print()
{
    NMLmsg *nml_msg;

    if (NULL == linked_list_ptr) {
	return;
    }

    nml_msg = (NMLmsg *) linked_list_ptr->get_head();

    while (NULL != nml_msg) {
	rcs_print("%d ", (int)nml_msg->type);
	nml_msg = (NMLmsg *) linked_list_ptr->get_next();
    }

    rcs_print("\n");
}

int NML_INTERP_LIST::len()
{
    if (NULL == linked_list_ptr) {
	return 0;
    }

    return ((int) linked_list_ptr->list_size);
}

int NML_INTERP_LIST::get_line_number()
{
    return line_number;
}

void NML_INTERP_LIST::sendMiniMove()
{
    static EMC_TRAJ_MINLINEAR_MOVE minLinearMovemsg;
    minLinearMovemsg.mininum = minimove.currentnum;
    memcpy(minLinearMovemsg.minx,minimove.minx,sizeof(minimove.minx));
    memcpy(minLinearMovemsg.miny,minimove.miny,sizeof(minimove.miny));
    memcpy(minLinearMovemsg.minz,minimove.minz,sizeof(minimove.minz));
    memcpy(minLinearMovemsg.mina,minimove.mina,sizeof(minimove.mina));
    memcpy(minLinearMovemsg.minb,minimove.minb,sizeof(minimove.minb));
    memcpy(minLinearMovemsg.minc,minimove.minc,sizeof(minimove.minc));
    memcpy(minLinearMovemsg.minu,minimove.minu,sizeof(minimove.minu));
    memcpy(minLinearMovemsg.minv,minimove.minv,sizeof(minimove.minv));
    memcpy(minLinearMovemsg.minw,minimove.minw,sizeof(minimove.minw));
    memcpy(minLinearMovemsg.cmdtype,minimove.cmdtype,sizeof(minimove.cmdtype));
    memcpy(minLinearMovemsg.minlinenum,minimove.minlinenum,sizeof(minimove.minlinenum));
    memcpy(minLinearMovemsg.vel,minimove.vel,sizeof(minimove.vel));
    memcpy(minLinearMovemsg.acc,minimove.acc,sizeof(minimove.acc));
    memcpy(minLinearMovemsg.gmodetype,minimove.gmodetype,sizeof(minimove.gmodetype));

    memcpy(minLinearMovemsg.type,minimove.type,sizeof(minimove.type));
    memcpy(minLinearMovemsg.center,minimove.center,sizeof(minimove.center));
    memcpy(minLinearMovemsg.normal,minimove.normal,sizeof(minimove.normal));
    memcpy(minLinearMovemsg.turn,minimove.turn,sizeof(minimove.turn));
    memcpy(minLinearMovemsg.ini_maxvel,minimove.ini_maxvel,sizeof(minimove.ini_maxvel));
    memcpy(minLinearMovemsg.feed_mode,minimove.feed_mode,sizeof(minimove.feed_mode));
    memcpy(minLinearMovemsg.dynamiccomp,minimove.dynamiccomp,sizeof(minimove.dynamiccomp));
    minimove.currentnum = 0;
    temp_node.line_number = next_line_number;

    memcpy(temp_node.command.commandbuf, &minLinearMovemsg, minLinearMovemsg.size);
    linked_list_ptr->store_at_tail(&temp_node,
                   minLinearMovemsg.size + sizeof(temp_node.line_number) +
                   sizeof(temp_node.dummy) + 32 + (32 -  minLinearMovemsg.size % 32), 1);
}

void MiniMove::clear(){
    currentnum = 0;
    for(int i = 0;i< MAXMINLEN;i++) {
        cmdtype[0] = 0;
        minx[0] = 0;
        miny[0] = 0;
        minz[0] = 0;
        mina[0] = 0;
        minb[0] = 0;
        minc[0] = 0;
        minu[0] = 0;
        minv[0] = 0;
        minw[0] = 0;
        minlinenum[0] = 0;
        vel[0] = 0;
        acc[0] = 0;

        type[0] = 0;
        center[0].x = 0;
        center[0].y = 0;
        center[0].z = 0;
        normal[0].x = 0;
        normal[0].y = 0;
        normal[0].z = 0;
        turn[0] = 0;
        ini_maxvel[0] = 0;
        feed_mode[0] = 0;
    }
}

