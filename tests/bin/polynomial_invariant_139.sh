#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=0.4079,-0.906459,-0.109319 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_139.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff polynomial_invariant_139.txt "${sourcepath}"/output/polynomial_invariant_139.txt || exit $?
