#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.0106793,0.20766,0.978143 --closure-method=open --output=convert_diagram_10.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_10.txt "${sourcepath}"/output/convert_diagram_10.txt || exit $?
