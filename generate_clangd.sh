#!/bin/bash

current_dir=$(pwd)

cat <<EOL > .clangd
CompileFlags:
  Add: [
    -I${current_dir}/include/,
    -I${current_dir}/include/dear_imgui/
  ]
EOL

echo ".clangd file created successfully in ${current_dir}"
