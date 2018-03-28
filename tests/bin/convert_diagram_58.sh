#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.36209,0.912544,-0.190144 --closure-method=open --output=convert_diagram_58.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_58.txt "${sourcepath}"/output/convert_diagram_58.txt || exit $?
