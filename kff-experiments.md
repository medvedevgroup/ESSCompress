# Experiments for KFF paper


 
## Dataset accessions

Gallus
```
SRR105788
SRR105789
SRR105792
SRR105794
SRR197985
SRR197986
```

Human
```
ERR024163
ERR024164
ERR024165
ERR024166
ERR024167
ERR024168
ERR024169
ERR024170
ERR024171
ERR024172
ERR024173
ERR024174
ERR024175
ERR024176
ERR024177
ERR024178
ERR024180
ERR024183
ERR024184
ERR024185
ERR024186
```




## Steps

### Installation of ESS-Compress with KFF support (v3.1)

#### Requirements
- Linux operating system (64 bit)   
- Git   
- GCC >= 4.8 or a C++11 capable compiler   
- CMake 3.1+   

#### Install from source:


(Use [commit a1ed1c](https://github.com/medvedevgroup/ESSCompress/commit/a1ed1c2be7887fdd091d872c87077dca092c543a) for the experiments in paper)
```
git clone https://github.com/medvedevgroup/ESSCompress   
cd ESSCompress  
./INSTALL   
```

#### Alternatively, install from binaries:
Download the latest Linux 64-bit binaries (v3.0).  
`wget https://github.com/medvedevgroup/ESSCompress/releases/download/v3.1/essCompress-v3.1-linux-64.tar.gz`   
`tar xvzf essCompress-v3.1-linux-64.tar.gz`   
`cd essCompress-v3.1/`


### Experiments with ESS-Compress and KFF

1. Put the path to downloaded datasets in fastq.gz format in a file (i.e. for gallus, put them in `gallus_lst` and for human, `human_lst`) and generate the KFF file using [KMC](https://github.com/refresh-bio/KMC/) v3.2.1. The commands look like this:
`kmc -fq -okff -k32 -ci1 @gallus_lst chicken tmp/`
`kmc -fq -okff -k32 -ci1 @human_lst human tmp/`

2. We start with `human.kff` and `gallus.kff`. To compress these two kff files, use:
```
bin/essCompress -i gallus.kff
bin/essCompress -i human.kff
```

By default, it will use single core. To use multiple cores (only BCALM and Blight module of ESS-Compress can use multiple cores),   

```
bin/essCompress -i gallus.kff -t 8   
bin/essCompress -i human.kff -t 8   
```

Final compressed outputs are `gallus.compressed.kff` and `human.compressed.kff`.

