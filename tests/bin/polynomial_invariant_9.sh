#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --projection=-0.0843676,-0.962164,0.25908 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_9.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff polynomial_invariant_9.txt "${sourcepath}"/output/polynomial_invariant_9.txt || exit $?
