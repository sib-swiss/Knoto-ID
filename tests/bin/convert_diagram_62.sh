#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.663087,-0.188345,-0.724459 --closure-method=open --output=convert_diagram_62.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_62.txt "${sourcepath}"/output/convert_diagram_62.txt || exit $?
