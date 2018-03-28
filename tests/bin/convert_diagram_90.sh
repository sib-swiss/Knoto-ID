#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.671402,-0.105898,-0.733488 --closure-method=open --output=convert_diagram_90.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_90.txt "${sourcepath}"/output/convert_diagram_90.txt || exit $?
