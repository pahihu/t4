#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern int verbose;

#define NODE_HOST          -1
#define NODE_NOTCONNECTED  -2
#define NODE_INVALID       -3

typedef struct _NETURL {
        int  node, link;
        char *url;
} NETURL;

#define MAX_NETURL 1024
NETURL NetURLs[MAX_NETURL];
int nNetURL;

typedef struct _NETLINK {
        int node, link;
        int othernode, otherlink;
} NETLINK;

#define MAX_NETLINK  (MAX_NETURL * 4)
NETLINK NetLinks[MAX_NETLINK];
int nNetLink;


static char* skip (char *ptr)
{
        while (*ptr && isspace (*ptr))
                ptr++;
        return ptr;
}

static char* read_number (char *ptr, int *node)
{
        int n;

        *node = NODE_INVALID;
        ptr = skip (ptr);
        if (0 == *ptr)
                return ptr;

        /* fprintf (stderr, "read_number = [%s]\n", ptr); */
        n = 0;
        while (isdigit(*ptr))
        {
                n = 10*n + (*ptr) - '0';
                ptr++;
        }
        *node = n;
        return ptr;
}

static char* read_string (char *ptr, char *buf)
{
        buf[0] = '\0';
        ptr = skip (ptr);
        if (0 == *ptr)
                return ptr;
        while (!isspace (*ptr))
                *buf++ = *ptr++;
        *buf++ = '\0';
        return ptr;
}

static char* read_link (char *ptr, int *node, int *link)
{
        ptr = skip (ptr);
        /* fprintf (stderr, "read_link = [%s]\n", ptr); */
        if (0 == *ptr)
        {
                *node = NODE_NOTCONNECTED;
                return ptr;
        }
        else if (0 == strncmp (ptr, "HOST ", 5))
        {
                *node = NODE_HOST;
                return ptr + 5;
        }
        else if (0 == strncmp (ptr, "processor ", 10))
        {
                /* "processor" <number> "link" <number> */
                ptr = read_number (ptr + 10, node);
                if (NODE_INVALID == *node)
                        return ptr;
                ptr = skip (ptr);
                if (0 == *ptr)
                {
                        *node = NODE_INVALID;
                        return ptr;
                }
                if (strncmp (ptr, "link ", 5))
                {
                        *node = NODE_INVALID;
                        return ptr;
                }
                ptr = read_number (ptr + 5, link);
                if (NODE_INVALID == *link)
                        *node = NODE_INVALID;
                return ptr;
        }
        else if (0 == strncmp (ptr, "host", 4))
        {
                *node = NODE_HOST;
                return ptr + 4;
        }
        else if (0 == strncmp (ptr, "-", 1))
        {
                *node = NODE_NOTCONNECTED;
                return ptr + 1;
        }
        ptr = read_number (ptr, node);
        if (NODE_INVALID == *node)
                return ptr;
        if (strncmp (ptr, "-", 1))
                return ptr + 1;
        ptr = read_number (ptr + 1, link);
        if (NODE_INVALID == *link)
                *node = NODE_INVALID;
        return ptr;
}

int readNetConfig (FILE *fin)
{
        char buf[128];
        int i, nline;
        char *ptr;
        int node, link, thenode, thelink;
        char url[128];

        nline = 1;
        ptr = fgets (buf, sizeof(buf), fin);
        while (ptr && !feof (fin))
        {
                /* fprintf (stderr, "line = [%s]\n", buf); */
                ptr = skip (ptr);
                if (0 == *ptr)
                        goto NextLine;

                if (0 == strncmp (ptr, "--", 2))
                        goto NextLine;
                else if (0 == strncmp (ptr, "Connect ", 8))
                {
                        /* Connect HOST to processor <number> link <number> */
                        /* Connect processor <number> link <number>  to processor <number> link <number> */
                        ptr = read_link (ptr + 8, &thenode, &thelink);
                        if (NODE_INVALID == thenode)
                        {
                                printf ("-E-EMU414: Cannot read link at line %d.\n", nline);
                                return (-1);
                        }
                        if (thenode >= 0)
                        {
                                ptr = skip (ptr);
                                if (strncmp (ptr, "to", 2))
                                {
                                        printf ("-E-EMU414: Missing 'to' at line %d.\n", nline);
                                        return (-1);
                                }
                                ptr = read_link (ptr + 2, &node, &link);
                                if (NODE_INVALID == thenode)
                                {
                                        printf ("-E-EMU414: Cannot read 'to' link at line %d.\n", nline);
                                        return (-1);
                                }
                                if (verbose)
                                        printf ("-I-EMU414: Connect %d-%d => %d-%d\n", thenode, thelink, node, link);
                                NetLinks[nNetLink].node = thenode;
                                NetLinks[nNetLink].link = thelink;
                                NetLinks[nNetLink].othernode = node;
                                NetLinks[nNetLink++].otherlink = link;
                                if (verbose)
                                        printf ("-I-EMU414: Connect %d-%d => %d-%d\n", node, link, thenode, thelink);
                                NetLinks[nNetLink].node = node;
                                NetLinks[nNetLink].link = link;
                                NetLinks[nNetLink].othernode = thenode;
                                NetLinks[nNetLink++].otherlink = thelink;
                        }
                }
                else if (0 == strncmp (ptr, "link", 4))
                {
                        /* "link" <link> string */
                        ptr = read_link (ptr + 4, &node, &link);
                        if (node < 0)
                        {
                                printf ("-E-EMU414: Invalid link at line %d.\n", nline);
                                return (-1);
                        }
                        ptr = read_string (ptr, url);
                        if (0 == url[0])
                        {
                                printf ("-E-EMU414: Missing URL at line %d.\n", nline);
                                return (-1);
                        }
                        if (verbose)
                                printf ("-I-EMU414: Node %d Link%dIn at %s\n", node, link, url);
                        NetURLs[nNetURL].node = node;
                        NetURLs[nNetURL].link = link;
                        NetURLs[nNetURL++].url = strdup (url);
                }
                else if (isdigit(*ptr))
                {
                        /* number <link> <link> <link> <link>
                         * link := "host" | "-" | number "-" number
                         */
                        ptr = read_number (ptr, &thenode);
                        if (-1 == node)
                        {
                                printf ("-E-EMU414: Cannot read node id at line %d.\n", nline);
                                return (-1);
                        }
                        for (i = 0; i < 4; i++)
                        {
                                ptr = skip (ptr);
                                if (0 == *ptr)
                                        break;
                                if (0 == strncmp (ptr, "--", 2))
                                        break;
                                ptr = read_link (ptr, &node, &link);
                                if (NODE_INVALID == node)
                                {
                                        printf ("-E-EMU414: Cannot read link at line %d.\n", nline);
                                        return (-1);
                                }
                                if (node >= 0) /* skip HOST and NOT CONNECTED links */
                                {
                                        if (verbose)
                                                printf ("-I-EMU414: Connect %d-%d => %d-%d\n", thenode, i, node, link);
                                        NetLinks[nNetLink].node = thenode;
                                        NetLinks[nNetLink].link = i;
                                        NetLinks[nNetLink].othernode = node;
                                        NetLinks[nNetLink++].otherlink = link;
                                }
                        }
                }
NextLine:
                ptr = fgets (buf, sizeof(buf), fin);
                nline++;
        }
        return 0;
}

int connectedNetLink(int node, int link, int *othernode, int *otherlink)
{
        int i;

        for (i = 0; i < nNetLink; i++)
        {
                if ((node == NetLinks[i].node) && (link == NetLinks[i].link))
                {
                        *othernode = NetLinks[i].othernode;
                        *otherlink = NetLinks[i].otherlink;
                        return 0;
                }
        }
        return -1;
}

char* netLinkURL (int node, int link)
{
        static char buf[128];
        int i;

        for (i = 0; i < nNetURL; i++)
        {
                if ((node == NetURLs[i].node) && (link == NetURLs[i].link))
                        return NetURLs[i].url;
        }
        sprintf (buf, "ipc:///tmp/node%d-%d.lnk", node, link);
        return buf;
}

#if defined(NETCFG_TEST)
int verbose = 1;
int main(int argc, char*argv[])
{
        FILE *fin;

        if (2 != argc)
        {
                fprintf (stderr, "usage: netcfg <file>\n");
                exit (1);
        }
        if ((fin = fopen (argv[1], "r")) == NULL)
        {
                fprintf (stderr, "cannot open %s\n", argv[1]);
                exit (1);
        }
        readNetConfig (fin);
        fclose (fin);

        return 0;
}
#endif
