#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --names-db=internal --projection=0.816082,0.567828,0.107616 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_242.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_242.txt "${sourcepath}"/output/polynomial_invariant_242.txt || exit $?
