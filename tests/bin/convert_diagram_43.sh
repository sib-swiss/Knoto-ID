#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.14468,0.455302,0.878503 --closure-method=open --output=convert_diagram_43.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_43.txt "${sourcepath}"/output/convert_diagram_43.txt || exit $?
