ESSCompress
===============

A tool to compress a set of k-mers represented in FASTA/FASTQ file(s).


# Installation

The easist way to install ESS is to download and extract the latest Linux 64-bit [binaries](https://github.com/medvedevgroup/ESSCompress/releases/download/v2.0/essCompress-v2.0-linux-64.tar.gz).

```
wget https://github.com/medvedevgroup/ESSCompress/releases/download/v2.0/essCompress-v2.0-linux-64.tar.gz
tar xvzf essCompress-v2.0-linux-64.tar.gz
```

The executable files are now located in the bin `essCompress-v2.0/bin`, relative to the working directory. Optionally, you can add this directory to your PATH variable or move all the files from this directory into another directory in your PATH.

If you prefer to install from source, see [below](#Installation-from-source).

# Quick start 

We illustrate the usage of ESS with an example. Please change into the base directory of the ESS installation. If you have just finished downloading and extracting ESS, you can do this by `cd essCompress-v2.0`. The distribution contains a small example fasta file:
```
$cat examples/smallExample.fa
>
AAAAAAACCCCCCCCCC
>
CCCCCCCCCCA
```
To compress this file using a k-mer size 11, run
```
$ bin/essCompress -k 11 -i examples/smallExample.fa
```
The output file is `examples/smallExample.fa.essc`. It is a  binary file, which stores the MFC compressed ESS representation of the k-mers in `examples/smallExample.fa`. To decompress it back into a fasta file, run
```
$ bin/essDecompress examples/smallExample.fa.essc   
```
The output file is `examples/smallExample.fa.essd`. You can check its contents
```
$ cat examples/smallExample.fa.essd 
>
AAAAAAACCCCCCCCCCA
```
Notice that the decompressed fasta file is not the same as the original fasta file, but 
it contains the same k-mers as smallExample.fa. You can double check this by running:

```
$ bin/essAuxValidate 11 examples/smallExample.fa examples/smallExample.fa.essd
Checking whether file 'examples/smallExample.fa' and file 'examples/smallExample.fa.essd' contain same 11-mers...
### SUCCESS: The two files contain same k-mers! ###
```


# Usage details

The options for essCompress are

       Syntax: ./essCompress [parameters]   

	   -k [int]          k-mer size (must be >= 4)
	   -i [input-file]   Path to an input fasta or fastq file which can optionally be gzipped.  Alternatively, it can be a path to a text file containing the list of multiple input files, with one file per line.

	   optional arguments:
	   -a [int]          Sets a threshold X, such that k-mers that appear less than X times in the input dataset are filtered out (default = 1).
	   -f                Fast mode. It can be twice as fast and use several times less RAM, at the cost of a smaller compression ratio (usually 10% more space).
	   -v                Enable verbose mode: print more useful information.
	   -c                Verify correctness after compression. This checks that all the distinct k-mers in the input file appears exactly once in compressed file. K-mers and their reverse complements are treated as equal by ESS.
	   -h                Print this help.

The output of essCompress is a [spectrum-preserving string set](http://doi.org/10.1007/978-3-030-45257-5_10) representation of the input file. In other words, the output file does not contain any duplicate k-mers and the k-mers it contains are exactly the distinct k-mers in the input. A k-mer and its reverse complement are treated as equal by ESS. To decompress, run `essDecompress [filename.essc]`, where *filename.essc* is the file output by essCompress. 

## Miscellenous information
In order to pass several files as input, you can generate the list of files (one file per line) following this example. If you are in the base directory of essCompress, run
```
$ ls -1 examples/*.fa > list_reads   
$ bin/essCompress -i list_reads -k 5
```

ESS ignores any paired-end information present in the input. 

# Running in UST mode
In our [RECOMB paper](http://doi.org/10.1007/978-3-030-45257-5_10) we described a program called UST to generate a spectrum-preserving string set (SPSS) from a set of sequences. The original UST software is now redundant because ESS can be used to generate the UST SPSS. In particular, running *essCompress* followed by *essDecompress* generates the UST SPSS. For example, if you are in the base directory *essCompress-v2.0* and want to run UST on *examples/smallExample.fa*, do:
```
$ bin/essCompress -k 11 -i examples/smallExample.fa
$ bin/essDecompress examples/smallExample.fa.essc   
```
The output file `examples/smallExample.fa.essd` contains the SPPS output by UST.


# Installation from source

The following are pre-requisites
- Linux operating system (64 bit)
- Git
- GCC >= 4.8 or a C++11 capable compiler   
- CMake 3.1+   

Download source and install:

```
git clone https://github.com/medvedevgroup/ESSCompress
cd ESSCompress
./INSTALL
```

Upon successful execution of this script, you will see linux binaries for [BCALM](https://github.com/GATB/bcalm) (`essAuxBcalm`), [DSK](https://github.com/GATB/dsk) (`essAuxDsk` and `essAuxDsk2ascii`) and [MFCompress](http://bioinformatics.ua.pt/software/mfcompress/) (`essAuxMFCompressC` and `essAuxMFCompressD`) in the `bin` folder, along with `essAuxValidate`, `essAuxCompress` and `essAuxDecompress`. All of these are auxiliary executables. The main two executables are `essCompress` and `essDecompress`.

To check that you have properly installed ESS, please try to compress a small file in the [Quick start](#Quick-start) section.


# Citation

If using ESS-Compresss in your research, please cite
* Amatur Rahman, Rayan Chikhi and Paul Medvedev, Disk compression of k-mer sets, WABI 2020.

If you are using it not for the purposes of compression but for the purposes of generating an SPSS or simulating UST, please also cite
* Amatur Rahman and Paul Medvedev, [Representation of k-mer Sets Using Spectrum-Preserving String Sets, RECOMB 2020](http://doi.org/10.1007/978-3-030-45257-5_10).
