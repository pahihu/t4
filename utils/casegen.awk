BEGIN \
{
  seqout = "fpseq.txt";
  fpout  = "fp.txt";
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
  }
}
