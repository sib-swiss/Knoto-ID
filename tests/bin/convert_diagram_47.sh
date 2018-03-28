#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.485512,0.477872,-0.732063 --closure-method=open --output=convert_diagram_47.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_47.txt "${sourcepath}"/output/convert_diagram_47.txt || exit $?
