
#!/usr/bin/env bash

set -u

# Always run from the project root.
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR" || exit 1

mkdir -p results

echo "======================================"
echo " Linux Synchronization Case Study"
echo "======================================"

echo
echo "[1/3] Building available programs..."
if ! make; then
    echo "Build failed. Fix compilation errors before continuing."
    exit 1
fi

echo
echo "[2/3] Running experiments..."

PROGRAMS=(
    race_condition
    mutex_demo
    semaphore_demo
    spinlock_demo
    benchmark
)

FAILURES=0
RAN_BENCHMARK=0

for program in "${PROGRAMS[@]}"; do
    executable="bin/$program"

    if [[ -x "$executable" ]]; then
        echo
        echo "--------------------------------------"
        echo "Running: $program"
        echo "--------------------------------------"

        if ! "./$executable"; then
            echo "WARNING: $program failed."
            FAILURES=$((FAILURES + 1))
        fi

        if [[ "$program" == "benchmark" ]]; then
            RAN_BENCHMARK=1
        fi
    else
        echo "Skipping $program: source is missing or empty."
    fi
done

echo
echo "[3/3] Generating benchmark visualization..."

if [[ "$RAN_BENCHMARK" -eq 1 && -f scripts/plot_results.py ]]; then
    if python3 scripts/plot_results.py; then
        echo "Visualization completed."
    else
        echo "WARNING: Visualization failed."
        FAILURES=$((FAILURES + 1))
    fi
else
    echo "Skipping visualization: benchmark or plotting script unavailable."
fi

echo
echo "======================================"

if [[ "$FAILURES" -eq 0 ]]; then
    echo "All executed experiments completed successfully."
else
    echo "$FAILURES experiment(s) or visualization step(s) failed."
fi

echo "======================================"

exit "$((FAILURES > 0))"
