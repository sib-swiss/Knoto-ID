#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.0559182,0.00650045,0.998414 --closure-method=direct --output=convert_diagram_69.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_69.txt "${sourcepath}"/output/convert_diagram_69.txt || exit $?
