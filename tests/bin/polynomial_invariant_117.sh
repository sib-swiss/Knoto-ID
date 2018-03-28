#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=0.153079,-0.739867,-0.655106 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_117.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff polynomial_invariant_117.txt "${sourcepath}"/output/polynomial_invariant_117.txt || exit $?
