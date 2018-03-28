#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=pd --output-format=pd --output=convert_diagram_165.txt "${sourcepath}"/input/input_diagram_open_sphere_1.txt || exit $? 
diff convert_diagram_165.txt "${sourcepath}"/output/convert_diagram_165.txt || exit $?
