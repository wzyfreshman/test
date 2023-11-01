/********************************************************************
* Description: interpl.hh
*   Mechanism for building lists of arbitrary NML messages, used by
*   the canonical interface and interpreter to pass planned sequences
*   to the HME.
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
#ifndef INTERP_LIST_HH
#define INTERP_LIST_HH

#include "emc.hh"
#include "emcpos.h"

#define MAX_NML_COMMAND_SIZE 100000 //hong -change to 10000 20150812

// these go on the interp list
struct NML_INTERP_LIST_NODE {
    int line_number;		// line number it was on
    union _dummy_union {
	int i;
	long l;
	double d;
	float f;
	long long ll;
	long double ld;
    } dummy;			// paranoid alignment variable.

    union _command_union {
	char commandbuf[MAX_NML_COMMAND_SIZE];	// the NML command;
	int i;
	long l;
	double d;
	float f;
	long long ll;
	long double ld;
    } command;
};

// here's the interp list itself
class NML_INTERP_LIST {
  public:
    NML_INTERP_LIST();
    ~NML_INTERP_LIST();

    int set_line_number(int line);
    int get_line_number();
    int append(NMLmsg &);
    int append(NMLmsg *);
    NMLmsg *get();
    void clear();
    void print();
    int len();
    void sendMiniMove();

  private:
    class LinkedList * linked_list_ptr;
    NML_INTERP_LIST_NODE temp_node;	// filled in and put on the list
    int next_line_number;	// line number used to fill temp_node
    int line_number;		// line number of node from get()
};

enum{
    LINEAR = 0,
    CIRCLE = 1
};
extern NML_INTERP_LIST interp_list;	/* NML Union, for interpreter */

class MiniMove{

public:
    MiniMove() {
        currentnum = 0;
    };
    void clear();
    int currentnum ;
    int cmdtype[MAXMINLEN];
    double minx[MAXMINLEN];
    double miny[MAXMINLEN];
    double minz[MAXMINLEN];
    double mina[MAXMINLEN],minb[MAXMINLEN],minc[MAXMINLEN];
    double minu[MAXMINLEN],minv[MAXMINLEN],minw[MAXMINLEN];
    int minlinenum[MAXMINLEN];
    double vel[MAXMINLEN];
    double acc[MAXMINLEN];
    int gmodetype[MAXMINLEN];

    int type[MAXMINLEN];
    PM_CARTESIAN center[MAXMINLEN];
    PM_CARTESIAN normal[MAXMINLEN];
    int turn[MAXMINLEN];
    double ini_maxvel[MAXMINLEN];
    int feed_mode[MAXMINLEN];
    double dynamiccomp[MAXMINLEN];


};
#endif
