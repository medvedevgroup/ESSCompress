# ESSCompress

A tool to compress a spectrum-preserving string set (SPSS) representation of a set of k-mers using compacted de Bruijn graph.

## Requirements

- GCC >= 4.8 or a C++11 capable compiler

- Supports only linux/unix 


## Quick start

To compile from source:

    git clone https://github.com/medvedevgroup/ESSCompress
    cd ESSCompress
    make


After compiling, use

    ./essCompress -i [bcalm2 cdbg file] -k [kmer_size] -t [1 or 0]
	
e.g.

    ./essCompress -i chol.unitigs.fa -k 11 -t 0


The important parameters are:

*  `k [int]` : The k-mer size that was used to generate the input, i.e. the length of the nodes of the node-centric de Bruijn graph.
*  `i [input-file]` : Unitigs file produced by [BCALM2 in FASTA format](https://github.com/GATB/bcalm#output).
*  `t [0 or 1]` : Default is 1. 0 ==> tip mode off, 1 ==> tip mode on. 

The output is a compressed ESS file with extenstion ".ess" (non-tip mode) or ".esstip (tip mode) in the working folder. 



#### For quick test

    make test1
    make test2


    