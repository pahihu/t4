#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage ()
{
        fprintf (stderr, "usage: myxxd [-c cols] [-s skip] [infile] [outfile]\n");
        exit (1);
}

int main(int argc, char*argv[])
{
        FILE *fin, *fout;
        int  cols, skip;
        int  i, done;
        unsigned int offs;
        char InFileName[256], OutFileName[256];

        InFileName[0]  = '\0';
        OutFileName[0] = '\0';
        cols = 16;
        skip = 0;

        for (i = 1; i < argc; i++)
        {
                if (*argv[i] == '-')
                switch (argv[i][1])
                {
                case 'c':
                        i++;
                        if (i == argc) usage ();
                        cols = atol (argv[i]);
                        break;
                case 'h':
                        usage ();
                        break;
                case 's':
                        i++;
                        if (i == argc) usage ();
                        skip = atol (argv[i]);
                        break;
                default:
                        usage ();
                }
                else
                {
                        if (InFileName[0] == '\0')
                                strcpy (InFileName, argv[i]);
                        else
                                strcpy (OutFileName, argv[i]);
                }
        }

        if (abs (skip) >= cols)
        {
                fprintf (stderr, "skip should be less than cols!\n");
                exit (1);
        }
        cols = cols - abs (skip);

        fin  = stdin;
        fout = stdout;
        if (InFileName[0])
        {
                fin = fopen (InFileName, "rb");
                if (fin == NULL)
                {
                        fprintf (stderr, "Cannot open %s for input!\n", InFileName);
                        exit (1);
                }
        }

        if (OutFileName[0])
        {
                fout = fopen (OutFileName, "wt");
                if (fout == NULL)
                {
                        fprintf (stderr, "Cannot open %s for output!\n", OutFileName);
                        exit (1);
                }
        }

        offs = 0;
        done = 0;
        while (!done)
        {
                fprintf (fout, "%08x:", offs);
                if (skip > 0)
                        for (i = 0; i < skip; i++)
                                getc (fin);
                for (i = 0; i < cols; i++)
                {
                        int ch = 0;

                        if ((i & 1) == 0)
                                fprintf (fout, " ");
                        if (feof (fin))
                                done = 1;
                        else
                                ch = getc (fin);
                        fprintf (fout, "%02x", 0xff & ch);
                }
                fprintf (fout, "\n");
                offs += cols;
                if (skip < 0)
                        for (i = 0; i < abs (skip); i++)
                                getc (fin);
        }
        fclose (fin);
        fclose (fout);

        return 0;
}
