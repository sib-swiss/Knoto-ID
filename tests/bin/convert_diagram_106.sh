#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.794665,0.606678,0.0211845 --closure-method=open --output=convert_diagram_106.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_106.txt "${sourcepath}"/output/convert_diagram_106.txt || exit $?
