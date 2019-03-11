#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --arrow-polynomial --projection=0.70585,0.28172,-0.64993 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_108.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff polynomial_invariant_108.txt "${sourcepath}"/output/polynomial_invariant_108.txt || exit $?
