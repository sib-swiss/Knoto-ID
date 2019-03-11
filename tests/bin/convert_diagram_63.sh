#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.98303,0.140806,0.117581 --closure-method=open --output=convert_diagram_63.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_63.txt "${sourcepath}"/output/convert_diagram_63.txt || exit $?
