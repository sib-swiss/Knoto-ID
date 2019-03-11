#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --arrow-polynomial --names-db=internal --projection=-0.215023,0.266243,0.939617 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_182.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff polynomial_invariant_182.txt "${sourcepath}"/output/polynomial_invariant_182.txt || exit $?
