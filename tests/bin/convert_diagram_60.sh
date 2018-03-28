#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.633253,-0.736359,-0.238258 --closure-method=open --output=convert_diagram_60.txt "${sourcepath}"/input/input3.xyz || exit $? 
diff convert_diagram_60.txt "${sourcepath}"/output/convert_diagram_60.txt || exit $?
