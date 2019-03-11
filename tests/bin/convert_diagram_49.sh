#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.148382,0.887022,0.437235 --closure-method=direct --output=convert_diagram_49.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_49.txt "${sourcepath}"/output/convert_diagram_49.txt || exit $?
