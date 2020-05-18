# Experiments 

In this page, we describe all the necessary steps to reproduce the results in the paper.


## Commands and parameters for tools used

#### SRA fastq-dump
For human RNA-seq data used in paper:

`fastq-dump --outdir fastq --gzip --skip-technical  --readids --read-filter pass --dumpbase --split-3 --clip SRR957915 `

This will generate two "fastq.gz" files.


To get FASTA file directly: 
`fastq-dump --fasta 100  -A SRR957915`


#### BCALM 2


##### Version to download
- Release v2.2.1 or earlier releases.

##### Prepare Input 
Make a file (i.e. "list_reads") with the path of the two "fastq.gz" files. 

##### Run bcalm 2 (without counts)
`bcalm -in list_reads -kmer-size 31 -abundance-min 2 -max-memory 1000`

This creates a file name "list_reads.unitigs.fa" in the working directory. The output header is:

	><id> LN:i:<length> KC:i:<abundance> KM:f:<abundance> L:<+/->:<other id>:<+/-> [..]
	
	



#### MFCompress
- Download [MFCompress](http://bioinformatics.ua.pt/software/mfcompress/)
- To compress: `MFCompressC stitchedUnitigs.fa`
- To decompress: `MFCompressD stitchedUnitigs.fa.mfc`

#### KMC 3 (required to test VARI)
`kmc -k31 -ci2 -sm -m2 -v  @list_reads /home/user/kmc31.2_outdir /home/user/kmc31.2_outdir/tmp`

#### Cosmo/VARI
- Follow the instructions to install [VARI](https://github.com/cosmo-team/cosmo/tree/VARI). Last checked with commit d35bc3dd2d6ba7861232c49274dc6c63320cedc1.
- Run kmc. Make a file `list_kmc` with the path to the kmc outputs (i.e. `/home/user/kmc31.2_outdir/kmc31.2`) .
- Run `cosmo-build -d list_kmc`
- This outputs the binary succinct DBG file `list_kmc.dbg`.


## Command to measure time and memory usage
`/usr/bin/time`

## Versions used for analysis

Versions of the tools used in experiments of the RECOMB paper. 

| Tool | URL | Git commit hash / Version | 
| ---  | --- | ------------------------- |
| UST | https://github.com/medvedevgroup/UST | 37364eaefa676cd67916e484ea79074945d4e354 |
| Bcalm2 | https://github.com/gatb/bcalm | f4e0012e8056c56a04c7b00a927c260d5dbd2636 |
| Cosmo / VARI | https://github.com/cosmo-team/cosmo/tree/VARI | ef82d38af2c402beab9ef9f12a72e7dcaeff210c |
| KMC   | https://github.com/refresh-bio/KMC | 85ad76956d890aa24fc8525eee5653078ed86ace | 
| MFC | http://bioinformatics.ua.pt/software/mfcompress/ | Version 1.01 |




## Dataset Download


| Dataset | Source | URL / Accession number | 
| ---  | --- | ------------------------- |
| Rhodobacter sphaeroides | GAGE | [http://gage.cbcb.umd.edu/](http://gage.cbcb.umd.edu/) |
| Human RNA-seq | NCBI SRA | SRR957915 |
| Keratinized gingiva, metagenome	 |  NCBI SRA | SRS014473 |
| Soybean RNA-seq   |  NCBI SRA | SRR11458718 | 
| Tongue dorsum, metagenome |  NCBI SRA | SRS011086 |



Command to download and filter SRA datasets:

- To get fastq.gz  `fastq-dump --outdir fastq --gzip --skip-technical  --readids --read-filter pass --dumpbase --split-3 --clip [accession number]`  

- To get fasta file 
`fastq-dump --fasta 100  -A [accession number]`

So, to get fasta file of human RNA-seq dataset, run:
 `fastq-dump --fasta 100  -A SRR957915`



## Pipeline to generate the compressed ESS format, and decompress

#### To compress using ESSCompress:

| Input | Tool name | Output | 
| ---  | --- | ------------------------- |
| SRR957915.fa (FASTA reads/sequence file)| bcalm2 | SRR957915.unitigs.fa (cdbG in bcalm2 format) |
| SRR957915.unitigs.fa | ESSCompress | SRR957915.unitigs.ess |
| SRR957915.unitigs.ess	 |  MFCompressC | SRR957915.unitigs.ess.mfc |


- run bcalm2 on SRR957915.fa
- run ESS on SRR957915.unitgs.fa
- in SRR957915.unitgs.ess, replace all non-dna characters with characters recognized by MFC 
 `cat SRR957915.unitgs.ess | tr "[" "a" | tr "]" "c" | tr "+" "g" | tr "-" "t" | awk -F=' ' '{print ""$1}' > SRR957915.unitgs.ess`
- run MFCompressC on SRR957915.unitgs.ess `MFCompressC SRR957915.unitgs.ess`
	- Final compressed output is SRR957915.unitgs.ess.mfc

#### To decompress to SPSS:
- `MFCompressD SRR957915.unitgs.ess.mfc`
	- output is in  SRR957915.unitgs.ess
- Replace back the characters:
	- 	`cat SRR957915.unitgs.ess | tr "a" "[" | tr "c" "]" | tr "g" "+" | tr "t" "-" | awk -F=' ' '{print ""$1}' > SRR957915.unitgs.ess`
- `decodeESS SRR957915.unitgs.ess`
	- output SPSS is in  SRR957915.ess.fa



## Validation of SPSS (check if two SPSS represents same set of k-mers)
	
###### Download [DSK](https://github.com/GATB/dsk)
- Follow the instructions to compile

###### Prepare the sorted k-mer sets from cdbG:

	./dsk  -file chol.unitigs.fa  -kmer-size 11 -abundance-min 1
	./dsk2ascii -file chol.unitigs.h5 -out chol.unitigs.kmers
	sort -k 1 -n chol.unitigs.kmers > chol.unitigs.kmers_sorted
	
###### Prepare the sorted k-mer sets from decompressed ESS output:

	./dsk  -file chol.unitigs.ess.fa  -kmer-size 11 -abundance-min 1 
	./dsk2ascii -file chol.unitigs.ess.h5 -out chol.unitigs.ess.kmers
	sort -k 1 -n chol.unitigs.ess.kmers > chol.unitigs.ess.kmers_sorted
	
###### Check if two sorted k-mer sets are identical:
	cmp chol.unitigs.kmers_sorted chol.unitigs.ess.kmers_sorted && echo '### SUCCESS: Files Are Identical! ###' || echo '### WARNING: Files Are Different! ###'



