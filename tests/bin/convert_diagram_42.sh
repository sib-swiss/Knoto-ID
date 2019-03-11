#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.829178,-0.178838,-0.529605 --closure-method=open --output=convert_diagram_42.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_42.txt "${sourcepath}"/output/convert_diagram_42.txt || exit $?
