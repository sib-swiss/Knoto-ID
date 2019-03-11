#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//polynomial_invariant --input-format=pd --nb-moves-III=100000 --output=polynomial_invariant_336.txt "${sourcepath}"/input/input_diagram_open_sphere_1.txt || exit $? 
diff polynomial_invariant_336.txt "${sourcepath}"/output/polynomial_invariant_336.txt || exit $?
