/*****************************************************************************
 * Description: inifile.cc
 *   C++ INI file reader
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
 *****************************************************************************/

#include <stdio.h>              /* FILE *, fopen(), fclose(), NULL */
#include <stdlib.h>
#include <string.h>             /* strstr() */
#include <ctype.h>              /* isspace() */
#include <fcntl.h>


#include "config.h"
#include "inifile.hh"

#define KEY_SIZE 5000 

/// Return TRUE if the line has a line-ending problem
static bool check_line_endings(const char *s) {
    if(!s) return false;
    for(; *s; s++ ) {
        if(*s == '\r') {
            char c = s[1];
            if(c == '\n' || c == '\0') {
                static bool warned = 0;
                if(!warned) 
                    fprintf(stderr, "inifile: warning: File contains DOS-style line endings.\n");
                warned = true;
                continue;
            }
            fprintf(stderr, "inifile: error: File contains ambiguous carriage returns\n");
            return true;
        }
    }
    return false;
}

IniFile::IniFile(int _errMask, FILE *_fp)
{
    fp = _fp;
    errMask = _errMask;
    owned = false;

    if(fp != NULL)
        LockFile();
}


/*! Opens the file for reading. If a file was already open, it is closed
   and the new one opened.

   @return true on success, false on failure */
bool
IniFile::Open(const char *file)
{
    char                        path[LINELEN] = "";

    if(IsOpen()) Close();

    TildeExpansion(file, path);

    if((fp = fopen(path, "r")) == NULL) {
        return(true);
    }

    owned = true;

    if(!LockFile()) {
        return(true);
    }


    return(true);
}


/*! Closes the file descriptor..

   @return true on success, false on failure */
bool
IniFile::Close()
{
    int                         rVal = 0;

    if(fp != NULL){
        lock.l_type = F_UNLCK;
        fcntl(fileno(fp), F_SETLKW, &lock);

        if(owned)
            rVal = fclose(fp);

        fp = NULL;
    }

    return(rVal == 0);
}


IniFile::ErrorCode                   
IniFile::Find(int *result, int min, int max,
              const char *tag, const char *section, int num)
{
    ErrorCode                   errCode;
    int                         tmp;

    if((errCode = Find(&tmp, tag, section, num)) != ERR_NONE)
        return(errCode);

    if((tmp > max) || (tmp < min))
        return(ERR_LIMITS);

    *result = tmp;

    return(ERR_NONE);
}


IniFile::ErrorCode                   
IniFile::Find(int *result, const char *tag, const char *section, int num)
{
    const char                  *pStr;
    int                         tmp;

    if((pStr = Find(tag, section, num)) == NULL){
        // We really need an ErrorCode return from Find() and should be passing
        // in a buffer. Just pick a suitable ErrorCode for now.
        return(ERR_TAG_NOT_FOUND);
    }

    if(sscanf(pStr, "%i", &tmp) != 1){
        ThrowException(ERR_CONVERSION);
        return(ERR_CONVERSION);
    }

    *result = tmp;

    return(ERR_NONE);
}


IniFile::ErrorCode                   
IniFile::Find(double *result, double min, double max,
              const char *tag, const char *section, int num)
{
    ErrorCode                   errCode;
    double                      tmp;

    if((errCode = Find(&tmp, tag, section, num)) != ERR_NONE)
        return(errCode);

    if((tmp > max) || (tmp < min))
        return(ERR_LIMITS);

    *result = tmp;

    return(ERR_NONE);
}


IniFile::ErrorCode                   
IniFile::Find(double *result, const char *tag, const char *section, int num)
{
    const char                  *pStr;
    double                      tmp;

    if((pStr = Find(tag, section, num)) == NULL){
        // We really need an ErrorCode return from Find() and should be passing
        // in a buffer. Just pick a suitable ErrorCode for now.
        return(ERR_TAG_NOT_FOUND);
    }

    if(sscanf(pStr, "%lf", &tmp) != 1){
        ThrowException(ERR_CONVERSION);
        return(ERR_CONVERSION);
    }

    *result = tmp;

    return(ERR_NONE);
}


IniFile::ErrorCode
IniFile::Find(int *result, StrIntPair *pPair,
     const char *tag, const char *section, int num)
{
    const char                  *pStr;
    int                         tmp;

    if((pStr = Find(tag, section, num)) == NULL){
        // We really need an ErrorCode return from Find() and should be passing
        // in a buffer. Just pick a suitable ErrorCode for now.
        return(ERR_TAG_NOT_FOUND);
    }

    if(sscanf(pStr, "%i", &tmp) == 1){
        *result = tmp;
        return(ERR_NONE);
    }

    while(pPair->pStr != NULL){
        if(strcasecmp(pStr, pPair->pStr) == 0){
            *result = pPair->value;
            return(ERR_NONE);
        }
        pPair++;
    }

    ThrowException(ERR_CONVERSION);
    return(ERR_CONVERSION);
}


IniFile::ErrorCode
IniFile::Find(double *result, StrDoublePair *pPair,
     const char *tag, const char *section, int num)
{
    const char                  *pStr;
    double                      tmp;

    if((pStr = Find(tag, section, num)) == NULL){
        // We really need an ErrorCode return from Find() and should be passing
        // in a buffer. Just pick a suitable ErrorCode for now.
        return(ERR_TAG_NOT_FOUND);
    }

    if(sscanf(pStr, "%lf", &tmp) == 1){
        *result = tmp;
        return(ERR_NONE);
    }

    while(pPair->pStr != NULL){
        if(strcasecmp(pStr, pPair->pStr) == 0){
            *result = pPair->value;
            return(ERR_NONE);
        }
        pPair++;
    }

    ThrowException(ERR_CONVERSION);
    return(ERR_CONVERSION);
}


/*! Finds the nth tag in section.

   @param tag Entry in the ini file to find.

   @param section The section to look for the tag.

   @param num (optionally) the Nth occurrence of the tag.

   @return pointer to the the variable after the '=' delimiter */
const char *
IniFile::Find(const char *_tag, const char *_section, int _num)
{
    // WTF, return a pointer to the middle of a local buffer?
    // FIX: this is totally non-reentrant.
    static char                 line[LINELEN + 2] = "";        /* 1 for newline, 1 for NULL */
    char                        bracketSection[LINELEN + 2] = "";
    char                        *nonWhite;
    int                         newLinePos;                /* position of newline to strip */
    int                         len;
    char                        tagEnd;
    char                        *valueString;
    char                        *endValueString;

    // For exceptions.
    lineNo = 0;
    char key[KEY_SIZE];
    char value[100];
    num = _num;
    
    if(strncmp(_section,"IO_",3)){
    if(sizeof(_tag)+sizeof(_section) > KEY_SIZE-1){
    	fprintf(stderr, "parameter name is too long,then return!");
    	return(NULL);
    }
    sprintf(key,"PAR_%s_%s",_section,_tag);
    strcpy(value,"VALUE");
    section = key;
    tag = value;
    }else {
    	tag = _tag;
    	section = _section;
    }
    //fprintf(stderr, "section = %s   %s\n",section,tag);

    /* check valid file */
    if(!CheckIfOpen())
        return(NULL);

    /* start from beginning */
    rewind(fp);

    /* check for section first-- if it's non-NULL, then position file at
       line after [section] */
    if(section != NULL){
        sprintf(bracketSection, "[%s]", section);

        /* find [section], and position fp just after it */
        while (!feof(fp)) {

            if (NULL == fgets(line, LINELEN + 1, fp)) {
                /* got to end of file without finding it */
                ThrowException(ERR_SECTION_NOT_FOUND);
                return(NULL);
            }

            if(check_line_endings(line)) {
                ThrowException(ERR_CONVERSION);
                return(NULL);
            }

            /* got a line */
            lineNo++;

            /* strip off newline */
            newLinePos = strlen(line) - 1;        /* newline is on back from 0 */
            if (newLinePos < 0) {
                newLinePos = 0;
            }
            if (line[newLinePos] == '\n') {
                line[newLinePos] = 0;        /* make the newline 0 */
            }

            if (NULL == (nonWhite = SkipWhite(line))) {
                /* blank line-- skip */
                continue;
            }

            /* not a blank line, and nonwhite is first char */
            if (strncmp(bracketSection, nonWhite, strlen(bracketSection)) != 0){
                /* not on this line */

                continue;
            }

            /* it matches-- fp is now set up for search on tag */
            break;
        }
    }

    while (!feof(fp)) {
        /* check for end of file */
        if (NULL == fgets(line, LINELEN + 1, (FILE *) fp)) {
            /* got to end of file without finding it */
            ThrowException(ERR_TAG_NOT_FOUND);
            return(NULL);
        }

        if(check_line_endings(line)) {
            ThrowException(ERR_CONVERSION);
            return(NULL);
        }

        /* got a line */
        lineNo++;

        /* strip off newline */
        newLinePos = strlen(line) - 1;        /* newline is on back from 0 */
        if (newLinePos < 0) {
            newLinePos = 0;
        }
        if (line[newLinePos] == '\n') {
            line[newLinePos] = 0;        /* make the newline 0 */
        }

        /* skip leading whitespace */
        if (NULL == (nonWhite = SkipWhite(line))) {
            /* blank line-- skip */
            continue;
        }

        /* check for '[' char-- if so, it's a section tag, and we're out
           of our section */
        if (NULL != section && nonWhite[0] == '[') {
            ThrowException(ERR_TAG_NOT_FOUND);
            return(NULL);
        }

        len = strlen(tag);
        if (strncmp(tag, nonWhite, len) != 0) {
            /* not on this line */
            continue;
        }

        /* it matches the first part of the string-- if whitespace or = is
           next char then call it a match */
        tagEnd = nonWhite[len];
        if (tagEnd == ' ' || tagEnd == '\r' || tagEnd == '\t'
            || tagEnd == '\n' || tagEnd == '=') {
            /* it matches-- return string after =, or NULL */
            if (--_num > 0) {
                /* Not looking for this one, so skip it... */
                continue;
            }
            nonWhite += len;
            valueString = AfterEqual(nonWhite);
            /* Eliminate white space at the end of a line also. */
            if (NULL == valueString) {
                ThrowException(ERR_TAG_NOT_FOUND);
                return(NULL);
            }
            endValueString = valueString + strlen(valueString) - 1;
            while (*endValueString == ' ' || *endValueString == '\t'
                   || *endValueString == '\r') {
                *endValueString = 0;
                endValueString--;
            }
            return(valueString);
        }
        /* else continue */
    }

    ThrowException(ERR_TAG_NOT_FOUND);
    return(NULL);
}


bool
IniFile::CheckIfOpen(void)
{
    if(IsOpen())
        return(true);

    ThrowException(ERR_NOT_OPEN);

    return(false);
}


bool
IniFile::LockFile(void)
{
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if(fcntl(fileno(fp), F_SETLK, &lock) == -1){
        if(owned)
            fclose(fp);

        fp = NULL;
        return(false);
    }

    return(true);
}


/*! Expands the tilde to $(HOME) and concatenates file to it. If the first char
    If file does not start with ~/, file will be copied into path as-is. 

   @param the input filename

   @param pointer for returning the resulting expanded name

 */
void
IniFile::TildeExpansion(const char *file, char *path)
{
    char                        *home;

    strncpy(path, file, LINELEN);
    if (strlen(file) < 2 || !(file[0] == '~' && file[1] == '/')) {
	/* no tilde expansion required, or unsupported
           tilde expansion type requested */
	return;
    }

    home = getenv("HOME");
    if (!home || strlen(home) + strlen(file) > LINELEN) {
	return;
    }

    /* Buffer overflow has already been checked. */

    strcpy(path, home);
    strcat(path, file + 1);
    return;
}


void
IniFile::ThrowException(ErrorCode errCode)
{
    if(errCode & errMask){
        exception.errCode = errCode;
        exception.tag = tag;
        exception.section = section;
        exception.num = num;
        exception.lineNo = lineNo;
        throw(exception);
    }
}


/*! Ignoring any tabs, spaces or other white spaces, finds the first
   character after the '=' delimiter.

   @param string Pointer to the tag

   @return NULL or pointer to first non-white char after the delimiter

   Called By: find() and section() only. */
char *
IniFile::AfterEqual(const char *string)
{
    const char                  *spot = string;

    for (;;) {
        if (*spot == '=') {
            /* = is there-- return next non-white, or NULL if not there */
            for (;;) {
                spot++;
                if (0 == *spot) {
                    /* ran out */
                    return(NULL);
                } else if (*spot != ' ' && *spot != '\t' && *spot != '\r'
                           && *spot != '\n') {
                    /* matched! */
                    return((char *)spot);
                } else {
                    /* keep going for the text */
                    continue;
                }
            }
        } else if (*spot == 0) {
            /* end of string */
            return(NULL);
        } else {
            /* haven't seen '=' yet-- keep going */
            spot++;
            continue;
        }
    }
}


/*! Finds the first non-white character on a new line and returns a
   pointer. Ignores any line that starts with a comment char i.e. a ';' or 
   '#'.

   @return NULL if not found or a valid pointer.

   Called By: find() and section() only. */
char *
IniFile::SkipWhite(const char *string)
{
    while(true){
        if (*string == 0) {
            return(NULL);
        }

        if ((*string == ';') || (*string == '#')) {
            return(NULL);
        }

        if (*string != ' ' && *string != '\t' && *string != '\r'
            && *string != '\n') {
            return((char *)string);
        }

        string++;
    }
}


void
IniFile::Exception::Print(FILE *fp)
{
    const char                  *msg;

    switch(errCode){
    case ERR_NONE:
        msg = "ERR_NONE";
        break;

    case ERR_NOT_OPEN:
        msg = "ERR_NOT_OPEN";
        break;

    case ERR_SECTION_NOT_FOUND:
        msg = "ERR_SECTION_NOT_FOUND";
        break;

    case ERR_TAG_NOT_FOUND:
        msg = "ERR_TAG_NOT_FOUND";
        break;

    case ERR_CONVERSION:
        msg = "ERR_CONVERSION";
        break;

    case ERR_LIMITS:
        msg = "ERR_LIMITS";
        break;

    default:
        msg = "UNKNOWN";
    }

    fprintf(fp, "INIFILE: %s, section=%s, tag=%s, num=%d, lineNo=%d\n",
            msg, section, tag, num, lineNo);
}


extern "C" const char *
iniFind(FILE *fp, const char *tag, const char *section)
{
    IniFile                     f(false, fp);

    return(f.Find(tag, section));
}
