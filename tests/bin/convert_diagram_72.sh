#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.999259,0.0294422,0.0247985 --closure-method=open --output=convert_diagram_72.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_72.txt "${sourcepath}"/output/convert_diagram_72.txt || exit $?
