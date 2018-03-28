#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --planar --projection=-0.550663,-0.381652,0.742369 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_96.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff polynomial_invariant_96.txt "${sourcepath}"/output/polynomial_invariant_96.txt || exit $?
