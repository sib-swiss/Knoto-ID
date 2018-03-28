#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.691738,-0.00496005,0.722131 --closure-method=open --output=convert_diagram_91.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_91.txt "${sourcepath}"/output/convert_diagram_91.txt || exit $?
