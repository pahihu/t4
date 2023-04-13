/****************************************************************************
*                prioq.c
*
*  This module implements a priority queue using a heap.
*
*  from Persistence of Vision Raytracer 
*  Copyright 1992 Persistence of Vision Team
*---------------------------------------------------------------------------
*  Copying, distribution and legal info is in the file povlegal.doc which
*  should be distributed with this file. If povlegal.doc is not available
*  or for more info please contact:
*
*       Drew Wells [POV-Team Leader] 
*       CIS: 73767,1244  Internet: 73767.1244@compuserve.com
*       Phone: (213) 254-4041
* 
* This program is based on the popular DKB raytracer version 2.12.
* DKBTrace was originally written by David K. Buck.
* DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
*
*****************************************************************************/

#include "frame.h"
#include "povproto.h"

#define NUMBER_OF_PRIOQS 32
#define NUMBER_OF_INTERSECTIONS 128

PRIOQ *prioqs;

void pq_init ()
{
   register int i;
   PRIOQ *new_pq;

   prioqs = NULL;

   for (i = 0 ; i < NUMBER_OF_PRIOQS; i++)
   {
      if ((new_pq = (PRIOQ *) malloc (sizeof (PRIOQ))) == NULL) {
         fprintf (stderr, "\nOut of memory. Cannot allocate queues");
         close_all();
         exit(1);
      }

      new_pq -> next_pq = prioqs;
      prioqs = new_pq;

      if ((new_pq -> queue = (INTERSECTION *)
         malloc (NUMBER_OF_INTERSECTIONS * sizeof (INTERSECTION))) == NULL) {
         fprintf (stderr, "\nOut of memory. Cannot allocate queue entries");
         close_all();
         exit(1);
      }
   }
}

PRIOQ *pq_alloc()
{
   PRIOQ *allocated_queue;

   if (prioqs == NULL) {
      fprintf (stderr, "\nOut of prioqs");
      close_all();
      exit(1);
   }

   allocated_queue = prioqs;
   prioqs = allocated_queue -> next_pq;
   return (allocated_queue);
}

void pq_free (pq)
PRIOQ *pq;
{
   pq -> next_pq = prioqs;
   prioqs = pq;
}

PRIOQ *pq_new (index_size)
int index_size;
{
   PRIOQ *pq;

   if (index_size >= NUMBER_OF_INTERSECTIONS)	/* Charles Marslett */
      index_size = NUMBER_OF_INTERSECTIONS - 1;	/* Most real scenes overflow! */


   if ((pq = pq_alloc()) == NULL)
      return (NULL);

   pq -> queue_size = index_size;
   pq -> current_entry = 0;
   return (pq);
}

void pq_balance(q, entry_pos1)
PRIOQ *q;
unsigned int entry_pos1;
{
   register INTERSECTION *entry1, *entry2;
   INTERSECTION temp_entry;
   register unsigned int entry_pos2;

   entry1 = &q->queue[entry_pos1];

   if ((entry_pos1 * 2 < q->queue_size)
      && (entry_pos1 * 2 <= q -> current_entry))
   {
      if ((entry_pos1*2+1 > q->current_entry) ||
         (q->queue[entry_pos1*2].Depth < q->queue[entry_pos1*2+1].Depth))
         entry_pos2 = entry_pos1*2;
      else
         entry_pos2 = entry_pos1*2+1;

      entry2 = &q->queue[entry_pos2];

      if (entry1->Depth > entry2->Depth) {
         temp_entry = *entry1;
         *entry1 = *entry2;
         *entry2 = temp_entry;
         pq_balance (q, entry_pos2);
      }
   }

   if (entry_pos1 / 2 >= 1 )
   {
      entry_pos2 = entry_pos1 / 2;
      entry2 = &q->queue[entry_pos2];
      if (entry1->Depth < entry2->Depth) {
         temp_entry = *entry1;
         *entry1 = *entry2;
         *entry2 = temp_entry;
         pq_balance (q, entry_pos2);
      }
   }
}

void pq_add (q, queue_entry)
PRIOQ *q;
INTERSECTION *queue_entry;
{
   q -> current_entry++;
   if (q -> current_entry >= q -> queue_size)
      q -> current_entry--;

   q -> queue [q -> current_entry] = *queue_entry;
   pq_balance (q, q -> current_entry);
}

INTERSECTION *pq_get_highest(q)
PRIOQ *q;
{
   if (q -> current_entry >= 1)
      return (&(q -> queue[1]));
   else
      return (NULL);
}

int pq_is_empty(q)
PRIOQ *q;
{
   if (q -> current_entry < 1)
      return (TRUE);
   else
      return (FALSE);
}

void pq_delete_highest (q)
PRIOQ *q;
{
   q -> queue[1] = q -> queue[q -> current_entry--];
   pq_balance (q, 1);
}
