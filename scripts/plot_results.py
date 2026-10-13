import csv
from pathlib import Path
import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parent.parent
RESULTS_DIR = ROOT / "results"
CSV_FILE = RESULTS_DIR / "benchmark_results.csv"
OUTPUT_FILE = RESULTS_DIR / "benchmark_comparison.png"
def main():
    if not CSV_FILE.exists():
        raise SystemExit(
            "Benchmark results not found. "
            "Run the benchmark program first."
        )
    mechanisms = []
    timings = []
    with CSV_FILE.open("r", newline="", encoding="utf-8") as file:
        reader = csv.DictReader(file)
        for row in reader:
            if row["expected"] != row["actual"]:
                print(
                    f"Warning: {row['mechanism']} produced "
                    "an incorrect counter."
                )
            mechanisms.append(row["mechanism"])
            timings.append(float(row["time_seconds"]))
    if not mechanisms:
        raise SystemExit("No benchmark results found.")
    plt.figure(figsize=(8, 5))
    plt.bar(mechanisms, timings)
    plt.title("Mutex vs Spinlock Execution Time")
    plt.xlabel("Synchronization Mechanism")
    plt.ylabel("Execution Time (seconds)")
    plt.tight_layout()
    plt.savefig(OUTPUT_FILE, dpi=150)
    print(f"Chart saved to: {OUTPUT_FILE}")
    plt.show()
if __name__ == "__main__":
    main()
