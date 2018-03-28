#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.363345,0.154231,0.9188 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_76.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_76.txt "${sourcepath}"/output/polynomial_invariant_76.txt || exit $?
