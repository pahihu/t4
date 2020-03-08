BEGIN \
{
  seqout = "fpseq.txt";
  fpout  = "fp.txt";
  iuout  = "iuout.txt";
}

{
  if ($2 == "s") {
    printf "\t\t\t   case 0x%s: /* %s    */\n",$1,$3 >>seqout;
    printf "\t\t\t              IPtr++;\n" >>seqout;
    printf "\t\t\t              printf (\"-W-EMU414: FPU instruction.\\n\");\n" >>seqout;
    printf "\t\t\t              break;\n" >>seqout;
  } else if (substr($2, 1, 2) == "fp") {
    printf "\t\tcase 0x%s: /* %s    */\n",$1,$2 >>fpout;
    printf "\t\t           if (IsT414)\n" >>fpout;
    printf "\t\t               goto BadCode;\n" >>fpout;
    printf "\t\t           IPtr++;\n" >>fpout;
    printf "\t\t           printf (\"-W-EMU414: FPU instruction.\\n\");\n" >>fpout;
    printf "\t\t           break;\n" >>fpout;
  } else if (index($0, "T800") != 0) {
    printf "\t\tcase 0x%s: /* %s    */\n",$1,$2 >>iuout;
    printf "\t\t           if (IsT414)\n" >>iuout;
    printf "\t\t               goto BadCode;\n" >>iuout;
    printf "\t\t           IPtr++;\n" >>iuout;
    printf "\t\t           printf (\"-W-EMU414: T800 instruction.\\n\");\n" >>iuout;
    printf "\t\t           break;\n" >>iuout;
  }
}
