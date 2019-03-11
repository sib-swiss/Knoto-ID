#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.977007,-0.137021,-0.163347 --closure-method=direct --output=convert_diagram_81.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_81.txt "${sourcepath}"/output/convert_diagram_81.txt || exit $?
