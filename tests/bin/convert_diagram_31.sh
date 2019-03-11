#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.353998,-0.907596,0.225732 --closure-method=open --output=convert_diagram_31.txt "${sourcepath}"/input/input1.xyz || exit $? 
diff convert_diagram_31.txt "${sourcepath}"/output/convert_diagram_31.txt || exit $?
