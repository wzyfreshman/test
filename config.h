/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/********************************************************************
* Description: config.h
*
*	Common defines used in many emc2 source files.
*       To add to this file, edit Section 4.1 in configure.in
*       and then run autoheader.
*
* Author: Autogenerated by autoheader
* License: LGPL Version 2
* System: Linux
*    
* Copyright (c) 2004 All rights reserved.
********************************************************************/
#ifndef EMC2_CONFIG_H
#define EMC2_CONFIG_H

/* LINELEN is used throughout for buffer sizes, length of file name strings,
   etc. Let's just have one instead of a multitude of defines all the same. */
#define LINELEN 255
/* Used in a number of places for sprintf() buffers. */
#define BUFFERLEN 80

#define MM_PER_INCH 25.4
#define INCH_PER_MM (1.0/25.4)


/* build hardware drivers */
#define BUILD_DRIVERS /**/

/* build modules through kbuild */
/* #undef BUILD_SYS_KBUILD */

/* build normal (unused) */
/* #undef BUILD_SYS_NORMAL */

/* build modules as .so shared objects */
#define BUILD_SYS_USER_DSO /**/

/* Directory for binaries */
#define EMC2_BIN_DIR "/home/.cnc/bin"

/* Directory for configuration files */
#define EMC2_CONFIG_DIR ""

/* Default nml file */
#define EMC2_DEFAULT_NMLFILE "/home/.cnc/share/linuxcnc/linuxcnc.nml"

/* Default nml file */
#define EMC2_DEFAULT_TOOLTABLE ""

/* Directory for help files */
#define EMC2_HELP_DIR "/home/.cnc/share/doc/linuxcnc"

/* Prefix or RIP directory */
#define EMC2_HOME "/home/.cnc"

/* Directory for images */
#define EMC2_IMAGE_DIR "/home/.cnc/share/linuxcnc"

/* Directory for tcl translation files */
#define EMC2_LANG_DIR "/home/.cnc/share/linuxcnc/tcl/msgs"

/* Directory for nc files */
#define EMC2_NCFILES_DIR "/home/.cnc/share/linuxcnc/ncfiles"

/* Directory for po/mo translation files */
#define EMC2_PO_DIR "/home/.cnc/share/locale"

/* Directory of realtime system */
#define EMC2_RTLIB_DIR "/home/.cnc/modules"

#define SIMULATE 1

#define SIMULATE_IGNORE 0

/* Directory for tcl scripts */
#define EMC2_TCL_DIR "/home/.cnc/lib/tcltk/linuxcnc"

/* Git version tag */
#define GIT_VERSION "v~~"

/* Define to 1 if you have the `clock_getres' function. */
#define HAVE_CLOCK_GETRES 1

/* Define to 1 if you have the <GL/glu.h> header file. */
/* #undef HAVE_GL_GLU_H */

/* Define to 1 if you have the <GL/gl.h> header file. */
/* #undef HAVE_GL_GL_H */

/* Define to 1 if you have the <inttypes.h> header file. */
/* #undef HAVE_INTTYPES_H */

/* Define to 1 if you have the `GL' library (-lGL). */
/* #undef HAVE_LIBGL */

/* Define to 1 if you have the `intl' library (-lintl). */
/* #undef HAVE_LIBINTL */

/* Define to 1 if you have the <libintl.h> header file. */
#define HAVE_LIBINTL_H 1

/* Define to 1 if you have the `ncurses' library (-lncurses). */
/* #undef HAVE_LIBNCURSES */

/* libudev installed and available */
#define HAVE_LIBUDEV /**/

/* Define to 1 if you have the `Xaw' library (-lXaw). */
/* #undef HAVE_LIBXAW */

/* Define to 1 if you have the `Xinerama' library (-lXinerama). */
#define HAVE_LIBXINERAMA 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the <memory.h> header file. */
/* #undef HAVE_MEMORY_H */

/* Define to 1 if you have the <ncurses.h> header file. */
/* #undef HAVE_NCURSES_H */

/* Define to 1 if you have the 'readline' library (-lreadline) and required
   headers */
#define HAVE_READLINE /**/

/* Define to 1 if you have the <readline/history.h> header file. */
#define HAVE_READLINE_HISTORY_H 1

/* Define to 1 if you have the <readline/readline.h> header file. */
#define HAVE_READLINE_READLINE_H 1

/* Define to 1 if you have the `semtimedop' function. */
#define HAVE_SEMTIMEDOP 1

/* Define to 1 if you have the `setlocale' function. */
#define HAVE_SETLOCALE 1

/* Define to 1 if sincos is available. */
#define HAVE_SINCOS 1

/* Define to 1 if you have the <stdint.h> header file. */
/* #undef HAVE_STDINT_H */

/* Define to 1 if you have the <stdlib.h> header file. */
/* #undef HAVE_STDLIB_H */

/* Define to 1 if you have the <strings.h> header file. */
/* #undef HAVE_STRINGS_H */

/* Define to 1 if you have the <string.h> header file. */
/* #undef HAVE_STRING_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
/* #undef HAVE_SYS_STAT_H */

/* Define to 1 if you have the <sys/types.h> header file. */
/* #undef HAVE_SYS_TYPES_H */

/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <tcl.h> header file. */
/* #undef HAVE_TCL_H */

/* Define to 1 if you have the <tk.h> header file. */
/* #undef HAVE_TK_H */

/* Define to 1 if you have the <unistd.h> header file. */
/* #undef HAVE_UNISTD_H */

/* Define to 1 if you have the <X11/extensions/Xinerama.h> header file. */
#define HAVE_X11_EXTENSIONS_XINERAMA_H 1

/* Define to 1 if you have the <X11/Xaw/XawInit.h> header file. */
/* #undef HAVE_X11_XAW_XAWINIT_H */

/* Define to 1 if you have the <X11/Xmu/Xmu.h> header file. */
/* #undef HAVE_X11_XMU_XMU_H */

/* Define to 1 if you have the `__sincos' function. */
/* #undef HAVE___SINCOS */

/* Define to 1 if linux/hidraw.h is usable and defines HIDIOCGRAWINFO */
#define HIDRAW_H_USABLE /**/

/* Extension for realtime modules */
#define MODULE_EXT ".so"

/* Define to 1 if asm/msr.h is usable and defines rdtscll */
/* #undef MSR_H_USABLE */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "emc-developers@lists.sourceforge.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "LinuxCNC"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "LinuxCNC 2.5.2"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "linuxcnc"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.5.2"

/* Required kernel version */
/* #undef RTAPI_KERNEL_VERSION */

/* thread style posix */
/* #undef RTAPI_POSIX */

/* thread style rtai */
/* #undef RTAPI_RTAI */

/* thread style rt-preempt-kernel */
/* #undef RTAPI_RTPREEMPT_KERNEL */

/* thread style rt-preempt-user */
#define RTAPI_RTPREEMPT_USER /**/

/* thread style xenomai-kernel */
/* #undef RTAPI_XENOMAI_KERNEL */

/* thread style xenomai-user */
/* #undef RTAPI_XENOMAI_USER */

/* Directory of the installed realtime system */
#define RTDIR ""

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* build for BeagleBone platform */
/* #undef TARGET_PLATFORM_BEAGLEBONE */

/* build for PC platform */
#define TARGET_PLATFORM_PC /**/

/* build for Raspberry Pi platform */
/* #undef TARGET_PLATFORM_RASPBERRY */

/* thread system settings header file */
#define THREADS_HEADERS "rtapi/rt-preempt-user.h"

/* build PCI drivers with usermode PCI support */
#define USERMODE_PCI /**/

/* use GCC builtin atomic operations */
/* #undef USE_GCC_ATOMIC_OPS */

/* use ppdev ioctl instead of inb/outb */
/* #undef USE_PORTABLE_PARPORT_IO */

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif


#endif

