#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --arrow-polynomial --projection=-0.588384,0.523207,-0.616489 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_102.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff polynomial_invariant_102.txt "${sourcepath}"/output/polynomial_invariant_102.txt || exit $?
