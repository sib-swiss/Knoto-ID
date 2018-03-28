#!/bin/bash
set -eo pipefail
sourcepath="$1"
../src//convert_diagram --input-format=pd --planar --output-format=pd --output=convert_diagram_181.txt "${sourcepath}"/input/input_diagram_open_plane_2.txt || exit $? 
diff convert_diagram_181.txt "${sourcepath}"/output/convert_diagram_181.txt || exit $?
