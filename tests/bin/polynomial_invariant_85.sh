#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=0.64315,0.765738,-0.00172649 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_85.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff polynomial_invariant_85.txt "${sourcepath}"/output/polynomial_invariant_85.txt || exit $?
