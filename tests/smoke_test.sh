#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <path-to-graph_traversal>" >&2
  exit 2
fi

binary="$1"
fixture="tests/fixtures/sink_destination.csv"

for algorithm in bfs dijkstra astar; do
  output="$("$binary" --start 1 --end 3 --algorithm "$algorithm" --file "$fixture" --mode text)"
  if ! grep -q "Total vertices on path from start to end = 3" <<<"$output"; then
    echo "FAIL: $algorithm did not find the expected three-vertex path." >&2
    echo "$output" >&2
    exit 1
  fi
  echo "PASS: $algorithm finds a path to a sink vertex"
done

if "$binary" --start 1 --end 2 --algorithm dijkstra --file tests/fixtures/malformed_record.csv --mode text >/dev/null 2>&1; then
  echo "FAIL: malformed record was accepted." >&2
  exit 1
fi

echo "PASS: malformed graph record is rejected"
