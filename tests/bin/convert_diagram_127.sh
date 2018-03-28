#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.119876,0.0706124,-0.990274 --closure-method=open --output=convert_diagram_127.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_127.txt "${sourcepath}"/output/convert_diagram_127.txt || exit $?
