#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.989426,-0.043271,-0.138434 --closure-method=open --output=convert_diagram_15.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_15.txt "${sourcepath}"/output/convert_diagram_15.txt || exit $?
