#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.86737,0.497157,0.0224322 --closure-method=open --output=convert_diagram_27.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_27.txt "${sourcepath}"/output/convert_diagram_27.txt || exit $?
