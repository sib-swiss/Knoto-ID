#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db=internal --projection=-0.0843676,-0.962164,0.25908 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_11.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff polynomial_invariant_11.txt "${sourcepath}"/output/polynomial_invariant_11.txt || exit $?
