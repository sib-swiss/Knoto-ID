#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=0.992719,-0.0925292,-0.0771216 --closure-method=direct --output=convert_diagram_85.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_85.txt "${sourcepath}"/output/convert_diagram_85.txt || exit $?
