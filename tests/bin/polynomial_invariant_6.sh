#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --projection=-0.322008,0.796015,-0.512514 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_6.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_6.txt "${sourcepath}"/output/polynomial_invariant_6.txt || exit $?
