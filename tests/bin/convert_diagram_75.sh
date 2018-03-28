#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.437531,0.379277,0.815301 --closure-method=open --output=convert_diagram_75.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_75.txt "${sourcepath}"/output/convert_diagram_75.txt || exit $?
