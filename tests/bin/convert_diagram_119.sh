#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=0.0731623,0.996372,-0.0434834 --closure-method=direct --output=convert_diagram_119.txt "${sourcepath}"/input/input7.xyz || exit $? 
diff convert_diagram_119.txt "${sourcepath}"/output/convert_diagram_119.txt || exit $?
