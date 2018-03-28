#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --planar --projection=0.467987,0.142653,-0.872146 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_80.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_80.txt "${sourcepath}"/output/polynomial_invariant_80.txt || exit $?
