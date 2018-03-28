#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.689829,-0.382185,-0.614875 --closure-method=open --output=convert_diagram_74.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_74.txt "${sourcepath}"/output/convert_diagram_74.txt || exit $?
