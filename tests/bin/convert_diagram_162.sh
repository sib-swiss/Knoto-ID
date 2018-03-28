#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=pd --output-format=gauss --output=convert_diagram_162.txt "${sourcepath}"/input/input_diagram_open_sphere_0.txt || exit $? 
diff convert_diagram_162.txt "${sourcepath}"/output/convert_diagram_162.txt || exit $?
