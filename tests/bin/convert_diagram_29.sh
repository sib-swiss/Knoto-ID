#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.550431,0.154956,0.820375 --closure-method=open --output=convert_diagram_29.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_29.txt "${sourcepath}"/output/convert_diagram_29.txt || exit $?
