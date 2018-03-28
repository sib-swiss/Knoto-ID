#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.853027,-0.24041,-0.463193 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_48.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff polynomial_invariant_48.txt "${sourcepath}"/output/polynomial_invariant_48.txt || exit $?
