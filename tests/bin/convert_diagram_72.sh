#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=-0.125153,-0.00423279,-0.992128 --closure-method=open --output=convert_diagram_72.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_72.txt "${sourcepath}"/output/convert_diagram_72.txt || exit $?
