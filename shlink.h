#ifndef _SHLINK_H
#define _SHLINK_H

void* shlink_attach (const char *fnm);
int shlink_detach (void *addr);
void* shlink_alloc (const char *fnm, int size);
int shlink_free (void);

#endif
