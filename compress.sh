#!/bin/bash
validateset=0
################################################################################
# Help                                                                         #
################################################################################
Help()
{
   # Display Help
   echo "Description of the script functions here."
   echo
   echo "Syntax: ess_run [options]"
   echo "options:"
   echo "-i [filepath]     Input file (include full path)"
   echo "-k [INT]          k-mer size"
   echo "-a [INT]          min-abundance (default 2)"
   echo "-m [0,1,2]        compression mode (default:0); 0=ess(high compression), 1=ess-tip(low memory compression), 2=no compression (plain spss)"
   echo "-t [1 or 0]       Specify input type (default 1): 0=fasta/fastq file, 1=cdbG by bcalm"
   echo "-h                Print this Help."
   echo "-v                Validate the output"
   echo "-V                Print software version and exit."
   echo
}

   # echo "-g                Print the GPL license notification."

Version()
{
   echo "ESS-Compress v2.0"
}




validate(){
   K=$1
   UNITIG_FILE=$2
   DECOMPRESSED_FILE=$3

   DSK_PATH=dskESS
   DSK2ASCII_PATH=dsk2asciiESS

   echo "Validating decoded ESS...";
   $DSK_PATH -file $UNITIG_FILE -kmer-size $K -abundance-min 1  -verbose 0 -out unitigs.h5
   $DSK_PATH -file $DECOMPRESSED_FILE -kmer-size $K -abundance-min 1  -verbose 0 -out spss.h5
   $DSK2ASCII_PATH -file unitigs.h5 -out unitigs.txt  -verbose 0
   $DSK2ASCII_PATH -file spss.h5 -out spss.txt  -verbose 0
   echo "doing highly  accurate validation................"
   sort -k 1 -n unitigs.txt -o sorted_unitigs.txt; sort -k 1 -n spss.txt -o sorted_spss.txt
   cmp sorted_unitigs.txt sorted_spss.txt && echo '### SUCCESS: Files Are Identical! ###' || echo '### WARNING: Files Are Different! ###'
   rm -rf sorted_spss.txt spss.txt unitigs.txt unitigs.h5 spss.h5  
   rm -rf sorted_unitigs.txt
}

validate_again(){
   K=$1
   UNITIG_FILE=$2
   DECOMPRESSED_FILE=$3

   DSK_PATH=dskESS
   DSK2ASCII_PATH=dsk2asciiESS

   echo "Validating decoded ESS...";
   $DSK_PATH -file $DECOMPRESSED_FILE -kmer-size $K -abundance-min 1  -verbose 0 -out spss.h5
   $DSK2ASCII_PATH -file spss.h5 -out spss.txt  -verbose 0
   echo "doing highly  accurate validation................"
   sort -k 1 -n spss.txt -o sorted_spss.txt
   cmp sorted_unitigs.txt sorted_spss.txt && echo '### SUCCESS: Files Are Identical! ###' || echo '### WARNING: Files Are Different! ###'
   rm -rf sorted_spss.txt spss.txt unitigs.txt unitigs.h5 spss.h5  
   #rm -rf sorted_unitigs.txt
}



# validate(){
#    K=$1
#    SORTED_UNITIG_KMERS=$2
#    DECOMPRESSED_FILE=$3

#    DSK_PATH=dskESS
#    DSK2ASCII_PATH=dsk2asciiESS

#    echo "Validating decoded ESS...";
#    $DSK_PATH -file $DECOMPRESSED_FILE -kmer-size $K -abundance-min 1  -verbose 0 -out spss_kmers.h5
#    $DSK2ASCII_PATH -fasta -file spss_kmers.h5 -out spss_kmers.fa  -verbose 0
#    echo "doing highly  accurate validation................"
#    cmp $SORTED_UNITIG_KMERS spss_kmers.fa && echo '### SUCCESS: Files Are Identical! ###' || echo '### WARNING: Files Are Different! ###'
#    #rm -rf spss_kmers.fa spss_kmers.h5
# }



essCompress_prior(){
   L=$1
   K=$2
   A=$3
   TYPE=$4
   O=$5
   KMERS_FA=kmers.fa

   cd $O
   mkdir -p ess$K.$A
   cd ess$K.$A

   ## get one k-mer per line fasta file
   echo "Running DSK to get k-mers..."
   dskESS -file $L -kmer-size $K -abundance-min $A -verbose 0 -out kmers.h5
   dsk2asciiESS -fasta -file kmers.h5 -out $KMERS_FA -verbose 0
   rm -rf kmers.h5
   echo "Done."
   echo "-------------"

   ## get unitigs
   echo "Running BCALM2 to get compacted de Bruijn graph..."
   /usr/bin/time  -f "%M\t%e" --output-file=mem_bcalm bcalmESS -in $KMERS_FA -kmer-size $K  -abundance-min 1 -verbose 0
   rm -rf *.glue* *.h5 trashme*
   echo "Done."
   echo "-------------"
}

#step 1
essCompress_main(){
   L=$1
   K=$2
   A=$3
   TYPE=$4
   O=$5
   #KMERS_FA=kmers.fa
   KMERS_FA=$L

   cd $O
   mkdir -p ess$K.$A
   cd ess$K.$A

   #get ess compression
   #L=kmers.unitigs.fa
   /usr/bin/time  -f "%M\t%e" --output-file=mem_ess_$TYPE essCompress -i $L -k $K -t $TYPE
   
   if [[ "$TYPE" -eq "0" ]]; then
      cat kmers.ess | tr "[" "g" | tr "]" "t" | tr "+" "a" | tr "-" "c" | awk -F=' ' '{print ""$1}' > kmers2.ess
      var=$(echo ">2.0\_$K\_0") ;  sed -i "1s/.*/$var/" kmers2.ess
      /usr/bin/time  -f "%M\t%e" --output-file=mem_mfc_$TYPE mfcESS kmers2.ess
      mv kmers2.ess.mfc $O/kmers.ess.mfc
   elif [[ "$TYPE" -eq  "1" ]]; then
      cat kmers.esstip | tr "{" "a" | tr "}" "c" | tr "(" "g" | tr ")" "t" | awk -F=' ' '{print ""$1}' > kmers2.esstip
      var=$(echo ">2.0\_$K\_1") ;  sed -i "1s/.*/$var/" kmers2.esstip
      /usr/bin/time  -f "%M\t%e" --output-file=mem_mfc_$TYPE mfcESS kmers2.esstip
      mv kmers2.esstip.mfc $O/kmers.esstip.mfc
   else
      /usr/bin/time  -f "%M\t%e" --output-file=mem_mfc_$TYPE mfcESS kmers.ust.spss
      mv kmers.ust.spss.mfc $O
   fi

   #validate
   if [[ $validateset -eq 1 ]]; then

      if [[ "$TYPE" -eq "0" ]]; then
         #cat kmers.ess | tr "[" "g" | tr "]" "t" | tr "+" "a" | tr "-" "c" | awk -F=' ' '{print ""$1}' > kmers2.ess
         #/usr/bin/time  -f "%M\t%e" --output-file=mem_mfc_$TYPE mfcESS kmers2.ess

         var=$(echo ">2.0\_$K\_0") ; sed -i "1s/.*/$var/" kmers.ess
         /usr/bin/time  -f "%M\t%e" --output-file=mem_dec_$TYPE essDecompress kmers.ess

         validate $K $KMERS_FA kmers.ess.spss

      elif [[ "$TYPE" -eq  "1" ]]; then
         #cat kmers.esstip | tr "{" "a" | tr "}" "c" | tr "(" "g" | tr ")" "t" | awk -F=' ' '{print ""$1}' > kmers2.esstip
         #/usr/bin/time  -f "%M\t%e" --output-file=mem_mfc_$TYPE mfcESS kmers2.esstip

         var=$(echo ">2.0\_$K\_1") ;  sed -i "1s/.*/$var/" kmers.esstip
         /usr/bin/time  -f "%M\t%e" --output-file=mem_dec_$TYPE essDecompress kmers.esstip

         validate $K $KMERS_FA kmers.esstip.spss
      else
         /usr/bin/time  -f "%M\t%e" --output-file=mem_mfc_$TYPE mfcESS kmers.ust.spss
         validate $K $KMERS_FA kmers.ust.spss
      fi

   fi
   
            
}



# essCompress_prior ~/s/rhodo/list_reads 31 2 0 ~/s/rhodo/
# essCompress_main ~/s/rhodo/list_reads 31 2 0 ~/s/rhodo/
# essCompress_main ~/s/rhodo/list_reads 31 2 1 ~/s/rhodo/
# essCompress_main ~/s/rhodo/list_reads 31 2 2 ~/s/rhodo/


countNonDNA(){
   cat $1 |  grep -v '>' | grep -o "[acgt]" | wc | awk '{print $3-$1}'
}

countDNA(){
   cat $1 |  grep -v '>' | grep -o "[ACGT]" | wc | awk '{print $3-$1}'
}






################################################################################
################################################################################
# Main program                                                                 #
################################################################################
################################################################################
################################################################################
# Process the input options. Add options as needed.                            #
################################################################################
# Get the options



colorset=0
modeset=0
outdirset=0


K=0
A=2
MODE=0
while getopts "vhVk:t:m:i:a:o:" option; do
   case $option in
      h) # display Help
         Help
         exit;;
      V) # display version
         Version
         exit;;
      v) validateset=1
         ;;
      k) K=$OPTARG
         echo $K
         ;;
         #exit;;
      a) A=$OPTARG
         ;;
         #exit;;
      m) MODE=$OPTARG
        [[ ! $MODE =~ 0|1|2 ]] && {
            echo "Incorrect options provided, use either 0 (ess), or 1 (ess-tip) or 2 (spss)"
            exit 1
        }
        modeset=1
        ;;
      t) COLOR=$OPTARG
      echo $COLOR
        [[ ! $COLOR =~ 0|1 ]] && {
            echo "Incorrect options provided, use either 0 (fasta/q), or 1 (cdbg)"
            exit 1
        }
        colorset=1
        ;;
      i) INPUTFILE=${OPTARG}
         ;;
         #exit;;
      o) OUTDIR=${OPTARG}
         outdirset=1
         ;;
         #exit;;
     \?) # incorrect option
         echo "Error: Invalid option,  Use -h for help."
         exit;;
   esac
done

if [[ K -eq 0 ]]; then
    echo "You must specify a value for k-mer size [-k]. Use -h for help."
    exit 2
fi

# if [[ colorset -eq 0 ]]; then
#     echo "You must specify input file type [-t]. Use -h for help."
#     exit 2
# fi

if [[ outdirset -eq 0 ]]; then
    echo "You must specify output directory. Use -h for help."
    exit 2
fi


# if [[ modeset -eq 0 ]]; then
#     echo "You must specify compression mode [-m]. Use -h for help."
#     exit 2
# fi

#export PATH="/home/aur1111/s/absorb/v2.0/bin:$PATH"; source ~/.bashrc


if [[ COLOR -eq 0 ]]; then
    essCompress_prior $INPUTFILE $K $A $MODE $OUTDIR
    essCompress_main kmers.unitigs.fa $K $A $MODE $OUTDIR 
fi

if [[ COLOR -eq 1 ]]; then
    essCompress_main $INPUTFILE $K $A $MODE $OUTDIR 
fi
