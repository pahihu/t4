#ifndef _NETCFG_H
#define _NETCFG_H

int readNetConfig (FILE *fin);
int connectedNetLink (int node, int link, int *othernode, int *otherlink);
char *netLinkURL (int node, int link);

#endif
