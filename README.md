# ESSCompress v2.0

A tool to compress a set of k-mers represented in a file with FASTA/FASTQ format.


## Requirements

- Linux/unix operating system

- Git  

- GCC >= 4.8 or a C++11 capable compiler   

- CMake 3.1+   



## Installation from source

1) Download source and install:

       git clone https://github.com/medvedevgroup/ESSCompress
       sh INSTALL

Upon successful execution of this script, you will see binaries for `essCompress`, `essDecompress`, `dskESS`, `bcalmESS` and `mfcESS` `mfcdESS` in the `bin` folder.

You can directly download the `bin` folder from the release to skip this step.


2) Add the `bin` folder to your PATH environment. To temporarily do so, 
run `export PATH="$PWD/bin:$PATH"`.

To make the PATH change permanent edit ~/.bashrc and append the line `export PATH="$PWD/bin:$PATH"`. Then run      
       
       source ~/.bashrc
	


## Input    

File input format can be fasta, fastq, either gzipped or not.
To pass several files as input, input can be a list of files (one file per line):

`ls -1 *.fastq > list_reads`   
`essCompress -i list_reads ...`

## Usage


Use the script `essCompress` to compress and `essDecompress` to decompress.

Compression:
	
       Syntax: essCompress [parameters]   
       
       mandatory parameters:   
           -i [filepath]     Full path for input file.        
	   -k [INT]          k-mer size.
	   -a [INT]          DEFAULT=1. Sets a threshold X below which k-mers that are seen (strictly) less than X times in the dataset are filtered out.
	   
	   optional parameters:   
	   -f		     Fast compression mode (with smaller compression ratio).
	   -h                Print this Help.
	   -v                Validate the output, check that all the distinct k-mers in the input file appears exactly once in compressed file.
	   -V                Print software version and exit.

      
Decompression:
  
        Syntax: essDecompress [file_to_decompress]


## Citation

If using ESS-Compresss in your research, please cite
* Amatur Rahman, Rayan Chikhi and Paul Medvedev, Disk compression of k-mer sets, WABI 2020.
