#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.199945,0.940621,0.274325 --closure-method=direct --output=convert_diagram_85.txt "${sourcepath}"/input/input5.xyz || exit $? 
diff convert_diagram_85.txt "${sourcepath}"/output/convert_diagram_85.txt || exit $?
