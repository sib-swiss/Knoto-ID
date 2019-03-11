#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --arrow-polynomial --projection=0.50448,-0.617295,0.603694 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_222.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff polynomial_invariant_222.txt "${sourcepath}"/output/polynomial_invariant_222.txt || exit $?
