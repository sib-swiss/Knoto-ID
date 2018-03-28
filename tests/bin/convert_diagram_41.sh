#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=pd --projection=-0.581443,0.00231199,0.813584 --closure-method=open --output=convert_diagram_41.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_41.txt "${sourcepath}"/output/convert_diagram_41.txt || exit $?
