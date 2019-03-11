#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --arrow-polynomial --projection=-0.885947,0.404946,-0.22609 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_246.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_246.txt "${sourcepath}"/output/polynomial_invariant_246.txt || exit $?
