#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.117015,0.50821,0.853247 --closure-method=direct --output=convert_diagram_16.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_16.txt "${sourcepath}"/output/convert_diagram_16.txt || exit $?
