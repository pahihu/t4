/***************************************/
/* definiere Strukturen fuer Filecache */
/***************************************/

#define MAX_CACHE_SIZE (1024*1024*4)

typedef struct cacheentry
{
        FILE *                  os_fd;         /* Filedescriptor von MS-DOS */
        char *                  name;          /* Filename */
        long                    size;          /* Filegroesse */
        char *                  filecache;     /* zeiger auf Filecache */
        int                     fcount;        /* wieviel mal werde ich benutzt */
        struct cacheentry *     prev;          /* Vorgaenger */
        struct cacheentry *     next;          /* Nachfolger */
} CacheEntry, *CacheEntryPtr;

typedef struct handlebuffer
{
        FILE *                  my_handle;      /* welches handle habe ich */
        long                    my_pos;         /* position im file von begin */
        BOOL                    my_eof;         /* EOF Indikator */
        CacheEntryPtr           my_entry;       /* verweis auf cacheeintrag */
        struct handlebuffer *   prev;           /* Vorgaenger */
        struct handlebuffer *   next;           /* Nachfolger */
} HandleBuffer, *HandleBufferPtr;
