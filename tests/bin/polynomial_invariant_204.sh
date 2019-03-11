#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --arrow-polynomial --projection=-0.358947,0.611486,0.705154 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_204.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff polynomial_invariant_204.txt "${sourcepath}"/output/polynomial_invariant_204.txt || exit $?
