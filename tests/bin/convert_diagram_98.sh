#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=xyz --output-format=gauss --projection=-0.442997,-0.144189,-0.884852 --closure-method=direct --output=convert_diagram_98.txt "${sourcepath}"/input/input6.xyz || exit $? 
diff convert_diagram_98.txt "${sourcepath}"/output/convert_diagram_98.txt || exit $?
