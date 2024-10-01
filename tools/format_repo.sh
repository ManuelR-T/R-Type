#!/bin/bash

dirs=("lib" "client" "include" "server")

echo "Formatting"
for dir in "${dirs[@]}"; do
  echo "  Formatting $dir..."; find "$dir" \( -iname "*.hpp" -o -iname "*.cpp" \) -exec clang-format -i {} +
done
echo "Formatting Done."