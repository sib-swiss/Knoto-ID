#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=-0.501717,-0.0470954,0.863749 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_16.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff polynomial_invariant_16.txt "${sourcepath}"/output/polynomial_invariant_16.txt || exit $?
