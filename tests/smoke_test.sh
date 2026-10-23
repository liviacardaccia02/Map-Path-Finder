#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <path-to-graph_traversal>" >&2
  exit 2
fi

binary="$1"
fixtures="tests/fixtures"

assert_contains() {
  local output="$1"
  local expected="$2"
  local context="$3"
  if ! grep -Fq "$expected" <<<"$output"; then
    echo "FAIL: $context (missing: $expected)" >&2
    echo "$output" >&2
    exit 1
  fi
}

assert_rejected() {
  local fixture="$1"
  local label="$2"
  if "$binary" --start 1 --end 2 --algorithm dijkstra --file "$fixture" --mode text >/dev/null 2>&1; then
    echo "FAIL: $label was accepted." >&2
    exit 1
  fi
  echo "PASS: $label is rejected"
}

# Existing regression: all algorithms must be able to reach a sink destination.
for algorithm in bfs dijkstra astar; do
  output="$("$binary" --start 1 --end 3 --algorithm "$algorithm" --file "$fixtures/sink_destination.csv" --mode text)"
  assert_contains "$output" "Total vertices on path from start to end = 3" "$algorithm reaches a sink destination"
  echo "PASS: $algorithm finds a path to a sink vertex"
done

# BFS minimizes edge count; Dijkstra and A* minimize total weight.
bfs_output="$("$binary" --start 1 --end 4 --algorithm bfs --file "$fixtures/weighted_routes.csv" --mode text)"
assert_contains "$bfs_output" "Total vertices on path from start to end = 2" "BFS chooses the fewest-edge route"
assert_contains "$bfs_output" "id =        4, length = 100.00" "BFS reports the weight of its selected route"
echo "PASS: BFS chooses the route with the fewest edges"

for algorithm in dijkstra astar; do
  output="$("$binary" --start 1 --end 4 --algorithm "$algorithm" --file "$fixtures/weighted_routes.csv" --mode text)"
  assert_contains "$output" "Total vertices on path from start to end = 4" "$algorithm chooses the minimum-cost route"
  assert_contains "$output" "id =        4, length = 3.00" "$algorithm reports minimum total cost"
  echo "PASS: $algorithm chooses the minimum-cost route"
done

# An isolated destination must not produce a false path.
for algorithm in bfs dijkstra astar; do
  output="$("$binary" --start 1 --end 5 --algorithm "$algorithm" --file "$fixtures/weighted_routes.csv" --mode text)"
  assert_contains "$output" "No path found from vertex 1 to vertex 5." "$algorithm handles unreachable destination"
  echo "PASS: $algorithm handles an unreachable destination"
done

# Same start and destination is a handled boundary case in the current CLI.
for algorithm in bfs dijkstra astar; do
  output="$("$binary" --start 1 --end 1 --algorithm "$algorithm" --file "$fixtures/weighted_routes.csv" --mode text)"
  assert_contains "$output" "Start and end" "$algorithm handles identical start and destination"
  echo "PASS: $algorithm handles identical start and destination"
done

assert_rejected "$fixtures/malformed_record.csv" "malformed record"
assert_rejected "$fixtures/negative_weight.csv" "negative edge weight"
assert_rejected "$fixtures/infinite_weight.csv" "infinite edge weight"
echo "All smoke tests passed."
