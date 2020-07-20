# ESSCompress v2.0

A tool to compress a set of k-mers represented in a file with FASTA/FASTQ format.


## Install

### Requirements

- Linux operating system (64 bit)

### Steps

1. Download the latest Linux 64-bit [binaries](https://github.com/medvedevgroup/ESSCompress/releases/download/v2.0/essCompress-v2.0-linux-64.zip) (zipped). `wget https://github.com/medvedevgroup/ESSCompress/releases/download/v2.0/essCompress-v2.0-linux-64.zip`

2. Unzip.   
`unzip essCompress-v2.0-linux-64.zip`

3. Add the `bin` folder to your PATH environment variable. There are two ways to set the path: i) Temporary and ii) Permanent. If you use the temporary method, you need to set the PATH each time shell is restarted.

	- To temporarily modify PATH:   
		* Locate the path of bin directory. Assuming you did not change any directory after step 1 and 2, you can find the path to bin directory using `$PWD/bin` command.
		* To add it to your PATH, run:  
		`export PATH="$PWD/bin:$PATH"`
	- 	To make the PATH change permanent:    
		* Open the file `~/.bashrc` in edit mode (i.e. using nano or vim).
		* Append the line `export PATH="$PWD/bin:$PATH"`.
		* Save and exit the file.
		* Run `source ~/.bashrc`   


## Usage

Read the instructions to install, then use the command `essCompress` to compress and `essDecompress` to decompress.


### essCompress: compression of a k-mer set

       Syntax: essCompress [parameters]   

       mandatory parameters:  
       -i [filepath]     Full path for input file.        
	   -k [INT]          k-mer size.

	   optional parameters:  
	   -a [INT]          DEFAULT=1. Sets a threshold X below which k-mers that are seen (strictly) less than X times in the dataset are filtered out.
	   -f				 Fast compression mode (with smaller compression ratio).
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

To pass a single file as input and compress:
`essCompress -i examples/11mers.fa -k 11`

To pass several files as input, generate the list of files (one file per line) as follows:

`ls -1 *.fastq > list_reads`   
`essCompress -i list_reads [..]`

ESS-Compress uses BCALM 2 under the hood, which does not care about paired-end information, all given reads contribute to k-mers in the graph (as long as such k-mers pass the abundance threshold).

#### Output for essCompress
The compressed output is in a file with `.essc` extension.



### essDecompress: decompression of .essc file

        Syntax: essDecompress [file_to_decompress]
i.e. 	`essDecompress 11mers.essc`   
Output is in a file with `.essd` extension. This represents a [spectrum-preserving string set](https://www.biorxiv.org/content/10.1101/2020.01.07.896928v2).

## Installation from source

### Pre-requisites
- Git  

- GCC >= 4.8 or a C++11 capable compiler   

- CMake 3.1+   

- Linux operating system

### Steps

1) Download source and install:

       git clone https://github.com/medvedevgroup/ESSCompress
       sh INSTALL

Upon successful execution of this script, you will see linux binaries for [BCALM](https://github.com/GATB/bcalm) (`bcalmESS`), [DSK](https://github.com/GATB/dsk) (`dskESS` and `dsk2asciiESS`) and [MFCompress](http://bioinformatics.ua.pt/software/mfcompress/) (`mfcESS` and `mfcdESS`) in the `bin` folder, along with `essCompress` and `essDecompress`.


2) Add the `bin` folder to your PATH environment. To temporarily do so,
run `export PATH="$PWD/bin:$PATH"`.

To make the PATH change permanent edit ~/.bashrc and append the line `export PATH="$PWD/bin:$PATH"`. Then run      

       source ~/.bashrc


## Citation

If using ESS-Compresss in your research, please cite
* Amatur Rahman, Rayan Chikhi and Paul Medvedev, Disk compression of k-mer sets, WABI 2020.
