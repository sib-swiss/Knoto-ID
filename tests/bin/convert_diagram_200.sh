#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=gauss --output-format=pd --output=convert_diagram_200.txt "${sourcepath}"/input/input_diagram_gauss_open_sphere_1.txt || exit $? 
diff convert_diagram_200.txt "${sourcepath}"/output/convert_diagram_200.txt || exit $?
