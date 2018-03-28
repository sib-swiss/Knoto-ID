#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.973045,-0.203564,0.108378 --closure-method=open --output=convert_diagram_123.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_123.txt "${sourcepath}"/output/convert_diagram_123.txt || exit $?
