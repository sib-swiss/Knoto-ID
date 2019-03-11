#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --names-db="${sourcepath}"/input/input_knotoid_names.txt --projection=0.197541,0.790977,-0.579079 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_97.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff polynomial_invariant_97.txt "${sourcepath}"/output/polynomial_invariant_97.txt || exit $?
