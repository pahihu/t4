extern int verbose;

#ifdef T4SHLINKS

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include "shlink.h"

static HANDLE hMapFile;

void *shlink_attach (const char *fnm, int size)
{
        char szName[256];
        void *addr = NULL;

        strcpy (szName, "T4SharedLinks");

        hMapFile = OpenFileMapping (
                        FILE_MAP_ALL_ACCESS,   // read/write access
                        FALSE,                 // do not inherit the name
                        szName);               // name of mapping object

        if (hMapFile == NULL)
        {
                printf ("-E-EMU414: Could not open file mapping object %s (%d).\n",
                        szName,
                        GetLastError());
                return NULL;
        }

        addr = (LPTSTR) MapViewOfFile (hMapFile,      // handle to map object
                                FILE_MAP_ALL_ACCESS,  // read/write permission
                                0,
                                0,
                                size);

        if (addr == NULL)
        {
                printf ("-E-EMU414: Could not map view of file (%d).\n", GetLastError());
                CloseHandle(hMapFile);
                return NULL;
        }

        if (verbose)
                printf ("-I-EMU414: Attached SHMLinks at #%p.\n", addr);

        return addr;
}

int shlink_detach (void *addr)
{
        UnmapViewOfFile (addr);
        CloseHandle(hMapFile);
        return 0;
}

void *shlink_alloc (const char *fnm, int size)
{
        char szName[256];
        void *addr = NULL;

        strcpy (szName, "T4SharedLinks");

        hMapFile = CreateFileMapping (
                        INVALID_HANDLE_VALUE,    // use paging file
                        0,     			 // default security
                        PAGE_READWRITE,          // read/write access
                        0,                       // maximum object size (high-order DWORD)
                        size,                    // maximum object size (low-order DWORD)
                        szName);                 // name of mapping object

        if (hMapFile == NULL)
        {
                printf ("-E-EMU414: Could not create file mapping object %s (%d).\n",
                        szName,
                        GetLastError ());
                return NULL;
        }

        addr = (LPTSTR) MapViewOfFile (hMapFile,     // handle to map object
                                FILE_MAP_ALL_ACCESS, // read/write permission
                                0,
                                0,
                                size);

        if (addr == NULL)
        {
                printf ("-E-EMU414: Could not map view of file (%d).\n",
                        GetLastError ());
                CloseHandle (hMapFile);
                return NULL;
        }

        if (verbose)
                printf ("-I-EMU414: Allocated SHMLinks at #%p.\n", addr);
        return addr;
}

int shlink_free (void)
{
        return 0;
}

#else

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

extern int verbose;
int shmid;

void* shlink_attach (const char *fnm, int size)
{
        key_t shmkey;
        int ret;
        void *addr;

        shmkey = ftok(fnm, 1984);
        if (shmkey == -1)
        {
                ret = errno;
                printf ("-E-EMU414: Cannot access %s (%s).\n", fnm, strerror (ret));
                return NULL;
        }
        shmid = shmget (shmkey, 0, 0);
        if (shmid == -1)
        {
                ret = errno;
                printf ("-E-EMU414: Cannot get SHMLinks (%s).\n", strerror (ret));
                return NULL;
        }

        addr = shmat (shmid, 0, 0);
        if (addr == (void *)-1)
        {
                ret = errno;
                printf ("-E-EMU414: Unable to attach SHMLinks (%s).\n", strerror (ret));
                return NULL;
        }
        if (verbose)
                printf ("-I-EMU414: Attached SHMLinks at #%p.\n", addr);
        return addr;
}

int shlink_detach (void *addr)
{
        shmdt (addr);
        return 0;
}

void* shlink_alloc (const char *fnm, int size)
{
        key_t shmkey;
        struct shmid_ds sbuf;
        void *addr;

        shmkey = ftok (fnm, 1984);
        if (shmkey == -1)
        {
                printf ("-E-EMU414: Cannot access %s (%s).\n", fnm, strerror (errno));
                return NULL;
        }
        shmid = shmget (shmkey, 0, 0);
        if (shmid != -1)
        {
                printf ("-E-EMU414: SHMLinks already exists.\n");
                return NULL;
        }

        shmid = shmget (shmkey, size, (SHM_R|SHM_W|(SHM_R>>3) |(SHM_W>>3)|IPC_CREAT));
        if (shmid == -1)
        {
                printf ("-E-EMU414: Unable to create SHMLinks (%s).\n", strerror (errno));
                return NULL;
        }

        addr = shmat (shmid, 0, 0);
        if (addr == (void *)-1)
        {
                printf ("-E-EMU414: Unable to attach to SHMLinks (%s).\n", strerror (errno));
                shmctl (shmid, (IPC_RMID), &sbuf);
                return NULL;
        }

        memset (addr, 0, size);

        if (verbose)
                printf ("-I-EMU414: Allocated SHMLinks at #%p.\n", addr);
        return addr;
}

int shlink_free (void)
{
        struct shmid_ds sbuf;
        return shmctl (shmid, (IPC_RMID), &sbuf);
}

#endif

#endif
