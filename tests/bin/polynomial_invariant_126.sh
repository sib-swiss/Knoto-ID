#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --arrow-polynomial --projection=-0.0239407,-0.0862144,0.995989 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_126.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff polynomial_invariant_126.txt "${sourcepath}"/output/polynomial_invariant_126.txt || exit $?
