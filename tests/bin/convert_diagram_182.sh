#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --close-diagram --input-format=pd --planar --output-format=gauss --output=convert_diagram_182.txt "${sourcepath}"/input/input_diagram_open_plane_2.txt || exit $? 
diff convert_diagram_182.txt "${sourcepath}"/output/convert_diagram_182.txt || exit $?
