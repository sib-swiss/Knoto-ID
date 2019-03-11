#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=xyz --projection=-0.274352,0.960983,0.0352448 --closure-method=open --planar --force --nb-iterations-relaxation=1000 --nb-points-spline=2 --output=stdout "${sourcepath}"/input/input5.xyz |cut -d' ' -f1-3 | ../src//polynomial_invariant --closure-method=open --planar --output=convert_diagram_246.txt --projection=0,0,1 stdin || exit $? 
diff convert_diagram_246.txt "${sourcepath}"/output/convert_diagram_246.txt || exit $?
