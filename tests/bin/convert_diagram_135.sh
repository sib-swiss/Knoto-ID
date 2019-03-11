#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.886518,0.327653,0.326694 --closure-method=direct --output=convert_diagram_135.txt "${sourcepath}"/input/input8.xyz || exit $? 
diff convert_diagram_135.txt "${sourcepath}"/output/convert_diagram_135.txt || exit $?
