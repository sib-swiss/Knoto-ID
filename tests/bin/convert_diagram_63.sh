#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.0215874,0.904021,-0.426942 --closure-method=open --output=convert_diagram_63.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_63.txt "${sourcepath}"/output/convert_diagram_63.txt || exit $?
