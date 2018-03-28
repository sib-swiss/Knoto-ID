#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=xyz --projection=-0.853482,0.284799,-0.436415 --closure-method=direct --planar --force --nb-iterations-relaxation=1000 --nb-points-spline=2 --output=stdout "${sourcepath}"/input/input5.xyz |cut -d' ' -f1-3 | ../src//polynomial_invariant --closure-method=direct --planar --output=convert_diagram_244.txt --projection=0,0,1 stdin || exit $? 
diff convert_diagram_244.txt "${sourcepath}"/output/convert_diagram_244.txt || exit $?
