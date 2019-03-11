#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --arrow-polynomial --names-db=internal --projection=0.0305894,-0.793571,-0.607708 --input-format=xyz --closure-method=open --nb-moves-III=100000 --output=polynomial_invariant_272.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff polynomial_invariant_272.txt "${sourcepath}"/output/polynomial_invariant_272.txt || exit $?
