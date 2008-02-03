
/*
 * pffs -- piece flash image file editor
 * by Yui N., 2005.
 * refrences copyrighted by OeRSTED / AQUAPLUS:
 *   /usr/piece/sysdev/pcekn/file.c
 *                           piece.h
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "psi.h"
#include "../pfi.h"

#include "CAuPFIFile.h"
#include "CAuPFFS.h"

#include <vector>
using namespace std;

typedef vector<char*> vpChar;
typedef vpChar::iterator ivpChar;

int main(int ac, char** av)
{
  CAuPFFS pffs;
  LPSTR szFileName = NULL;
  vpChar vsArguments;
  char cAction = 0;

  while(*++av)
  {
    char* p = *av;
    if(!szFileName && *p != '-')
    {
      szFileName = p;
      continue;
    }
    if(*p != '-')
    {
      vsArguments.push_back(p);
      continue;
    }
    switch(*(p + 1))
    {
      case 'a':        // add file(s).
      case 'd':        // delete file(s).
      case 'e':        // extract file(s).
      case 'l':        // list files
      case 'v':        // show sysinfo
        cAction = *(p + 1);
        break;
      default:
        fprintf(stderr, "invalid option -%c, skipping.\n", *(p + 1));
    }
  }

  if(szFileName && pffs.Open(szFileName) && cAction)
  {
    switch(cAction)
    {
      case 'a':        // add file(s).
      {
        BOOL bSuccess = TRUE;
        for(ivpChar i = vsArguments.begin(); i != vsArguments.end(); i++)
        {
          printf("Adding %s...", *i);
          if(pffs.AddFile(*i))
          {
            printf("ok\n");
          }
          else
          {
            printf("failed, skipping\n");
            bSuccess = FALSE;
          }
        }
        if(bSuccess && vsArguments.size() > 0)
        {
          printf("Writing modifications to %s...", szFileName);
          pffs.GetPFIFile()->SaveToFile(szFileName);
          printf("ok\n");
        }
        break;
      }
      case 'd':        // delete file(s).
      {
        BOOL bSuccess = TRUE;
        for(ivpChar i = vsArguments.begin(); i != vsArguments.end(); i++)
        {
          printf("Deleting %s...", *i);
          if(pffs.DeleteFile(*i))
          {
            printf("ok\n");
          }
          else
          {
            printf("failed, skipping\n");
            bSuccess = FALSE;
          }
        }
        if(bSuccess && vsArguments.size() > 0)
        {
          printf("Writing modifications to %s...", szFileName);
          pffs.GetPFIFile()->SaveToFile(szFileName);
          printf("ok\n");
        }
        break;
      }
      case 'e':        // extract file(s).
        for(ivpChar i = vsArguments.begin(); i != vsArguments.end(); i++)
        {
          printf("Extracting %s...", *i);
          if(pffs.ExtractFile(*i))
            printf("ok\n");
          else
            printf("failed, skipping\n");
        }
        break;
      case 'l':        // list files
        pffs.DumpDirEntries();
        break;
      case 'v':        // show sysinfo
        CAuPFIFile::DumpSystemInfo(pffs.GetPFIFile()->GetSystemInfo());
    }
    pffs.Close();
  }
  else
  {
    fprintf(stderr, "Usage: pffs piece.pfi -adelv [file [...]]\n");
  }
  return 0;
}
