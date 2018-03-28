#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=0.555576,0.814234,-0.168398 --closure-method=open --output=convert_diagram_46.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_46.txt "${sourcepath}"/output/convert_diagram_46.txt || exit $?
