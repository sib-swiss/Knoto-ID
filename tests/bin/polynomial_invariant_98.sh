#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --names-db=internal --projection=0.197541,0.790977,-0.579079 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_98.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff polynomial_invariant_98.txt "${sourcepath}"/output/polynomial_invariant_98.txt || exit $?
