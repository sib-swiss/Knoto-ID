#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.702496,-0.703964,0.104571 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_92.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff polynomial_invariant_92.txt "${sourcepath}"/output/polynomial_invariant_92.txt || exit $?
