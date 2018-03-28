#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=-0.142878,-0.0143657,-0.989636 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_10.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff polynomial_invariant_10.txt "${sourcepath}"/output/polynomial_invariant_10.txt || exit $?
