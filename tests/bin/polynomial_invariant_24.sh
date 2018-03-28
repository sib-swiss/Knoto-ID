#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src/polynomial_invariant --projection=0.65406,-0.677159,0.337136 --input-format=xyz --closure-method=rays --nb-moves-III=100000 --output=polynomial_invariant_24.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff polynomial_invariant_24.txt "${sourcepath}"/output/polynomial_invariant_24.txt || exit $?
