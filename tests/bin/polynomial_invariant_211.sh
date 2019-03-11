#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=-0.744209,0.53618,0.398327 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_211.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff polynomial_invariant_211.txt "${sourcepath}"/output/polynomial_invariant_211.txt || exit $?
