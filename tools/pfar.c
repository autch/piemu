/** @file pfar.c
 * P/ECE Flash image ARchiver
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "pffs.h"
#include "pfi.h"

int run(PFI* pfi, char* pPFIfilename, char action, char** args);
int usage();

int main(int ac, char** av)
{
  PFI pfi;
  char* pFilename = NULL;
  char action = 'l';
  char** args = NULL;

  while(*++av)
  {
    char* p = *av;
    if(!pFilename && *p != '-')
    {
      pFilename = p;
      continue;
    }
    if(*p != '-' && !args)
    {
      args = av;
      break;
    }
    switch(*(p + 1))
    {
    case 'a':        // add file(s).
    case 'd':        // delete file(s).
    case 'e':        // extract file(s).
    case 'l':        // list files
    case 'v':        // show sysinfo
      action = *(p + 1);
      break;
    default:
      fprintf(stderr, "invalid option -%c, skipping.\n", *(p + 1));
    }
  }

  if(action && pFilename)
  {
    PFIInit(&pfi);

    if(!PFIOpen(&pfi, pFilename))
    {
      fprintf(stderr, "pffs: unable to open pfi file: %s\n", pFilename);
      PFIExit(&pfi);
      return EXIT_FAILURE;
    }
    run(&pfi, pFilename, action, args);

    PFIClose(&pfi);
    PFIExit(&pfi);
  }
  else
  {
    return usage();
  }

  return EXIT_SUCCESS;
}

int usage()
{
  fprintf(stderr, "Usage: pfar piece.pfi -adelv [file [...]]\n");
  return EXIT_FAILURE;
}

int run(PFI* pfi, char* pPFIfilename, char action, char** args)
{
  int nfiles = 0;
  
  switch(action)
  {
  case 'a':        // add file(s).
  {
    if(!args) return usage();
    for(; *args; args++)
    {
      printf("Adding %s...", *args);
      if(PFFSAddFile(pfi, *args))
      {
	printf("ok\n");
	nfiles++;
      }
      else
      {
	printf("failed, aborting\n");
	return -1;
      }
    }
    if(nfiles > 0)
    {
      printf("Writing modifications to %s...", pPFIfilename);
      PFISave(pfi, pPFIfilename);
      printf("done\n");
    }
    break;
  }
  case 'd':        // delete file(s).
  {
    if(!args) return usage();
    for(; *args; args++)
    {
      printf("Deleting %s...", *args);
      if(PFFSDeleteFile(pfi, *args))
      {
	printf("ok\n");
	nfiles++;
      }
      else
      {
	printf("failed, aborting\n");
	return -1;
      }
    }
    if(nfiles > 0)
    {
      printf("Writing modifications to %s...", pPFIfilename);
      PFISave(pfi, pPFIfilename);
      printf("ok\n");
    }
    break;
  }
  case 'e':        // extract file(s).
    if(!args) return usage();
    for(; *args; args++)
    {
      printf("Extracting %s...", *args);
      if(PFFSExtractFile(pfi, *args, *args))
	printf("ok\n");
      else
	printf("failed, skipping\n");
    }
    break;
  case 'l':        // list files
    PFIDumpSystemInfo(pfi);
    printf("\n");
    PFFSDumpDirEntries(pfi);
    break;
  case 'v':        // show sysinfo
    PFIDumpSystemInfo(pfi);
    break;
  default:
    return usage();
    break;
  }
  return 0;
}
