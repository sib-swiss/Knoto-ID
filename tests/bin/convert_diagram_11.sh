#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.58838,-0.457873,0.666455 --closure-method=open --output=convert_diagram_11.txt "${sourcepath}"/input/input0.xyz || exit $? 
diff convert_diagram_11.txt "${sourcepath}"/output/convert_diagram_11.txt || exit $?
