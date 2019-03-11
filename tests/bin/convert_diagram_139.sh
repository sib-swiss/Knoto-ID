#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.825276,-0.30373,-0.476096 --closure-method=open --output=convert_diagram_139.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_139.txt "${sourcepath}"/output/convert_diagram_139.txt || exit $?
