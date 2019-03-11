#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --names-db=internal --projection=0.797734,0.225086,-0.559425 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_122.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff polynomial_invariant_122.txt "${sourcepath}"/output/polynomial_invariant_122.txt || exit $?
