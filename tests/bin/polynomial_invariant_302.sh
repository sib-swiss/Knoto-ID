#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --arrow-polynomial --names-db=internal --projection=-0.945426,0.311648,0.0951018 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_302.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff polynomial_invariant_302.txt "${sourcepath}"/output/polynomial_invariant_302.txt || exit $?
