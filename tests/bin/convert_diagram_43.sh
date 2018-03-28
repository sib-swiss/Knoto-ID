#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.497016,-0.313017,-0.809318 --closure-method=open --output=convert_diagram_43.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_43.txt "${sourcepath}"/output/convert_diagram_43.txt || exit $?
