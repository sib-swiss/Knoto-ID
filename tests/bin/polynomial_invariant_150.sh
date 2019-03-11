#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --arrow-polynomial --projection=-0.270052,-0.585374,-0.764466 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_150.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_150.txt "${sourcepath}"/output/polynomial_invariant_150.txt || exit $?
