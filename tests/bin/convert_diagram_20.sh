#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=pd --projection=0.776921,0.6265,0.062386 --closure-method=direct --output=convert_diagram_20.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_20.txt "${sourcepath}"/output/convert_diagram_20.txt || exit $?
