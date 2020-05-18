# ESSCompress

Encode to spectrum-preserving string set (SPSS) representation of a set of k-mers.

## Requirements

GCC >= 4.8 or a C++11 capable compiler


## Quick start

To compile from source:

    git clone https://github.com/medvedevgroup/ESSCompress
    cd ESSCompress
    make


After compiling, use

    ./ess -i [file path] -k [kmer_size] -t [1 or 0]

The input file should be a compacted de Bruijn graph from bcalm2.

e.g.

    ./ess -i file.unitigs.fa -k 11 -t 0
