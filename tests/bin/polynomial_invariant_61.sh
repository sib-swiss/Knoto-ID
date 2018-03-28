#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=0.474333,0.866142,-0.157498 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_61.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff polynomial_invariant_61.txt "${sourcepath}"/output/polynomial_invariant_61.txt || exit $?
