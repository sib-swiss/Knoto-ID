#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=xyz --projection=0.572601,0.138764,-0.808005 --closure-method=open --planar --force --nb-iterations-relaxation=1000 --nb-points-spline=2 --output=stdout "${sourcepath}"/input/input1.xyz |cut -d' ' -f1-3 | ../src//polynomial_invariant --closure-method=open --planar --output=convert_diagram_238.txt --projection=0,0,1 stdin || exit $? 
diff convert_diagram_238.txt "${sourcepath}"/output/convert_diagram_238.txt || exit $?
