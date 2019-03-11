#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.91003,0.00264075,-0.414535 --closure-method=open --output=convert_diagram_95.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_95.txt "${sourcepath}"/output/convert_diagram_95.txt || exit $?
