/*
 * Keeps a file mapped in memory,
 * and active by steadily reading from it.
 *
 * This Works is placed under the terms of the Copyright Less License,
 * see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>

#include "keepcached_version.h"

/* basically to spare some mempory
 */

static void
on(const char *s, int fd)
{
  int	i;

  for (i=0; s[i]; i++);
  if (i)
    write(fd, s, i);
}

static void
o(const char *s)
{
  on(s, 0);
}

static void
oint5(int n)
{
  char	buf[100];
  int	i;

  i		= sizeof buf;
  buf[--i]	= 0;
  while (i>=0 && n)
    {
      buf[--i]	= '0'+(n%10);
      n		/= 10;
    }
  while (i>sizeof buf-6)
    buf[--i]	= '0';
  o(buf+i);
}

static void
o2(const char *s)
{
  on(s, 2);
}

static void
err(const char *s)
{
  int	e;

  e	= errno;
  o2("error: ");
  o2(s);
  o2(": ");
  o2(strerror(e));
  o2("\n");
}

static void
ex(const char *s)
{
  err(s);
  exit(1);
}

int
main(int argc, char **argv)
{
  int		fd;
  size_t	l;
  volatile void	*p;
  int		a, round, max;

  if (argc!=2)
    {
      o2("Usage: keepcached file\n"
	 "\t\tVersion " KEEPCACHED_VERSION " compiled " __DATE__ "\n"
	 "\tMaps file to memory and keep it cached into memory\n");
      return 2;
    }

  fd	= open(argv[1], O_RDONLY);
  if (fd<0)
    ex(argv[1]);

  l	= 0;
  p	= 0;
  max	= 0;

  a	= 0;
  for (round=0;; round++)
    {
      struct stat	st;
      time_t		now;
      int		i;

      if (fstat(fd, &st))
	ex("stat");
      if (st.st_size!=l)
	{
	  if (p)
	    munmap((void *)p, l);
	  l	= st.st_size;
	  p	= mmap(NULL, l, PROT_READ, MAP_SHARED, fd, (off_t)0);
	  if (!p)
	    ex("mmap");
	  max	= 0;
	}
      printf("\r%c", "|/-\\"[round&3]);
      fflush(stdout);
      time(&now);
      for (i=0; i<l && i<=max; i+=BUFSIZ)
	  a	+= ((volatile unsigned char *)p)[i];
      if (time(NULL)-now>1)
	{
	  max	/= 2;
	  printf("<%lu ", (unsigned long)(1+((l-max)>>20)));
	}
      else if (max<l)
	{
	  printf(">%lu ", (unsigned long)(1+((l-max)>>20)));
	  max	+= 1<<20;
	}
      else
	printf(" ok");
      usleep(100000ul);
      a	%= 2;
    }
  return 0;
}
