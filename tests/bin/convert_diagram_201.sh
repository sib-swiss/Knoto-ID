#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=gauss --output-format=pd --output=convert_diagram_201.txt "${sourcepath}"/input/input_diagram_gauss_open_sphere_1.txt || exit $? 
diff convert_diagram_201.txt "${sourcepath}"/output/convert_diagram_201.txt || exit $?
