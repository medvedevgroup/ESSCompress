# ESSCompress v2.0

A tool to compress a set of k-mers represented in FASTA/FASTQ file(s).


## Installation

### Requirements

- Linux operating system (64 bit)

### Steps

1. Download the latest Linux 64-bit [binaries](https://github.com/medvedevgroup/ESSCompress/releases/download/v2.0/essCompress-v2.0-linux-64.zip) (zipped).   
`wget https://github.com/medvedevgroup/ESSCompress/releases/download/v2.0/essCompress-v2.0-linux-64.zip`

2. Extract the zip file.   
`unzip essCompress-v2.0-linux-64.zip`



## Quick start with a step-by-step example

Lets say you have a small fasta file of sequences named example.fa, and   
`cat example.fa` returns

```
>
AAAAAAACCCCCCCCCC
>
CCCCCCCCCCA
```
We can compress it using k=11 as follows
```
./essCompress -k 11 -i example.fa
```  
Now `ls` will show both original file and compressed file in the same directory:

```
example.fa.essc
example.fa
```
example.fa.essc is a compressed binary file generated by MFCompress, so it is not in a readable format.

To decompress into a readable format, you can run
```
./essDecompress example.fa.essc   
```

You'll now see the decompressed file example.fa.essd in the same directory.   
`cat example.fa.essd ` will return:    

```
>
AAAAAAACCCCCCCCCCA
```
Notice that the decompressed fasta file is not the same as the original file, but it contains the same k-mers as example.fa. You can double check this using
`./validateSameKmer 11 example.fa example.fa.essd`. If they contain the same k-mers, output will show:

```
### SUCCESS: The two files contain same k-mers! ###
```


## Usage

Read the instructions to install, then use `bin/essCompress` to compress and `bin/essDecompress` to decompress.


### essCompress: compression of a k-mer set
	
       Syntax: ./essCompress [parameters]   
       
       mandatory parameters:  
       -i [filepath]     Full path for input file.        
	   -k [INT]          k-mer size (must be >=11)
	   
	   optional parameters:  
	   -a [INT]          DEFAULT=1. Sets a threshold X below which k-mers that are seen (strictly) less than X times in the dataset are filtered out. 
	   -f		     Fast compression mode (with smaller compression ratio).
	   -h                Print this Help.
	   -v                Validate the output, check that all the distinct k-mers in the input file appears exactly once in compressed file.
	   -V                Print software version and exit.


#### Input for essCompress 

Two important input parameters are 
* input [-i]   
* k-mer size [-k]   

File input format can be   
	1. a single fasta or fastq file (either gzipped or not)   
	2. a text file containing the list of multiple fasta/fastq files (one file per line)	 

To pass a single file as input and compress: `essCompress -i examples/11mers.fa -k 11`

To pass several files as input, generate the list of files (one file per line) as follows:

```
ls -1 *.fastq > list_reads   
./essCompress -i list_reads [..]
```

ESS-Compress uses BCALM 2 under the hood, which does not care about paired-end information, all given reads contribute to k-mers in the graph (as long as such k-mers pass the abundance threshold).



#### Output for essCompress 
The compressed output is in a file with `.essc` extension.




     
### essDecompress: decompression of .essc file
  
        Syntax: ./essDecompress [file_to_decompress]

Input: a .essc file generated by essCompress   
Output: a fasta file containing spectrum-preserving string set represented by the .essc file   

## Installation from source

### Pre-requisites
- Linux operating system (64 bit) 

- Git 

- GCC >= 4.8 or a C++11 capable compiler   

- CMake 3.1+   

### Steps

Download source and install:

       git clone https://github.com/medvedevgroup/ESSCompress
       sh INSTALL

Upon successful execution of this script, you will see linux binaries for [BCALM](https://github.com/GATB/bcalm) (`bcalmESS`), [DSK](https://github.com/GATB/dsk) (`dskESS` and `dsk2asciiESS`) and [MFCompress](http://bioinformatics.ua.pt/software/mfcompress/) (`mfcESS` and `mfcdESS`) in the `bin` folder, along with `validateSameKmer`, `essCompress` and `essDecompress`.

	

## Citation

If using ESS-Compresss in your research, please cite
* Amatur Rahman, Rayan Chikhi and Paul Medvedev, Disk compression of k-mer sets, WABI 2020.
