#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db="${sourcepath}"/input/input_knot_names.txt --projection=-0.660272,-0.738738,-0.135308 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_19.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff polynomial_invariant_19.txt "${sourcepath}"/output/polynomial_invariant_19.txt || exit $?
