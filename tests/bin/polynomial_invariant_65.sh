#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --planar --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=-0.757635,0.0448138,-0.651139 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_65.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff polynomial_invariant_65.txt "${sourcepath}"/output/polynomial_invariant_65.txt || exit $?
