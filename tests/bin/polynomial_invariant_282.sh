#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --projection=0.236317,0.879779,-0.412484 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_282.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff polynomial_invariant_282.txt "${sourcepath}"/output/polynomial_invariant_282.txt || exit $?
