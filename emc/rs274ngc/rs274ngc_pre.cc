/********************************************************************
* Description: rs274ngc_pre.cc
*
*   Derived from a work by Thomas Kramer
*
* Author:
* License: GPL Version 2
* System: Linux
*    
* Copyright (c) 2004 All rights reserved.
*
* Last change:
********************************************************************/
/* rs274ngc.cc

This rs274ngc.cc file contains the source code for (1) the kernel of
several rs274ngc interpreters and (2) two of the four sets of interface
functions declared in canon.hh:
1. interface functions to call to tell the interpreter what to do.
   These all return a status value.
2. interface functions to call to get information from the interpreter.

Kernel functions call each other. A few kernel functions are called by
interface functions.

Interface function names all begin with "Interp::".

Error handling is by returning a status value of either a non-error
code (INTERP_OK, INTERP_EXIT, etc.) or some specific error code
from each function where there is a possibility of error.  If an error
occurs, processing is always stopped, and control is passed back up
through the function call hierarchy to an interface function; the
error code is also passed back up. The stack of functions called is
also recorded. The external program calling an interface function may
then handle the error further as it sees fit.

Since returned values are usually used as just described to handle the
possibility of errors, an alternative method of passing calculated
values is required. In general, if function A needs a value for
variable V calculated by function B, this is handled by passing a
pointer to V from A to B, and B calculates and sets V.

There are a lot of functions named read_XXXX. All such functions read
characters from a string using a counter. They all reset the counter
to point at the character in the string following the last one used by
the function. The counter is passed around from function to function
by using pointers to it. The first character read by each of these
functions is expected to be a member of some set of characters (often
a specific character), and each function checks the first character.

This version of the interpreter not saving input lines. A list of all
lines will be needed in future versions to implement loops, and
probably for other purposes.

This version does not use any additional memory as it runs. No
memory is allocated by the source code.

This version does not suppress superfluous commands, such as a command
to start the spindle when the spindle is already turning, or a command
to turn on flood coolant, when flood coolant is already on.  When the
interpreter is being used for direct control of the machining center,
suppressing superfluous commands might confuse the user and could be
dangerous, but when it is used to translate from one file to another,
suppression can produce more concise output. Future versions might
include an option for suppressing superfluous commands.

****************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <libintl.h>
#include <fcntl.h>

#include "inifile.hh"		// INIFILE
#include "rs274ngc.hh"
#include "rs274ngc_return.hh"
#include "interp_internal.hh"	// interpreter private definitions
#include "interp_queue.hh"
#include "rs274ngc_interp.hh"
//#include "rs274ngc_errors.cc"

#include "units.h"

extern char * _rs274ngc_errors[];

#undef LOG_FILE

#define LOG_FILE &_setup.log_file[0]


Interp::Interp() 
    : log_file(0)
{}

Interp::~Interp() {
    if(log_file) {
	fclose(log_file);
	log_file = 0;
    }
}

void Interp::doLog(const char *fmt, ...)
{
#ifdef LOG_FILE
    struct timeval tv;
    struct tm *tm;
    va_list ap;

    if(log_file == NULL)
    {
       log_file = fopen(LOG_FILE, "a");
    }

    if(log_file == NULL)
    {
         fprintf(stderr, "(%d)Unable to open log file:%s\n",
                  getpid(), LOG_FILE);
         return;
    }

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    fprintf(log_file, "%04d%02d%02d-%02d:%02d:%02d.%03ld ",
	    tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
	    tm->tm_hour, tm->tm_min, tm->tm_sec,
	    tv.tv_usec/1000);

    va_start(ap, fmt);

    vfprintf(log_file, fmt, ap);
    fflush(log_file);

    va_end(ap);
#endif
}

/****************************************************************************/

/*

The functions in this section of this file are functions for
external programs to call to tell the rs274ngc interpreter
what to do. They are declared in rs274ngc.hh.

*/

/***********************************************************************/

/*! Interp::close

Returned Value: int (INTERP_OK)

Side Effects:
   The NC-code file is closed if open.
   The _setup world model is reset.

Called By: external programs

*/

int Interp::interclose()
{
  if(_setup.use_lazy_close)
    {
      _setup.lazy_closing = 1;
      return INTERP_OK;
    }

  if (_setup.file_pointer != NULL) {
    fclose(_setup.file_pointer);
    _setup.file_pointer = NULL;
    _setup.percent_flag = false;
  }
  reset();

  return INTERP_OK;
}

/***********************************************************************/

/*! Interp::execute

Returned Value: int)
   If execute_block returns INTERP_EXIT, this returns that.
   If execute_block returns INTERP_EXECUTE_FINISH, this returns that.
   If execute_block returns an error code, this returns that code.
   Otherwise, this returns INTERP_OK.

Side Effects:
   Calls to canonical machining commands are made.
   The interpreter variables are changed.
   At the end of the program, the file is closed.
   If using a file, the active G codes and M codes are updated.

Called By: external programs

This executes a previously parsed block.

*/

int Interp::_execute(const char *command)
{
  int status;
  int n;
  int MDImode = 0;

  if (NULL != command) {
    MDImode = 1;
    status = read(command);
    if (status != INTERP_OK) {
      return status;
    }
  }

  logDebug("MDImode = 1");
  logDebug("Interp::execute(%s)", command);
  // process control functions -- will skip if skipping
  //  if (_setup.block1.o_number != 0)
  if ((_setup.block1.o_number != 0) || (_setup.block1.o_name != 0) ||
          (_setup.mdi_interrupt))
    {
      logDebug("Convert control functions");
      CHP(convert_control_functions(&(_setup.block1), &_setup));

#if 1
      // let MDI code call subroutines.
      // !!!KL not clear what happens if last execution failed while in
      // !!!KL a subroutine

      // NOTE: the last executed file will still be open, because "close"
      // is really a lazy close.
      if (_setup.mdi_interrupt) {
	  _setup.mdi_interrupt = false;
	  MDImode = 1;
      }
      logDebug("!!!KL Open file is:%s:", _setup.filename);
      logDebug("MDImode = %d", MDImode);

      while(MDImode && _setup.call_level) // we are still in a subroutine
      {
          status = read(0);  // reads from current file and calls parse
          if (status != INTERP_OK)
	    {
               return status;
	    }
          status = execute();  // special handling for mdi errors
          if (status != INTERP_OK) {
		if (status == INTERP_EXECUTE_FINISH) {
		    _setup.mdi_interrupt = true;
		} else
		    reset();
               CHP(status);
          }
      }
      _setup.mdi_interrupt = false;
     if (MDImode)
	  FINISH();
#endif
      return INTERP_OK;
    }

  // skip if skipping
  if(_setup.skipping_o)
    {
      logDebug("skipping to: %s", _setup.skipping_o);
      return INTERP_OK;
    }
  for (n = 0; n < _setup.parameter_occurrence; n++)
  {  // copy parameter settings from parameter buffer into parameter table
    if(_setup.only_reading_flag == 0 || _setup.parameter_numbers[n] < 1000)
    _setup.parameters[_setup.parameter_numbers[n]]
          = _setup.parameter_values[n];
    if(_setup.parameter_numbers[n] == 4041)
        SET_LINEARAUX(0,_setup.parameter_values[n]);
    if(_setup.parameter_numbers[n] == 4042)
        SET_LINEARAUX(1,_setup.parameter_values[n]);
    if(_setup.parameter_numbers[n] == 4043)
        SET_LINEARAUX(2,_setup.parameter_values[n]);
    if(_setup.parameter_numbers[n] == 4044)
        SET_LINEARAUX(3,_setup.parameter_values[n]);
  }

  logDebug("_setup.named_parameter_occurrence = %d",
           _setup.named_parameter_occurrence);
  for (n = 0; n < _setup.named_parameter_occurrence; n++)
  {  // copy parameter settings from parameter buffer into parameter table

      logDebug("storing param");
      logDebug("storing param:|%s|", _setup.named_parameters[n]);
    CHP(store_named_param(_setup.named_parameters[n],
                          _setup.named_parameter_values[n]));

    // free the string
      logDebug("freeing param[%d]:|%s|:%p", n, _setup.named_parameters[n],
               _setup.named_parameters[n]);
    free(_setup.named_parameters[n]);
  }
  if(_setup.sequence_number == 1)
      _setup.startmove = 0;

  _setup.named_parameter_occurrence = 0;
 // if (EMC_DEBUG & EMC_DEBUG_INTERP
//    printf("now exec cmd id=%d ==================== %s\n",_setup.sequence_number,_setup.blocktext);

  if (_setup.line_length != 0) {        /* line not blank */
    status = execute_block(&(_setup.block1), &_setup);
    write_g_codes(&(_setup.block1), &_setup);
    write_m_codes(&(_setup.block1), &_setup);
    write_settings(&_setup);
    if ((status != INTERP_OK) &&
        (status != INTERP_EXECUTE_FINISH) && (status != INTERP_EXIT))
      ERP(status);

    if((_setup.block1.x_flag || _setup.block1.y_flag || _setup.block1.z_flag)
            && _setup.motion_to_be >= 0) {
        double a,b,c,u,v,w;
        find_ends_2(&(_setup.block1),&_setup,&_setup.get_read_end_x,
                    &_setup.get_read_end_y,&_setup.get_read_end_z,
                    &a,&b,&c,&u,&v,&w);
      //  printf("endxx=%.3f,endyy=%.3f,endzz=%.3f\n",_setup.get_read_end_x,_setup.get_read_end_y,_setup.get_read_end_z);

    }

  } else {                       /* blank line is OK */
    status = INTERP_OK;}
  return status;
}


int Interp::execute(const char *command) 
{
    int status;
    if ((status = _execute(command)) > INTERP_MIN_ERROR) {
	unwind_call(status, __FILE__,__LINE__);
    }
    return status;
}

int Interp::execute(const char *command, int line_number)
{

  _setup.sequence_number = line_number;
  return Interp::execute(command);
}

/***********************************************************************/

/*! Interp::exit

Returned Value: int (INTERP_OK)

Side Effects: See below

Called By: external programs

The system parameters are saved to a file and some parts of the world
model are reset. If GET_EXTERNAL_PARAMETER_FILE_NAME provides a
non-empty file name, that name is used for the file that is
written. Otherwise, the default parameter file name is used.

*/

int Interp::exit()
{
  char file_name[LINELEN];

  GET_EXTERNAL_PARAMETER_FILE_NAME(file_name, (LINELEN - 1));
  save_parameters(((file_name[0] ==
                             0) ?
                            RS274NGC_PARAMETER_FILE_NAME_DEFAULT :
                            file_name), _setup.parameters);
  reset();

  return INTERP_OK;
}

/***********************************************************************/

/*! rs274_ngc_init

Returned Value: int
   If any of the following errors occur, this returns the error code shown.
   Otherwise, this returns INTERP_OK.
   1. Interp::restore_parameters returns an error code.

Side Effects:
   Many values in the _setup structure are reset.
   A USE_LENGTH_UNITS canonical command call is made.
   A SET_FEED_REFERENCE canonical command call is made.
   A SET_ORIGIN_OFFSETS canonical command call is made.
   An INIT_CANON call is made.

Called By: external programs

Currently we are running only in CANON_XYZ feed_reference mode.  There
is no command regarding feed_reference in the rs274 language (we
should try to get one added). The initialization routine, therefore,
always calls SET_FEED_REFERENCE(CANON_XYZ).

*/

int Interp::init(const char * subNgcDir )
{
  int k;                        // starting index in parameters of origin offsets
  char filename[LINELEN];
  double *pars;                 // short name for _setup.parameters

  char *iniFileName;

  INIT_CANON();

  iniFileName = getenv("INI_FILE_NAME");

  // the default log file
  strcpy(&_setup.log_file[0], "emc_log");
  _setup.loggingLevel = 0;
  _setup.tool_change_at_g30 = 0;
  _setup.tool_change_quill_up = 0;
  _setup.tool_change_with_spindle_on = 0;
  _setup.a_axis_wrapped = 0;
  _setup.b_axis_wrapped = 0;
  _setup.c_axis_wrapped = 0;
  _setup.random_toolchanger = 0;
  _setup.a_indexer = 0;
  _setup.b_indexer = 0;
  _setup.c_indexer = 0;

  // not clear -- but this is fn is called a second time without an INI.
  if(NULL == iniFileName)
  {
      logDebug("INI_FILE_NAME not found");
  }
  else
  {
      IniFile inifile;

      logDebug("iniFileName:%s:", iniFileName);

      if (inifile.Open(iniFileName) == false) {
          logDebug("Unable to open inifile:%s:", iniFileName);
      }
      else
      {
          const char *inistring;

          inifile.Find(&_setup.tool_change_at_g30, "TOOL_CHANGE_AT_G30", "EMCIO");
          inifile.Find(&_setup.tool_change_quill_up, "TOOL_CHANGE_QUILL_UP", "EMCIO");
          inifile.Find(&_setup.tool_change_with_spindle_on, "TOOL_CHANGE_WITH_SPINDLE_ON", "EMCIO");
          inifile.Find(&_setup.a_axis_wrapped, "WRAPPED_ROTARY", "AXIS_3");
          inifile.Find(&_setup.b_axis_wrapped, "WRAPPED_ROTARY", "AXIS_4");
          inifile.Find(&_setup.c_axis_wrapped, "WRAPPED_ROTARY", "AXIS_5");
          inifile.Find(&_setup.random_toolchanger, "RANDOM_TOOLCHANGER", "EMCIO");

          inifile.Find(&_setup.a_indexer, "LOCKING_INDEXER", "AXIS_3");
          inifile.Find(&_setup.b_indexer, "LOCKING_INDEXER", "AXIS_4");
          inifile.Find(&_setup.c_indexer, "LOCKING_INDEXER", "AXIS_5");


          if(NULL != (inistring = inifile.Find("LOG_LEVEL", "RS274NGC")))
          {
              _setup.loggingLevel = atol(inistring);
          }
          if(NULL != (inistring = inifile.Find("Z_MODE", "RS274NGC")))
          {
              _setup.z_mode = atoi(inistring);
          }
          if(NULL != (inistring = inifile.Find("SPINDLE_SPEED_FLAG", "RS274NGC")))
          {
              _setup.s_mode = atoi(inistring);
          }
          if(NULL != (inistring = inifile.Find("AXES", "RS274NGC")))
          {
              _setup.axis_mode = atoi(inistring);
          } else {
              _setup.axis_mode = 3;             //default value is double_z
          }
          if(NULL != (inistring = inifile.Find("LOG_FILE", "RS274NGC")))
          {
	    // found it
            if (realpath(inistring, &_setup.log_file[0]) == NULL) {
        	//realpath didn't find the file
        	//nothing to do, checking for log_file[0] will report it later
    	    }
          }
        if(NULL != (inistring = inifile.Find("MAXZDIFF", "RS274NGC")))
        {
            _setup.maxzdiff = fabs(atof(inistring));
        }else{
            _setup.maxzdiff = 30;             //default value is 30
        }
          _setup.use_lazy_close = 1;

	  _setup.wizard_root[0] = 0;
          if(NULL != (inistring = inifile.Find("WIZARD_ROOT", "WIZARD")))
          {
	    logDebug("[WIZARD]WIZARD_ROOT:%s\n", inistring);
            if (realpath(inistring, _setup.wizard_root) == NULL) {
        	//realpath didn't find the file
        	logDebug("realpath failed to find wizard_root:%s:\n", inistring);
            }
          }
          logDebug("_setup.wizard_root:%s:\n", _setup.wizard_root);

	  _setup.program_prefix[0] = 0;
          if(NULL != (inistring = inifile.Find("PROGRAM_PREFIX", "DISPLAY")))
          {
	    // found it
            if (realpath(inistring, _setup.program_prefix) == NULL){
        	//realpath didn't find the file
        	logDebug("realpath failed to find program_prefix:%s:\n", inistring);    
            }
            logDebug("program prefix:%s: prefix:%s:",
		     inistring, _setup.program_prefix);
          }
          else
          {
	      logDebug("PROGRAM_PREFIX not found");
          }
          logDebug("_setup.program_prefix:%s:\n", _setup.program_prefix);


          if(NULL != (inistring = inifile.Find("SUBROUTINE_PATH", "RS274NGC")))
          {
            // found it
            int dct;
            char* nextdir;
            char tmpdirs[PATH_MAX+1];
            strcpy(_setup.subngcpath,inistring);
            for (dct=0; dct < MAX_SUB_DIRS; dct++) {
                 _setup.subroutines[dct][0] = 0;
            }

            strcpy(tmpdirs,inistring);
            nextdir = strtok(tmpdirs,":");  // first token
            dct = 0;
            while (1) {
                if (realpath(nextdir,_setup.subroutines[dct]) == NULL){
                   //realpath didn't find the file
                   logDebug("realpath failed to find subroutines[%d]:%s:\n",dct,nextdir);
                    _setup.subroutines[dct][0] = 0;
                } else {
                    logDebug("subroutine path[%d]:%s\n",dct,_setup.subroutines[dct]);
                }
                dct++;
                if (dct >= MAX_SUB_DIRS) {
                   logDebug("too many entries in SUBROUTINE_PATH, max=%d\n", MAX_SUB_DIRS);
                   break;
                }
                nextdir = strtok(NULL,":");
                if (nextdir == NULL) break; // no more tokens
             }
          }
          else
          {

              strcpy(_setup.subngcpath,"/home/.config/subNgc/");
              logDebug("SUBROUTINE_PATH not found");
          }
          logDebug("_setup.subroutines:%p:\n", _setup.subroutines);


/*
          for(int i=1;i<CANON_POCKETS_MAX;i++) {
              char parname[256];
              sprintf(parname,"TOOLPOS_X_%d",i);
             if(NULL != (inistring = inifile.Find(parname, "RS274"))) {
                 _setup.parameters[4300+5*i] = atof(inistring);
             } else
                 _setup.parameters[4300+5*i] = 0;

             sprintf(parname,"TOOLPOS_Y_%d",i);
            if(NULL != (inistring = inifile.Find(parname, "RS274"))) {
                _setup.parameters[4301+5*i] = atof(inistring);
            } else
                _setup.parameters[4301+5*i] = 0;

            sprintf(parname,"TOOLPOS_Z_%d",i);
           if(NULL != (inistring = inifile.Find(parname, "RS274"))) {
               _setup.parameters[4302+5*i] = atof(inistring);
           } else
               _setup.parameters[4302+5*i] = 0;
          }
          */
          // close it
          inifile.Close();
      }
  }

  if( subNgcDir != NULL ) {
       sprintf(_setup.program_prefix,subNgcDir);
  }

  _setup.length_units = GET_EXTERNAL_LENGTH_UNIT_TYPE();
  USE_LENGTH_UNITS(_setup.length_units);
  GET_EXTERNAL_PARAMETER_FILE_NAME(filename, LINELEN);
  if(0 != checkParameters(filename)) {
      char line[PATH_MAX];
      snprintf(line, sizeof(line), "%s%s", filename,
               RS274NGC_PARAMETER_FILE_BACKUP_SUFFIX);
      rename(line,filename);
      //print the error msg to the log file
      system("sed -i '1i var file check error' /home/.cnc/info");
  }
  if (filename[0] == 0)
    strcpy(filename, RS274NGC_PARAMETER_FILE_NAME_DEFAULT);
  CHP(restore_parameters(filename));
  pars = _setup.parameters;
  _setup.origin_index = (int) (pars[5220] + 0.0001);
  if(_setup.origin_index < 1 || _setup.origin_index > 9) {
      _setup.origin_index = 1;
      pars[5220] = 1.0;
  }

  k = (5200 + (_setup.origin_index * 20));
  _setup.origin_offset_x = USER_TO_PROGRAM_LEN(pars[k + 1]);
  _setup.origin_offset_y = USER_TO_PROGRAM_LEN(pars[k + 2]);
  _setup.origin_offset_z = USER_TO_PROGRAM_LEN(pars[k + 3]);
  _setup.AA_origin_offset = USER_TO_PROGRAM_ANG(pars[k + 4]);
  _setup.BB_origin_offset = USER_TO_PROGRAM_ANG(pars[k + 5]);
  _setup.CC_origin_offset = USER_TO_PROGRAM_ANG(pars[k + 6]);
  _setup.u_origin_offset = USER_TO_PROGRAM_LEN(pars[k + 7]);
  _setup.v_origin_offset = USER_TO_PROGRAM_LEN(pars[k + 8]);
  _setup.w_origin_offset = USER_TO_PROGRAM_LEN(pars[k + 9]);

  SET_G5X_OFFSET(_setup.origin_index,
                 _setup.origin_offset_x ,
                 _setup.origin_offset_y ,
                 _setup.origin_offset_z ,
                 _setup.AA_origin_offset,
                 _setup.BB_origin_offset,
                 _setup.CC_origin_offset,
                 _setup.u_origin_offset ,
                 _setup.v_origin_offset ,
                 _setup.w_origin_offset);

  if (pars[5210]) {
      _setup.axis_offset_x = USER_TO_PROGRAM_LEN(pars[5211]);
      _setup.axis_offset_y = USER_TO_PROGRAM_LEN(pars[5212]);
      _setup.axis_offset_z = USER_TO_PROGRAM_LEN(pars[5213]);
      _setup.AA_axis_offset = USER_TO_PROGRAM_ANG(pars[5214]);
      _setup.BB_axis_offset = USER_TO_PROGRAM_ANG(pars[5215]);
      _setup.CC_axis_offset = USER_TO_PROGRAM_ANG(pars[5216]);
      _setup.u_axis_offset = USER_TO_PROGRAM_LEN(pars[5217]);
      _setup.v_axis_offset = USER_TO_PROGRAM_LEN(pars[5218]);
      _setup.w_axis_offset = USER_TO_PROGRAM_LEN(pars[5219]);
  } else {
      _setup.axis_offset_x = 0.0;
      _setup.axis_offset_y = 0.0;
      _setup.axis_offset_z = 0.0;
      _setup.AA_axis_offset = 0.0;
      _setup.BB_axis_offset = 0.0;
      _setup.CC_axis_offset = 0.0;
      _setup.u_axis_offset = 0.0;
      _setup.v_axis_offset = 0.0;
      _setup.w_axis_offset = 0.0;
  }

  SET_G92_OFFSET(_setup.axis_offset_x ,
                 _setup.axis_offset_y ,
                 _setup.axis_offset_z ,
                 _setup.AA_axis_offset,
                 _setup.BB_axis_offset,
                 _setup.CC_axis_offset,
                 _setup.u_axis_offset ,
                 _setup.v_axis_offset ,
                 _setup.w_axis_offset);


  //_setup.rotation_xy = pars[k+10]; blocked by hong
  _setup.rotation_xy = 0;
  _setup.rotation_offsetx = 0;
  _setup.rotation_offsety = 0;

  SET_XY_ROTATION(_setup.rotation_xy,_setup.rotation_offsetx,_setup.rotation_offsety);
  SET_FEED_REFERENCE(CANON_XYZ);
//_setup.active_g_codes initialized below
//_setup.active_m_codes initialized below
//_setup.active_settings initialized below
//_setup.block1 does not need initialization
  _setup.blocktext[0] = 0;
//_setup.current_slot set in Interp::synch
//_setup.current_x set in Interp::synch
//_setup.current_y set in Interp::synch
//_setup.current_z set in Interp::synch
  _setup.cutter_comp_side = false;
  _setup.arc_not_allowed = false;
  _setup.cycle_il_flag = false;
  _setup.distance_mode = MODE_ABSOLUTE;
  _setup.ijk_distance_mode = MODE_INCREMENTAL;  // backwards compatability
  _setup.feed_mode = UNITS_PER_MINUTE;
//_setup.feed_override set in Interp::synch
//_setup.feed_rate set in Interp::synch
  _setup.filename[0] = 0;
  _setup.file_pointer = NULL;
//_setup.flood set in Interp::synch
  _setup.tool_offset_index = 0;
//_setup.length_units set in Interp::synch
  _setup.line_length = 0;
  _setup.linetext[0] = 0;
//_setup.mist set in Interp::synch
  _setup.motion_mode = G_80;
//_setup.origin_index set above
//_setup.parameters set above
//_setup.parameter_occurrence does not need initialization
//_setup.parameter_numbers does not need initialization
//_setup.parameter_values does not need initialization
//_setup.percent_flag does not need initialization
//_setup.plane set in Interp::synch
  _setup.probe_flag = false;
  _setup.toolchange_flag = false;
  _setup.input_flag = false;
  _setup.input_index = -1;
  _setup.input_digital = false;
  _setup.program_x = 0.;   /* for cutter comp */
  _setup.program_y = 0.;   /* for cutter comp */
  _setup.program_z = 0.;   /* for cutter comp */
  _setup.cutter_comp_firstmove = true;
//_setup.retract_mode does not need initialization
//_setup.selected_tool_slot set in Interp::synch
  _setup.sequence_number = 0;   /*DOES THIS NEED TO BE AT TOP? */
//_setup.speed set in Interp::synch
  _setup.speed_feed_mode = CANON_INDEPENDENT;
  _setup.spindle_mode = CONSTANT_RPM;
//_setup.speed_override set in Interp::synch
//_setup.spindle_turning set in Interp::synch
//_setup.stack does not need initialization
//_setup.stack_index does not need initialization
   ZERO_EMC_POSE(_setup.tool_offset);
//_setup.tool_max set in Interp::synch
//_setup.tool_table set in Interp::synch
//_setup.traverse_rate set in Interp::synch
//_setup.adaptive_feed set in Interp::synch
//_setup.feed_hold set in Interp::synch

  // initialization stuff for subroutines and control structures
  _setup.call_level = 0;
  _setup.defining_sub = 0;
  _setup.skipping_o = 0;
  _setup.oword_labels = 0;

  _setup.lathe_diameter_mode = false;
  _setup.m99_flag = false;
  _setup.only_reading_flag = 0;
  _setup.changez_flag = false;
  _setup.get_read_end_x = 0.0;
  _setup.get_read_end_y = 0.0;
  _setup.get_read_end_z = 0.0;
  _setup.motion_to_be = -1;
  _setup.g84_location = 0;
  _setup.g84_needcycle = 0;
  _setup.g84_cyclepval = 0;
  _setup.g84_cycleqval = 0;

  _setup.zdiff_flag = false;
  _setup.doublezdiff = 0;

  _setup.m91_flag = false;
  _setup.m70_flag = false;

  _setup.runfromline = 0;

  _setup.dynamicradius = 0.000;

  _setup.changcoorsys = false;

  _setup.breakflag = 0;

  _setup.startmove = 0;
  _setup.runafterset = 0;
  _setup.usegl = 0;


  ZERO_EMC_POSE(_setup.circlepoint[0]);
  ZERO_EMC_POSE(_setup.circlepoint[1]);
  ZERO_EMC_POSE(_setup.circlepoint[2]);
  _setup.points_in_circle = 0;

  _setup.selected_pocket = _setup.parameters[4301];
  CHANGE_TOOL_NUMBER(_setup.selected_pocket,0,0);
 // CHANGE_TOOL(_setup.selected_pocket);

  memcpy(_readers, default_readers, sizeof(default_readers));
  /*
  long axis_mask = GET_EXTERNAL_AXIS_MASK();

  if(!(axis_mask & AXIS_MASK_X)) _readers[(int)'x'] = 0;
  if(!(axis_mask & AXIS_MASK_Y)) _readers[(int)'y'] = 0;
  if(!(axis_mask & AXIS_MASK_Z)) _readers[(int)'z'] = 0;
  if(!(axis_mask & AXIS_MASK_A)) _readers[(int)'a'] = 0;
  if(!(axis_mask & AXIS_MASK_B)) _readers[(int)'b'] = 0;
  if(!(axis_mask & AXIS_MASK_C)) _readers[(int)'c'] = 0;
  if(!(axis_mask & AXIS_MASK_U)) _readers[(int)'u'] = 0;
  if(!(axis_mask & AXIS_MASK_V)) _readers[(int)'v'] = 0;
  if(!(axis_mask & AXIS_MASK_W)) _readers[(int)'w'] = 0;
  */
  synch(); //synch first, then update the interface

  write_g_codes((block_pointer) NULL, &_setup);
  write_m_codes((block_pointer) NULL, &_setup);
  write_settings(&_setup);

  init_tool_parameters();
  CHP(init_named_parameters());
  // Synch rest of settings to external world
  return INTERP_OK;
}

int Interp::init_named_parameters()
{
// version       major   minor      Note
// ------------ -------- ---------- -------------------------------------
// M.N.m         M.N     0.m        normal format
// M.N.m~xxx     M.N     0.m        pre-release format
  const char *pkgversion = PACKAGE_VERSION;  //examples: 2.4.6, 2.5.0~pre
  const char *version_major = "_vmajor";// named_parameter name (use lower case)
  const char *version_minor = "_vminor";// named_parameter name (use lower case)
  double vmajor=0.0, vminor=0.0;
  sscanf(pkgversion, "%lf%lf", &vmajor, &vminor);
  CHP( add_named_param((char*)version_major));
  CHP(init_named_param((char*)version_major,vmajor));
  CHP( add_named_param((char*)version_minor));
  CHP(init_named_param((char*)version_minor,vminor));

  return INTERP_OK;
}

void Interp::get_var(double *p)
{
// get the var parameters from _setup
    int n;
    for (n = 0; n < RS274NGC_MAX_PARAMETERS; n++) {
      p[n] = _setup.parameters[n];
    }
}

void Interp::set_only_reading_flag(int val)
{
    _setup.only_reading_flag = val;
}

int Interp::get_only_reading_flag()
{
    return _setup.only_reading_flag;
}

int Interp::get_comp_side()
{
    return _setup.cutter_comp_side;
}

bool Interp::get_xyz_flag()
{
    return (_setup.line_length > 0 && (_setup.block1.x_flag ||
            _setup.block1.y_flag || _setup.block1.z_flag) && _setup.motion_to_be >= 0 );
}

void Interp::set_choice_default_feed(double feed) {
    _setup.feed_default = feed;
    enqueue_SET_FEED_RATE(_setup.feed_default);
}

void Interp::interpStatusInit()
{
    _setup.loggingLevel = 0;
    _setup.tool_change_at_g30 = 0;
    _setup.tool_change_quill_up = 0;
    _setup.tool_change_with_spindle_on = 0;
    _setup.a_axis_wrapped = 0;
    _setup.b_axis_wrapped = 0;
    _setup.c_axis_wrapped = 0;
    _setup.random_toolchanger = 0;
    _setup.a_indexer = 0;
    _setup.b_indexer = 0;
    _setup.c_indexer = 0;
      _setup.blocktext[0] = 0;
      _setup.cutter_comp_side = false;
      _setup.arc_not_allowed = false;
      _setup.cycle_il_flag = false;
      _setup.distance_mode = MODE_ABSOLUTE;
      _setup.ijk_distance_mode = MODE_INCREMENTAL;  // backwards compatability
      _setup.feed_mode = UNITS_PER_MINUTE;
      _setup.line_length = 0;
      _setup.linetext[0] = 0;
      _setup.motion_mode = G_80;
      _setup.probe_flag = false;
      _setup.toolchange_flag = false;
      _setup.input_flag = false;
      _setup.input_index = -1;
      _setup.input_digital = false;
      _setup.program_x = 0.;   /* for cutter comp */
      _setup.program_y = 0.;   /* for cutter comp */
      _setup.program_z = 0.;   /* for cutter comp */
      _setup.cutter_comp_firstmove = true;
      _setup.sequence_number = 0;   /*DOES THIS NEED TO BE AT TOP? */
      _setup.speed_feed_mode = CANON_INDEPENDENT;
      _setup.spindle_mode = CONSTANT_RPM;
      _setup.tool_offset_index = 0;

      _setup.parameters[4041] = 0;
      _setup.parameters[4042] = 0;
      _setup.parameters[4043] = 0;
      _setup.parameters[4044] = 0;
      _setup.parameters[4060] = 0;
      STOP_CUTTER_RADIUS_COMPENSATION();

       ZERO_EMC_POSE(_setup.tool_offset);

       ZERO_EMC_POSE(_setup.circlepoint[0]);
       ZERO_EMC_POSE(_setup.circlepoint[1]);
       ZERO_EMC_POSE(_setup.circlepoint[2]);
       _setup.points_in_circle = 0;

      // initialization stuff for subroutines and control structures
      _setup.call_level = 0;
      _setup.defining_sub = 0;
      _setup.skipping_o = 0;
      _setup.oword_labels = 0;

      _setup.lathe_diameter_mode = false;
      _setup.m99_flag = false;
      _setup.only_reading_flag = 0;
      _setup.changcoorsys = false;

      _setup.changez_flag = false;
      _setup.get_read_end_x = 0.0;
      _setup.get_read_end_y = 0.0;
      _setup.get_read_end_z = 0.0;
      _setup.motion_to_be = -1;
      _setup.g84_location = 0;
      _setup.g84_needcycle = 0;
      _setup.g84_cyclepval = 0;
      _setup.g84_cycleqval = 0;

      _setup.zdiff_flag = false;
      _setup.doublezdiff = 0;

      _setup.m91_flag = false;
      _setup.m70_flag = false;

      _setup.runfromline = 0;
      _setup.startmove = 0;
      _setup.runafterset = 0;
      _setup.usegl = 0;

}

void Interp::m99_init()
{
    // initialization stuff for subroutines
    _setup.call_level = 0;
    _setup.defining_sub = 0;
    _setup.skipping_o = 0;
    _setup.oword_labels = 0;
    _setup.m99_flag = false;
    _setup.sequence_number = 0;
    _setup.percent_flag = false;
    _setup.parameters[4060] = 0;
    _setup.runafterset = 0;
    _setup.usegl = 0;

}

bool Interp::get_m99_flag()
{
    return _setup.m99_flag;
}

bool Interp::get_changez_flag()
{
    return _setup.changez_flag;
}

void Interp::clear_changez_flag()
{
    _setup.changez_flag = false;
}

void Interp::get_read_end_pos(double *x,double *y,double *z,int *gmode)
{
    *x = _setup.get_read_end_x;
    *y = _setup.get_read_end_y;
    *z = _setup.get_read_end_z;
    *gmode = _setup.block1.g_modes[0];
}

int *Interp::tool_exist()
{
    return &_setup.toolexist[0];
}

//hong 2014-03-22
//get the feed rate from the interp when in g1 mode
double Interp::get_feed_rate()
{
    return _setup.feed_rate / 60; //ghf-2014/6/18
}

void Interp::setParameter(int index, double val)
{
    _setup.parameters[index] = val;
    //synch();
}

void Interp::setSmartrunLine(int line) {
    _setup.runfromline = line;
}

void Interp::setFeedChoiceMode(int mode) {
    _setup.feed_choice_mode = mode;

    if (0 < _setup.feed_choice_mode) {
        SET_FEED_RATE(_setup.feed_default);
    }
}

void Interp::setFeedDefaultZ(double feedz,double feed) {
    _setup.feed_z_default = feedz;
    _setup.feed_default = feed;
}

/***********************************************************************/

/*! Interp::load_tool_table

Returned Value: int
   If any of the following errors occur, this returns the error code shown.
   Otherwise, this returns INTERP_OK.
   1. _setup.tool_max is larger than CANON_TOOL_MAX: NCE_TOOL_MAX_TOO_LARGE

Side Effects:
   _setup.tool_table[] is modified.

Called By:
   Interp::synch
   external programs

This function calls the canonical interface function GET_EXTERNAL_TOOL_TABLE
to load the whole tool table into the _setup.

The CANON_TOOL_MAX is an upper limit for this software. The
_setup.tool_max is intended to be set for a particular machine.

*/

int Interp::load_tool_table()
{
  int n;

  CHKS((_setup.pockets_max > CANON_POCKETS_MAX), NCE_POCKET_MAX_TOO_LARGE);
  for (n = 0; n < _setup.pockets_max; n++) {
    _setup.tool_table[n] = GET_EXTERNAL_TOOL_TABLE(n);
  }
  for (; n < CANON_POCKETS_MAX; n++) {
    _setup.tool_table[n].toolno = -1;
    ZERO_EMC_POSE(_setup.tool_table[n].offset);
    _setup.tool_table[n].diameter = 0;
    _setup.tool_table[n].orientation = 0;
    _setup.tool_table[n].frontangle = 0;
    _setup.tool_table[n].backangle = 0;
  }
  set_tool_parameters();
  return INTERP_OK;
}

/***********************************************************************/

/*! Interp::open

Returned Value: int
   If any of the following errors occur, this returns the error code shown.
   Otherwise it returns INTERP_OK.
   1. A file is already open: NCE_A_FILE_IS_ALREADY_OPEN
   2. The name of the file is too long: NCE_FILE_NAME_TOO_LONG
   3. The file cannot be opened: NCE_UNABLE_TO_OPEN_FILE

Side Effects: See below

Called By: external programs

The file is opened for reading and _setup.file_pointer is set.
The file name is copied into _setup.filename.
The _setup.sequence_number, is set to zero.
Interp::reset() is called, changing several more _setup attributes.

The manual [NCMS, page 3] discusses the use of the "%" character at the
beginning of a "tape". It is not clear whether it is intended that
every NC-code file should begin with that character.

In the following, "uses percents" means the first non-blank line
of the file must consist of nothing but the percent sign, with optional
leading and trailing white space, and there must be a second line
of the same sort later on in the file. If a file uses percents,
execution stops at the second percent line. Any lines after the
second percent line are ignored.

In this interpreter (recalling that M2 and M30 always ends execution):
1. If execution of a file is ended by M2 or M30 (not necessarily on
the last line of the file), then it is optional that the file
uses percents.
2. If execution of a file is not ended by M2 or M30, then it is
required that the file uses percents.

If the file being opened uses percents, this function turns on the
_setup.percent flag, reads any initial blank lines, and reads the
first line with the "%". If not, after reading enough to determine
that, this function puts the file pointer back at the beginning of the
file.

*/

int Interp::interopen(const char *filename) //!< string: the name of the input NC-program file
{
  char *line;
  int index;
  int length;

  if(_setup.use_lazy_close && _setup.lazy_closing)
    {
      _setup.use_lazy_close = 0; // so that close will work
      interclose();
      _setup.use_lazy_close = 1;
      _setup.lazy_closing = 0;
    }

  CHKS((_setup.file_pointer != NULL), NCE_A_FILE_IS_ALREADY_OPEN);
  CHKS((strlen(filename) > (LINELEN - 1)), NCE_FILE_NAME_TOO_LONG);
  _setup.file_pointer = fopen(filename, "r");
  CHKS((_setup.file_pointer == NULL), NCE_UNABLE_TO_OPEN_FILE, filename);
  line = _setup.linetext;
  char c = fgetc(_setup.file_pointer);
  if(c == EOF){
      strcpy(_setup.filename, filename);
      reset();
      return INTERP_OK;
  }
  for (index = -1; index == -1;) {      /* skip blank lines */
    CHKS((fgets(line, LINELEN, _setup.file_pointer) ==
         NULL), NCE_FILE_ENDED_WITH_NO_PERCENT_SIGN);
    length = strlen(line);
    if (length == (LINELEN - 1)) {   // line is too long. need to finish reading the line to recover
      for (; fgetc(_setup.file_pointer) != '\n';);      // could look for EOF
      ERS(NCE_COMMAND_TOO_LONG);
    }
    for (index = (length - 1);  // index set on last char
         (index >= 0) && (isspace(line[index])); index--);
  }
  if (line[index] == '%') {
    for (index--; (index >= 0) && (isspace(line[index])); index--);
    if (index == -1) {
    //  _setup.percent_flag = true;
      _setup.sequence_number = 1;       // We have already read the first line
      // and we are not going back to it.
    } else {
      fseek(_setup.file_pointer, 0, SEEK_SET);
      _setup.percent_flag = false;
      _setup.sequence_number = 0;       // Going back to line 0
    }
  } else {
    fseek(_setup.file_pointer, 0, SEEK_SET);
    _setup.percent_flag = false;
    _setup.sequence_number = 0; // Going back to line 0
  }
  strcpy(_setup.filename, filename);
  reset();
  return INTERP_OK;
}

/***********************************************************************/

/*! Interp::read

Returned Value: int
   If any of the following errors occur, this returns the error code shown.
   Otherwise, this returns:
       a. INTERP_ENDFILE if the only non-white character on the line is %,
       b. INTERP_EXECUTE_FINISH if the first character of the
          close_and_downcased line is a slash, and
       c. INTERP_OK otherwise.
   1. The command and_setup.file_pointer are both NULL: INTERP_FILE_NOT_OPEN
   2. The probe_flag is true but the HME command queue is not empty:
      NCE_QUEUE_IS_NOT_EMPTY_AFTER_PROBING
   3. If read_text (which gets a line of NC code from file) or parse_line
     (which parses the line) returns an error code, this returns that code.

Side Effects:
   _setup.sequence_number is incremented.
   The _setup.block1 is filled with data.

Called By: external programs

This reads a line of NC-code from the command string or, (if the
command string is NULL) from the currently open file. The
_setup.line_length will be set by read_text. This will be zero if the
line is blank or consists of nothing but a slash. If the length is not
zero, this parses the line into the _setup.block1.

*/

int Interp::_read(const char *command)  //!< may be NULL or a string to read
{
  static char name[] = "Interp::read";
  int read_status;
  if (_setup.probe_flag) {
    CHKS((GET_EXTERNAL_QUEUE_EMPTY() == 0),
        NCE_QUEUE_IS_NOT_EMPTY_AFTER_PROBING);
    set_probe_data(&_setup);
    _setup.probe_flag = false;
  }
  if (_setup.toolchange_flag) {
//    CHKS((GET_EXTERNAL_QUEUE_EMPTY() == 0),
//         _("Queue is not empty after tool change"));
    refresh_actual_position(&_setup);
    load_tool_table();
    _setup.toolchange_flag = false;
  }
  if (_setup.input_flag) {
    CHKS((GET_EXTERNAL_QUEUE_EMPTY() == 0),
        NCE_QUEUE_IS_NOT_EMPTY_AFTER_INPUT);
    if (_setup.input_digital) { // we are checking for a digital input
	_setup.parameters[5399] =
	    GET_EXTERNAL_DIGITAL_INPUT(_setup.input_index,
				      (_setup.parameters[5399] != 0.0));
    } else { // checking for analog input
	_setup.parameters[5399] =
	    GET_EXTERNAL_ANALOG_INPUT(_setup.input_index, _setup.parameters[5399]);
    }
    _setup.input_flag = false;
  }
  CHKN(((command == NULL) && (_setup.file_pointer == NULL)),
      INTERP_FILE_NOT_OPEN);

  _setup.parameters[5420] = _setup.current_x;
  _setup.parameters[5421] = _setup.current_y;
  _setup.parameters[5422] = _setup.current_z;
  _setup.parameters[5423] = _setup.AA_current;
  _setup.parameters[5424] = _setup.BB_current;
  _setup.parameters[5425] = _setup.CC_current;
  _setup.parameters[5426] = _setup.u_current;
  _setup.parameters[5427] = _setup.v_current;
  _setup.parameters[5428] = _setup.w_current;
  if(_setup.file_pointer)
  {
     _setup.block1.offset = ftell(_setup.file_pointer);
  }

   // printf("now get linetext ===== %s\n",_setup.linetext);
  read_status =
    read_text(command, _setup.file_pointer, _setup.linetext,
              _setup.blocktext, &_setup.line_length);
  if (read_status == INTERP_ERROR && _setup.skipping_to_sub) {
    free(_setup.skipping_to_sub);
    _setup.skipping_to_sub = 0;
  }
  if(command)logDebug("%s:[cmd]:|%s|", name, command);
  else logDebug("%s:|%s|", name, _setup.linetext);


  if ((read_status != INTERP_EXIT) &&
          ((read_status == INTERP_EXECUTE_FINISH)
      || (read_status == INTERP_OK))) {
    if (_setup.line_length != 0) {
      CHP(parse_line(_setup.blocktext, &(_setup.block1), &_setup));
    }

    else // Blank line (zero length)
    {
          /* RUM - this case reached when the block delete '/' character 
             is used, or READ_FULL_COMMENT is false and a comment is the
             only content of a line. 
             If a block o-type is in effect, block->o_number needs to be 
             incremented to allow o-extensions to work. 
             Note that the the block is 'refreshed' by init_block(),
             not created new, so this is a legal operation on block1. */
        if (_setup.block1.o_type != O_none)
        {
            // Clear o_type, this isn't line isn't a command...
            _setup.block1.o_type = 0;
            // increment o_number
            _setup.block1.o_number++;
        }
    }
  } else if (read_status == INTERP_ENDFILE);
  else
    ERP(read_status);
  return read_status;
}

int Interp::read(const char *command) 
{

    int status;
    if ((status = _read(command)) > INTERP_MIN_ERROR) {
	unwind_call(status, __FILE__,__LINE__);
    }
    return status;
}

// Reset interpreter state and  terminate a call in progress by
// falling back to toplevel in a controlled way. Idempotent.
// The input line (_setup.linetext,_setup.blocktext, _setup.line_length) 
// is left untouched for inspection post-error. This is only
// cleared in reset().
int Interp::unwind_call(int status, const char *file, int line)
{
    logDebug("unwind_call call_level=%d status=%d from %s:%d\n",
	    _setup.call_level, status, file, line);

    for(; _setup.call_level > 0; _setup.call_level--) {
	int i;
	context * sub = _setup.sub_context + _setup.call_level - 1;
	free_named_parameters(_setup.call_level, &_setup);
	if(sub->subName) {
	    logDebug("unwind_call leaving sub '%s'\n", sub->subName);
	    free(sub->subName);
	    sub->subName = 0;
	}

    for(i=0; i<INTERP_SUB_PARAMS; i++) {
	    _setup.parameters[i+INTERP_FIRST_SUBROUTINE_PARAM] =
		sub->saved_params[i];
	}

	// When called from Interp::close via Interp::reset, this one is NULL
	if (!_setup.file_pointer) continue;

	if(0 != strcmp(_setup.filename, sub->filename)) {
	    fclose(_setup.file_pointer);
	    _setup.file_pointer = fopen(sub->filename, "r");
	    logDebug("unwind_call: reopening '%s' at %ld\n",
		    sub->filename, sub->position);
	    strcpy(_setup.filename, sub->filename);
	}

	fseek(_setup.file_pointer, sub->position, SEEK_SET);

	_setup.sequence_number = sub->sequence_number;
	logDebug("unwind_call: setting sequence number=%d from frame %d\n",
		_setup.sequence_number,_setup.call_level);

    }

    if(_setup.sub_name) {
	logDebug("unwind_call exiting current sub '%s'\n", _setup.sub_name);
	free(_setup.sub_name);
	_setup.sub_name = 0;
    }
    _setup.call_level = 0;
    _setup.defining_sub = 0;
    _setup.skipping_o = 0;
    _setup.skipping_to_sub = 0;
    _setup.oword_labels = 0;

    _setup.mdi_interrupt = false;

    qc_reset();
    return INTERP_OK;
}

/***********************************************************************/

/*! Interp::reset

Returned Value: int (INTERP_OK)

Side Effects: See below

Called By:
   external programs
   Interp::close
   Interp::exit
   Interp::open

This function resets the parts of the _setup model having to do with
reading and interpreting one line. It does not affect the parts of the
model dealing with a file being open; Interp::open and Interp::close
do that.

There is a hierarchy of resetting the interpreter. Each of the following
calls does everything the ones above it do.

Interp::reset()
Interp::close()
Interp::init()

In addition, Interp::synch and Interp::restore_parameters (both of
which are called by Interp::init) change the model.

*/

int Interp::reset()
{
  //!!!KL According to the comment,
  //!!!KL this should not be here because this is for
  //!!!KL more than one line.
  //!!!KL But the comment seems wrong -- it is only called at open, close,
  //!!!KL init times which should affect the more global structure.
  //!!!KL (also called by external -- but probably OK)
  //
  // initialization stuff for subroutines and control structures

    _setup.linetext[0] = 0;
    _setup.blocktext[0] = 0;
    _setup.line_length = 0;
    unwind_call(INTERP_OK, __FILE__,__LINE__);
    return INTERP_OK;
}

int Interp::checkParameters(const char *filename)
{

    FILE *infile,*outfile;
    char line1[256],line2[256],bkname[PATH_MAX];
    if(-1 == access(filename,0))
        return -1;

    snprintf(bkname, sizeof(bkname), "%s%s", filename, RS274NGC_PARAMETER_FILE_BACKUP_SUFFIX);
    int ok = 0;
    infile = fopen(bkname,"r");
    outfile = fopen(filename,"r");
    if(fgets(line1, 256, outfile) == NULL )
        ok = -1;
/*    while (feof(outfile) == 0) {
        if (fgets(line1, 256, outfile) == NULL ||
            fgets(line2, 256, infile) == NULL ) {
          break;
        }
        if(0 != strncmp(line1,line2,strlen(line1))) {
            ok = -1;
            break;
        }

    }
    */
    fclose(infile);
    fclose(outfile);
    return ok;
}


/***********************************************************************/

/*! Interp::restore_parameters

Returned Value:
  If any of the following errors occur, this returns the error code shown.
  Otherwise it returns INTERP_OK.
  1. The parameter file cannot be opened for reading: NCE_UNABLE_TO_OPEN_FILE
  2. A parameter index is out of range: NCE_PARAMETER_NUMBER_OUT_OF_RANGE
  3. The parameter file is not in increasing order:
     NCE_PARAMETER_FILE_OUT_OF_ORDER

Side Effects: See below

Called By:
  external programs
  Interp::init

This function restores the parameters from a file, modifying the
parameters array. Usually parameters is _setup.parameters. The file
contains lines of the form:

<variable number> <value>

e.g.

5161 10.456

The variable numbers must be in increasing order, and certain
parameters must be included, as given in the _required_parameters
array. These are the axis offsets, the origin index (5220), and nine
sets of origin offsets. Any parameter not given a value in the file
has its value set to zero.

*/
int Interp::restore_parameters(const char *filename)   //!< name of parameter file to read  
{
  FILE *infile;
  char line[256];
  int variable;
  double value;
  int required;                 // number of next required parameter
  int index;                    // index into _required_parameters
  double *pars;                 // short name for _setup.parameters
  int k;

  // it's OK if the parameter file doesn't exist yet
  // it'll be created in due course with some default values
  if(access(filename, F_OK) == -1)
      return INTERP_OK;
  // open original for reading
  infile = fopen(filename, "r");
  CHKS((infile == NULL), _("Unable to open parameter file: '%s'"), filename);
  pars = _setup.parameters;
  k = 0;
  index = 0;
  required = _required_parameters[index++];
  while (feof(infile) == 0) {
    if (fgets(line, 256, infile) == NULL) {
      break;
    }
    // try for a variable-value match in the file
    if (sscanf(line, "%d %lf", &variable, &value) == 2) {
      CHKS(((variable <= 0)
           || (variable >= RS274NGC_MAX_PARAMETERS)),
          NCE_PARAMETER_NUMBER_OUT_OF_RANGE);
      for (; k < RS274NGC_MAX_PARAMETERS; k++) {
        if (k > variable) {
          fclose(infile);
          ERS(NCE_PARAMETER_FILE_OUT_OF_ORDER);
        } else if (k == variable) {
          pars[k] = value;
          if (k == required)
            required = _required_parameters[index++];
          k++;
          break;
        } else                  // if (k < variable)
        {
          if (k == required)
            required = _required_parameters[index++];
          pars[k] = 0;
        }
      }
    }
  }
  fclose(infile);
  for (; k < RS274NGC_MAX_PARAMETERS; k++) {
    pars[k] = 0;
  }
  return INTERP_OK;
}

/***********************************************************************/

/*! Interp::save_parameters

Returned Value:
  If any of the following errors occur, this returns the error code shown.
  Otherwise it returns INTERP_OK.
  1. The existing file cannot be renamed:  NCE_CANNOT_CREATE_BACKUP_FILE
  2. The renamed file cannot be opened to read: NCE_CANNOT_OPEN_BACKUP_FILE
  3. The new file cannot be opened to write: NCE_CANNOT_OPEN_VARIABLE_FILE
  4. A parameter index is out of range: NCE_PARAMETER_NUMBER_OUT_OF_RANGE
  5. The renamed file is out of order: NCE_PARAMETER_FILE_OUT_OF_ORDER

Side Effects: See below

Called By:
   external programs
   Interp::exit

A file containing variable-value assignments is updated. The old
version of the file is saved under a different name.  For each
variable-value pair in the old file, a line is written in the new file
giving the current value of the variable.  File lines have the form:

<variable number> <value>

e.g.

5161 10.456

If a required parameter is missing from the input file, this does not
complain, but does write it in the output file.

*/
int Interp::save_parameters(const char *filename,      //!< name of file to write
                             const double parameters[]) //!< parameters to save
{
    FILE *infile;
      FILE *outfile;
      char line[PATH_MAX];
      char bkfile[PATH_MAX];
      int variable;
      double value;
      int required;                 // number of next required parameter
      int index;                    // index into _required_parameters
      int k;
      char vardata [256*RS274NGC_MAX_PARAMETERS];
      memset(vardata,0,sizeof(vardata));
      if(access(filename, F_OK)==0)     //更新前先备份到.bak文件
      {
        int r;
        r = snprintf(line, sizeof(line), "%s%s", filename, RS274NGC_PARAMETER_FILE_BACKUP_SUFFIX);
        strcpy(bkfile,line);
        CHKS((r >= (int)sizeof(line)), NCE_CANNOT_CREATE_BACKUP_FILE);
        CHKS((rename(filename, line) != 0), NCE_CANNOT_CREATE_BACKUP_FILE);

        // open backup for reading
        infile = fopen(line, "r");
        CHKS((infile == NULL), NCE_CANNOT_OPEN_BACKUP_FILE);
      } else {
        infile = fopen("/dev/null", "r");
      }
      outfile = fopen(filename, "w");
      CHKS((outfile == NULL), NCE_CANNOT_OPEN_VARIABLE_FILE);
      k = 0;
      index = 0;
      required = _required_parameters[index++];

      while (feof(infile) == 0) {
        if (fgets(line, 256, infile) == NULL) {
          break;
        }
        if (sscanf(line, "%d %lf", &variable, &value) == 2) {
          CHKS(((variable <= 0)
               || (variable >= RS274NGC_MAX_PARAMETERS)),
              NCE_PARAMETER_NUMBER_OUT_OF_RANGE);
          for (; k < RS274NGC_MAX_PARAMETERS; k++) {
            if (k > variable) {             //参数不合理
                fsync(fileno(infile));
                fclose(infile);
                fflush(outfile);
                fsync(fileno(outfile));
                fclose(outfile);
                ERS(NCE_PARAMETER_FILE_OUT_OF_ORDER);
            } else if (k == variable) {         //文件内容和定义匹配上
              sprintf(line, "%d\t%f\n", k, parameters[k]);
              fputs(line, outfile);
              if (k == required)
                required = _required_parameters[index++];
                k++;
              break;
            } else if (k == required)       // know (k < variable)      //定义有文件没有，添加到文件里面
            {
              sprintf(line, "%d\t%f\n", k, parameters[k]);
              fputs(line, outfile);
              required = _required_parameters[index++];
            }
          }
        }
      }

      fsync(fileno(infile));
      fclose(infile);
      for (; k < RS274NGC_MAX_PARAMETERS; k++) {                //大于文件原有的参数，但定义也有的，需要补到文件里面
        if (k == required) {
          sprintf(line, "%d\t%f\n", k, parameters[k]);
          fputs(line, outfile);
          required = _required_parameters[index++];
        }
      }

      fflush(outfile);
      fsync(fileno(outfile));       //写入到硬盘
      fclose(outfile);
      if(0 != access(filename,0) || 0 != access(bkfile,0)) {
          return INTERP_OK;
      }
      return INTERP_OK;
}

/***********************************************************************/

/*! Interp::synch

Returned Value: int (INTERP_OK)

Side Effects:
   sets the value of many attribute of _setup by calling various
   GET_EXTERNAL_xxx functions.

Called By:
   Interp::init
   external programs

This function gets the _setup world model in synch with the rest of
the controller.

*/

int Interp::synch()
{

  char file_name[LINELEN];
  _setup.control_mode = GET_EXTERNAL_MOTION_CONTROL_MODE();
  _setup.current_pocket = GET_EXTERNAL_TOOL_SLOT();
  _setup.current_x = GET_EXTERNAL_POSITION_X();
  _setup.current_y = GET_EXTERNAL_POSITION_Y();
  _setup.current_z = GET_EXTERNAL_POSITION_Z();
  _setup.u_current = GET_EXTERNAL_POSITION_U();
  _setup.v_current = GET_EXTERNAL_POSITION_V();
  _setup.w_current = GET_EXTERNAL_POSITION_W();
  _setup.AA_current = GET_EXTERNAL_POSITION_A();
  _setup.BB_current = GET_EXTERNAL_POSITION_B();
  _setup.CC_current = GET_EXTERNAL_POSITION_C();
   _setup.CC_current =  _setup.CC_current - (int) _setup.CC_current/360*360;
  _setup.feed_rate = GET_EXTERNAL_FEED_RATE();
  _setup.flood = GET_EXTERNAL_FLOOD();
  _setup.length_units = GET_EXTERNAL_LENGTH_UNIT_TYPE();
  _setup.mist = GET_EXTERNAL_MIST();
  _setup.plane = GET_EXTERNAL_PLANE();
  //_setup.selected_pocket = GET_EXTERNAL_SELECTED_TOOL_SLOT();
  _setup.speed = GET_EXTERNAL_SPEED();
  _setup.spindle_turning = GET_EXTERNAL_SPINDLE();
  _setup.pockets_max = GET_EXTERNAL_POCKETS_MAX();
  _setup.traverse_rate = GET_EXTERNAL_TRAVERSE_RATE();
  _setup.feed_override = GET_EXTERNAL_FEED_OVERRIDE_ENABLE();
  _setup.speed_override = GET_EXTERNAL_SPINDLE_OVERRIDE_ENABLE();
  _setup.adaptive_feed = GET_EXTERNAL_ADAPTIVE_FEED_ENABLE();
  _setup.feed_hold = GET_EXTERNAL_FEED_HOLD_ENABLE();
  GET_EXTERNAL_PARAMETER_FILE_NAME(file_name, (LINELEN - 1));
  save_parameters(((file_name[0] ==
                             0) ?
                            RS274NGC_PARAMETER_FILE_NAME_DEFAULT :
                            file_name), _setup.parameters);

  load_tool_table();   /*  must set  _setup.tool_max first */

  int k = (5200 + (_setup.origin_index * 20));
  _setup.origin_offset_z = USER_TO_PROGRAM_LEN(_setup.parameters[k + 3]);

  return INTERP_OK;
}

/***********************************************************************/
/***********************************************************************/

/*

The functions in this section are to extract information from the
interpreter.

*/

/***********************************************************************/

/*! Interp::active_g_codes

Returned Value: none

Side Effects: copies active G codes into the codes array

Called By: external programs

See documentation of write_g_codes.

*/

void Interp::active_g_codes(int *codes)        //!< array of codes to copy into
{
  int n;

  for (n = 0; n < ACTIVE_G_CODES; n++) {
    codes[n] = _setup.active_g_codes[n];
  }
}

/***********************************************************************/

/*! Interp::active_m_codes

Returned Value: none

Side Effects: copies active M codes into the codes array

Called By: external programs

See documentation of write_m_codes.

*/

void Interp::active_m_codes(int *codes)        //!< array of codes to copy into
{
  int n;

  for (n = 0; n < ACTIVE_M_CODES; n++) {
    codes[n] = _setup.active_m_codes[n];
  }
}

/***********************************************************************/

/*! Interp::active_settings

Returned Value: none

Side Effects: copies active F, S settings into array

Called By: external programs

See documentation of write_settings.

*/

void Interp::active_settings(double *settings) //!< array of settings to copy into
{
  int n;

  for (n = 0; n < ACTIVE_SETTINGS; n++) {
    settings[n] = _setup.active_settings[n];
  }
}

static char savedError[LINELEN+1];
void Interp::setError(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);

    if(0 < _setup.sequence_number){
        char error[256];
        sprintf(error,_("Line %d:"),_setup.sequence_number);
        strcat(error,fmt);
        vsnprintf(savedError, LINELEN, error, ap);
    }else
        vsnprintf(savedError, LINELEN, fmt, ap);
    va_end(ap);
}

/***********************************************************************/

/*! Interp::error_text

Returned Value: none

Side Effects: see below

Called By: external programs

This copies the error string whose index in the _rs274ngc_errors array
is error_code into the error_text array -- unless the error_code is
an out-of-bounds index or the length of the error string is not less
than max_size, in which case an empty string is put into the
error_text. The length of the error_text array should be at least
max_size.

*/

void Interp::error_text(int error_code,        //!< code number of error                
                         char *error_text,      //!< char array to copy error text into  
                         int max_size)  //!< maximum number of characters to copy
{
    if(error_code == INTERP_ERROR)
    {
        strncpy(error_text, savedError, max_size);
        error_text[max_size-1] = 0;

        return;
    }

    error_text[0] = 0;
}

/***********************************************************************/

/*! Interp::file_name

Returned Value: none

Side Effects: see below

Called By: external programs

This copies the _setup.filename (the name of the currently open
file) into the file_name array -- unless the name is not shorter than
max_size, in which case a null string is put in the file_name array.


*/

void Interp::file_name(char *file_name,        //!< string: to copy file name into      
                        int max_size)   //!< maximum number of characters to copy
{
    if (strlen(_setup.filename) < ((size_t) max_size))
    strcpy(file_name, _setup.filename);
    else
    file_name[0] = 0;
}

/***********************************************************************/

/*! Interp::line_length

Returned Value: the length of the most recently read line

Side Effects: none

Called By: external programs

*/

int Interp::line_length()
{
  return _setup.line_length;
}

/***********************************************************************/

/*! Interp::line_text

Returned Value: none

Side Effects: See below

Called By: external programs

This copies at most (max_size - 1) non-null characters of the most
recently read line into the line_text string and puts a NULL after the
last non-null character.

*/

void Interp::line_text(char *line_text,        //!< string: to copy line into           
                        int max_size)   //!< maximum number of characters to copy
{
  int n;
  char *the_text;

  the_text = _setup.linetext;
  for (n = 0; n < (max_size - 1); n++) {
    if (the_text[n] != 0)
      line_text[n] = the_text[n];
    else
      break;
  }
  line_text[n] = 0;
}

/***********************************************************************/

/*! Interp::sequence_number

Returned Value: the current interpreter sequence number (how many
lines read since the last time the sequence number was reset to zero,
which happens only when Interp::init or Interp::open is called).

Side Effects: none

Called By: external programs

*/

int Interp::sequence_number()
{
  return _setup.sequence_number;
}

 int Interp::oword_saveline(int level) {
         if(_setup.call_level > 0)
        return _setup.sub_context[level].sequence_number;
     else
         return -1;
 }
/***********************************************************************/

/*! Interp::stack_name

Returned Value: none

Side Effects: see below

Called By: external programs

This copies at most (max_size - 1) non-null characters from the
string whose index in the _setup.stack array is stack_index into the
function_name string and puts a NULL after the last non-null character --
unless the stack_index is an out-of-bounds index, in which case an
empty string is put into the function_name.

This function is intended to be used several times in a row to get the
stack of function calls that existed when the most recent error
occurred. It should be called first with stack_index equal to 0,
next with stack_index equal to 1, and so on, stopping when an
empty string is returned for the name.

*/

void Interp::stack_name(int stack_index,       //!< index into stack of function names  
                         char *function_name,   //!< string: to copy function name into
                         int max_size)  //!< maximum number of characters to copy
{
  int n;
  char *the_name;

  if ((stack_index > -1) && (stack_index < STACK_LEN)) {
    the_name = _setup.stack[stack_index];
    for (n = 0; n < (max_size - 1); n++) {
      if (the_name[n] != 0)
        function_name[n] = the_name[n];
      else
        break;
    }
    function_name[n] = 0;
  } else
    function_name[0] = 0;
}

/***********************************************************************/

/* Interp::ini_load()

Returned Value: INTERP_OK, RS274NGC_ERROR

Side Effects:
   An INI file containing values for global variables is used to
   update the globals

Called By:
   Interp::init()

The file looks like this:

[RS274NGC]
VARIABLE_FILE = rs274ngc.var

*/

int Interp::ini_load(const char *filename)
{
    IniFile inifile;
    const char *inistring;

    // open it
    if (inifile.Open(filename) == false) {
        logDebug("Unable to open inifile:%s:", filename);
	return -1;
    }

    logDebug("Opened inifile:%s:", filename);


    if (NULL != (inistring = inifile.Find("PARAMETER_FILE", "RS274NGC"))) {
	// found it
	strncpy(_parameter_file_name, inistring, LINELEN);
        if (_parameter_file_name[LINELEN-1] != '\0') {
            logDebug("%s:[RS274NGC]PARAMETER_FILE is too long (max len %d)", filename, LINELEN-1);
            inifile.Close();
            _parameter_file_name[0] = '\0';
            return -1;
        }
        logDebug("found PARAMETER_FILE:%s:", _parameter_file_name);
    } else {
	// not found, leave RS274NGC_PARAMETER_FILE alone
        logDebug("did not find PARAMETER_FILE");
    }



    inifile.Find(&_setup.doublezXdistance,"ROTATION_X_DISTANCE","RS274NGC");
    _setup.parameters[4120] = _setup.doublezXdistance;
    inifile.Find(&_setup.doublezYdistance,"ROTATION_Y_DISTANCE","RS274NGC");
    _setup.parameters[4121] = _setup.doublezYdistance;
    inifile.Find(&_setup.doublezZdistance,"ROTATION_Z_DISTANCE","RS274NGC");
    _setup.parameters[4122] = _setup.doublezZdistance;
        printf("now I get the rotation coor %f  %f  %f\n",_setup.doublezXdistance,_setup.doublezYdistance,_setup.doublezZdistance);

    inifile.Find(&_setup.feed_choice_mode, "FEED_CHOICE_MODE", "OPERATOR");
    if(NULL != (inistring = inifile.Find("DEFAULT_VELOCITY", "TRAJ")))
    {
       _setup.feed_default = atof(inistring)*60;
    }else{
        _setup.feed_default = 100*60;             //default value is double_z
        printf("Cannot read the default feed \n");
    }
    if(NULL != (inistring = inifile.Find("DEFAULT_FEED_Z_VELOCITY", "TRAJ")))
    {
       _setup.feed_z_default = atof(inistring)*60;
    }else{
        _setup.feed_z_default = 100*60;             //default value is double_z
        printf("Cannot read the default z feed \n");
    }
    if (0 < _setup.feed_choice_mode) {
        SET_FEED_RATE(_setup.feed_default);
    }
    // close it
    inifile.Close();

    return 0;
}

int Interp::init_tool_parameters()
{
  if (_setup.random_toolchanger) {
     // random_toolchanger: tool at startup expected
    _setup.parameters[5400] = _setup.tool_table[0].toolno;
    _setup.parameters[5401] = _setup.tool_table[0].offset.tran.x;
    _setup.parameters[5402] = _setup.tool_table[0].offset.tran.y;
    _setup.parameters[5403] = _setup.tool_table[0].offset.tran.z;
    _setup.parameters[5404] = _setup.tool_table[0].offset.a;
    _setup.parameters[5405] = _setup.tool_table[0].offset.b;
    _setup.parameters[5406] = _setup.tool_table[0].offset.c;
    _setup.parameters[5407] = _setup.tool_table[0].offset.u;
    _setup.parameters[5408] = _setup.tool_table[0].offset.v;
    _setup.parameters[5409] = _setup.tool_table[0].offset.w;
    _setup.parameters[5410] = _setup.tool_table[0].diameter;
    _setup.parameters[5411] = _setup.tool_table[0].frontangle;
    _setup.parameters[5412] = _setup.tool_table[0].backangle;
    _setup.parameters[5413] = _setup.tool_table[0].orientation;
  } else {
    // non random_toolchanger: no tool at startup, one-time init
    if (_setup.tool_table[0].toolno == -1) {
      default_tool_parameters();
    }
  }
  return 0;
}

int Interp::default_tool_parameters()
{
  _setup.parameters[5400] =  0; // toolno
  _setup.parameters[5401] =  0; // x offset
  _setup.parameters[5402] =  0; // y offset RESERVED
  _setup.parameters[5403] =  0; // z offset
  _setup.parameters[5404] =  0; // a offset RESERVED
  _setup.parameters[5405] =  0; // b offset RESERVED
  _setup.parameters[5406] =  0; // c offset RESERVED
  _setup.parameters[5407] =  0; // u offset RESERVED
  _setup.parameters[5408] =  0; // v offset RESERVED
  _setup.parameters[5409] =  0; // w offset RESERVED
  _setup.parameters[5410] =  0; // diameter
  _setup.parameters[5411] =  0; // frontangle
  _setup.parameters[5412] =  0; // backangle
  _setup.parameters[5413] =  0; // orientation
  return 0;
}

int Interp::set_tool_parameters()
{
  // invoke to set tool parameters for current tool (pocket==0)
  // when a tool is absent, set default (zero offset) tool parameters

  if ((! _setup.random_toolchanger) && (_setup.tool_table[0].toolno <= 0)) {
    default_tool_parameters();
    return 0;
  }
  _setup.parameters[5400] = _setup.tool_table[0].toolno;
  _setup.parameters[5401] = _setup.tool_table[0].offset.tran.x;
  _setup.parameters[5402] = _setup.tool_table[0].offset.tran.y;
  _setup.parameters[5403] = _setup.tool_table[0].offset.tran.z;
  _setup.parameters[5404] = _setup.tool_table[0].offset.a;
  _setup.parameters[5405] = _setup.tool_table[0].offset.b;
  _setup.parameters[5406] = _setup.tool_table[0].offset.c;
  _setup.parameters[5407] = _setup.tool_table[0].offset.u;
  _setup.parameters[5408] = _setup.tool_table[0].offset.v;
  _setup.parameters[5409] = _setup.tool_table[0].offset.w;
  _setup.parameters[5410] = _setup.tool_table[0].diameter;
  _setup.parameters[5411] = _setup.tool_table[0].frontangle;
  _setup.parameters[5412] = _setup.tool_table[0].backangle;
  _setup.parameters[5413] = _setup.tool_table[0].orientation;

  return 0;
}

void Interp::set_current_z(double val)
{
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxin currentz %.3f\n",_setup.current_z);
    _setup.current_z = val;
}

void Interp::set_current_x(double val)
{
    _setup.current_x = val;

    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxin currentx %.3f\n",_setup.current_x);
}
void Interp::set_current_y(double val)
{
    _setup.current_y = val;
}

void Interp::set_current_a(double val)
{
    _setup.AA_current = val;
}

double Interp::get_interp_var(int _index)
{
    if(_index > RS274NGC_MAX_PARAMETERS)
        return 0;
    return _setup.parameters[_index];
}

void Interp::set_interp_var(int _index,double _val)
{
    if(_index > RS274NGC_MAX_PARAMETERS)
        return ;
    _setup.parameters[_index] = _val;
}

void Interp::set_gl_flag(int _flag) {
    _setup.usegl = _flag;
}
