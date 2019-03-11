#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.839842,0.303107,0.450323 --closure-method=direct --output=convert_diagram_39.txt "${sourcepath}"/input/input2.xyz || exit $? 
diff convert_diagram_39.txt "${sourcepath}"/output/convert_diagram_39.txt || exit $?
