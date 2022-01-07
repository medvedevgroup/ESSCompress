ESSCompress with KFF
===============

### Pre-requisites
* Download ESS-Compress from [https://github.com/medvedevgroup/ESSCompress/](https://github.com/medvedevgroup/ESSCompress/)
* Follow instructions in [https://github.com/medvedevgroup/ESSCompress/tree/kff-support/blight](https://github.com/medvedevgroup/ESSCompress/tree/kff-support/src) to install blight.
* Install kff-tools from [https://github.com/Kmer-File-Format/kff-tools](https://github.com/Kmer-File-Format/kff-tools)
* Install dsk from [https://github.com/GATB/dsk](https://github.com/GATB/dsk)

After obtaining all the pre-requisites, follow the two examples below to compress KFF file with ESS-Compress in UST mode.


#### An example to count and store the k-mers from fasta file "test.fa" in "test.kff" (Naive KFF)

* First get the k-mers and their counts using dsk

```sh
dsk -kmer-size 28 -file test.fa -abundance-min 1
dsk2ascii -file test.h5 -out test_naive.instr
```

* Next, get the kff file from "test_naive.instr"
```
kff-tools instr -i test_naive.instr -o test_naive.kff --kmer-size 28 -d 1  
```

#### An example to convert "naive KFF" file to compressed  "ESS-Compressed KFF" 

* First, get the k-mers and their counts from kff file in plaintext format

```
kff-tools outstr -i test_naive.kff > test2.instr 
```
* Get the k-mers in fasta format (test2.fa) and then run ESS-Compress (in UST mode) to obtain "test2.fa.essd"

```sh
cat test2.instr | cut -f1 -d" " | awk '{print ">contig\n" $0}' > test2.fa
essCompress -i test2.fa -k 28 -u 
```

* Use Blight to link the data and (using 8 cores) generate "test2.essd_instr"
```
blight/blight-ess -k 28 -i test2.fa.essd -d test2.instr -t 8   
```

* Finally, generate ESS-compressed KFF (test2_compressed.kff)

```
 kff-tools instr -i test2.essd_instr -o test2_compressed.kff -k 28 -d 2 -m $(bash blight/getmaxlen.sh test2.essd_instr) --delimiter ' ' --data-delimiter ','
``` 
 