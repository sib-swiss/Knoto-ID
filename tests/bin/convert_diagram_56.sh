#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.515953,-0.502934,0.693434 --closure-method=open --output=convert_diagram_56.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_56.txt "${sourcepath}"/output/convert_diagram_56.txt || exit $?
