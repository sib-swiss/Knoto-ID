#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --projection=0.401552,0.836762,-0.372272 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_216.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff polynomial_invariant_216.txt "${sourcepath}"/output/polynomial_invariant_216.txt || exit $?
