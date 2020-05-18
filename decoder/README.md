# Decoder for ESS-Compress and ESS-Tip-Compress

Decode to spectrum-preserving string set (SPSS) representation of a set of k-mers.

## Requirements

GCC >= 4.8 or a C++11 capable compiler


## Quick start

To compile from source:

    git clone https://github.com/medvedevgroup/ESSCompress
    cd decoder
    make


After compiling, use

    ./decoder -i [compressed.ess] -k [kmer_size] -t [1 or 0]
	
e.g.

    ./decoder -i ust_ess_abs.txt -k 11 -t 0

For quick test: run
	`make test 1` or  
	`make test 2` 
	

The important parameters are:

*  `k [int]` : The k-mer size that was used to generate the input, i.e. the length of the nodes of the node-centric de Bruijn graph.
*  `i [input-file]` : compressed file by ESS-Compress or ESS-Tip-Compress.
*  `t [0 or 1]` : Default is 0 (for decompression of ESS-Compress). Use a value of 1 to decompress file compressed using ESS-Tip-Compress.

The output is a FASTA file with extenstion ".fa" in the working directory (i.e. ust\_ess\_abs.txt.fa). This is an SPSS representation. 

