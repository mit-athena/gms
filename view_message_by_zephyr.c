/* This file is part of the Project Athena Global Message System.
 * Created by: Mark W. Eichin <eichin@athena.mit.edu>
 * $Source: /afs/dev.mit.edu/source/repository/athena/bin/gms/view_message_by_zephyr.c,v $
 * $Author: eichin $
 *
 *	Copyright (c) 1988 by the Massachusetts Institute of Technology.
 *	For copying and distribution information, see the file
 *	"mit-copyright.h". 
 */
#include <mit-copyright.h>
#ifndef lint
static char rcsid_view_message_by_zephyr_c[] = "$Header: /afs/dev.mit.edu/source/repository/athena/bin/gms/view_message_by_zephyr.c,v 1.1 1988-09-26 15:22:09 eichin Exp $";
#endif lint

#include "globalmessage.h"
#include <pwd.h>
#include <stdio.h>
#include <strings.h>
#include <syslog.h>

void view_message_by_zephyr(message)
     char *message;
{
  char *whoami, *getlogin();
  char *ptr;
  
  whoami = getlogin();

  if(!whoami) {
    struct passwd *pw;
    pw = getpwuid(getuid());
    if(pw) {
      whoami = pw->pw_name;
    } else {
      fprintf(stderr,
	      "get_message: couldn't find username to send zephyr notice\n");
      exit(2);
    }
  }
  /* skip magic headers */
  ptr = index(message, '\n')+1;
  
  /* check that there is *something* after the headers */
  if(*ptr) {
    /* don't even fork... this just exits anyway... */
    execl("/usr/athena/zwrite",
	  "zwrite", "-d", "-q", "-n",  whoami, "-m", ptr, 0);
    /* put logging here in case the exec fails. */
    syslog(LOG_INFO, "GMS client execl of zwrite failed [%s]",
	   error_message(errno));
  }
}
