#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=pd --planar --output-format=gauss --output=convert_diagram_178.txt "${sourcepath}"/input/input_diagram_open_plane_1.txt || exit $? 
diff convert_diagram_178.txt "${sourcepath}"/output/convert_diagram_178.txt || exit $?
