#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --arrow-polynomial --names-db=internal --projection=-0.524262,0.820374,-0.228331 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_254.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_254.txt "${sourcepath}"/output/polynomial_invariant_254.txt || exit $?
