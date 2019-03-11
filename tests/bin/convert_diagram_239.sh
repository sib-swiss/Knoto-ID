#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=xyz --projection=0.235655,0.6205,0.747962 --closure-method=open --force --nb-iterations-relaxation=1000 --nb-points-spline=2 --output=stdout "${sourcepath}"/input/input1.xyz |cut -d' ' -f1-3 | ../src//polynomial_invariant --closure-method=open --output=convert_diagram_239.txt --projection=0,0,1 stdin || exit $? 
diff convert_diagram_239.txt "${sourcepath}"/output/convert_diagram_239.txt || exit $?
