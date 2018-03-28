#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --planar --projection=-0.360663,-0.263642,-0.894659 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_88.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff polynomial_invariant_88.txt "${sourcepath}"/output/polynomial_invariant_88.txt || exit $?
