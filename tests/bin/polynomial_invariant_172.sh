#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --input-format=gauss --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_172.txt "${sourcepath}"/input/input_diagram_gauss_closed_1.txt || exit $? 
diff polynomial_invariant_172.txt "${sourcepath}"/output/polynomial_invariant_172.txt || exit $?
