#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knot_names.txt --projection=-0.970094,-0.0578986,-0.235721 --input-format=xyz --closure-method=direct --nb-moves-III=100000 --output=polynomial_invariant_5.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_5.txt "${sourcepath}"/output/polynomial_invariant_5.txt || exit $?
