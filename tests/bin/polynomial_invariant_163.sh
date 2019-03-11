#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=0.746561,-0.450928,0.489194 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_163.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff polynomial_invariant_163.txt "${sourcepath}"/output/polynomial_invariant_163.txt || exit $?
