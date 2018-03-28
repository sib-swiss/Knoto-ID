#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=xyz --output-format=gauss --projection=-0.302837,-0.601795,0.739007 --closure-method=direct --output=convert_diagram_71.txt "${sourcepath}"/input/input4.xyz || exit $? 
diff convert_diagram_71.txt "${sourcepath}"/output/convert_diagram_71.txt || exit $?
