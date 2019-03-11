#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --arrow-polynomial --names-db=internal --projection=-0.0696741,0.90602,0.417461 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_176.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff polynomial_invariant_176.txt "${sourcepath}"/output/polynomial_invariant_176.txt || exit $?
