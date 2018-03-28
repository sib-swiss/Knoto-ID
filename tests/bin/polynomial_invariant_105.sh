#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --planar --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=0.681578,-0.640823,0.353266 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_105.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff polynomial_invariant_105.txt "${sourcepath}"/output/polynomial_invariant_105.txt || exit $?
