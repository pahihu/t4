#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

extern int verbose;
int shmid;

void* shlink_attach (const char *fnm)
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
                printf ("-E-EMU414: Cannot get SharedChannels (%s).\n", strerror (ret));
                return NULL;
        }

        addr = shmat (shmid, 0, 0);
        if (addr == (void *)-1)
        {
                ret = errno;
                printf ("-E-EMU414: Unable to attach SharedChannels (%s).\n", strerror (ret));
                return NULL;
        }
        if (verbose)
                printf ("-I-EMU414: Attached SharedChannels at #%p.\n", addr);
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
        int i;
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
                printf ("-E-EMU414: SharedChannels already exists.\n");
                return NULL;
        }

        shmid = shmget (shmkey, size, (SHM_R|SHM_W|(SHM_R>>3) |(SHM_W>>3)|IPC_CREAT));
        if (shmid == -1)
        {
                printf ("-E-EMU414: Unable to create SharedChannels (%s).\n", strerror (errno));
                return NULL;
        }

        addr = shmat (shmid, 0, 0);
        if (addr == (void *)-1)
        {
                printf ("-E-MU414: Unable to attach to SharedChannels (%s).\n", strerror (errno));
                i = shmctl (shmid, (IPC_RMID), &sbuf);
                return NULL;
        }

        if (verbose)
                printf ("-I-EMU414: Allocated SharedChannels at #%p.\n", addr);
        return addr;
}

int shlink_free (void)
{
        struct shmid_ds sbuf;
        return shmctl (shmid, (IPC_RMID), &sbuf);
}

