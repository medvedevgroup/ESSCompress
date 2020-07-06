set -e
mfcdESS $1
ENAME=$(basename $1 .mfc)
cat $1.d | tr "a" "{" | tr "}" "c" | tr "(" "g" | tr ")" "t" | awk -F=' ' '{print ""$1}' > $ENAME
essDecompress $ENAME
rm $1.d
