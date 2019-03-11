#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --planar --names-db=internal --projection=0.154997,0.532956,-0.831826 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_314.txt "${sourcepath}"/input/input9.xyz || exit $? 
diff polynomial_invariant_314.txt "${sourcepath}"/output/polynomial_invariant_314.txt || exit $?
