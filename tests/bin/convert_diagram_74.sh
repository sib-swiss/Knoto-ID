#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.110058,-0.465177,0.878349 --closure-method=open --output=convert_diagram_74.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_74.txt "${sourcepath}"/output/convert_diagram_74.txt || exit $?
