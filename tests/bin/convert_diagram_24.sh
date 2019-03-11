#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.581882,0.637601,-0.504855 --closure-method=open --output=convert_diagram_24.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_24.txt "${sourcepath}"/output/convert_diagram_24.txt || exit $?
