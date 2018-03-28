#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=0.363345,0.154231,0.9188 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_77.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_77.txt "${sourcepath}"/output/polynomial_invariant_77.txt || exit $?
