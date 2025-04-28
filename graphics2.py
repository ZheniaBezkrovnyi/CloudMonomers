import matplotlib.pyplot as plt
import numpy as np
import os

# ======= ЗАГАЛЬНІ ФУНКЦІЇ =======

def read_coordination_numbers(file_path):
    removed_spheres = []
    coordination_numbers = []
    with open(file_path, "r") as file:
        for line in file:
            line = line.strip()
            if not line:
                continue
            if line.startswith("Removed spheres:"):
                removed_spheres.append(int(line.split(": ")[1]))
            elif line.startswith("After cluster processing"):
                coordination_numbers.append(float(line.split(", ")[1]))
    return removed_spheres, coordination_numbers

def read_clusters_distribution(file_path):
    clusters_by_removed = {}
    with open(file_path, "r") as file:
        lines = file.readlines()

    current_removed = None
    current_clusters = []
    for line in lines:
        line = line.strip()
        if not line:
            continue
        if line.startswith("Removed spheres:"):
            if current_removed is not None:
                clusters_by_removed[current_removed] = current_clusters
            current_removed = int(line.split(": ")[1])
            current_clusters = []
        elif line.startswith("Cluster Size, Count"):
            continue
        elif line[0].isdigit():
            size, count = map(int, line.split(","))
            current_clusters.append((size, count))

    if current_removed is not None:
        clusters_by_removed[current_removed] = current_clusters

    return clusters_by_removed

def read_timings(file_path):
    removed_spheres = []
    times = []
    with open(file_path, "r") as file:
        for line in file:
            line = line.strip()
            if not line:
                continue
            if line.startswith("Removed spheres:"):
                parts = line.split(", Time: ")
                if len(parts) == 2:
                    removed = int(parts[0].split(": ")[1])
                    time = float(parts[1].split(" ")[0])
                    removed_spheres.append(removed)
                    times.append(time)
    return removed_spheres, times

# ======= ПОБУДОВА ГРАФІКІВ =======

def build_coordination_number_graph():
    removed_spheres, coordination_numbers = read_coordination_numbers("coordination_numbers.txt")

    plt.figure(figsize=(10, 6))
    plt.plot(removed_spheres, coordination_numbers, marker="o", linestyle="-", color="blue", label="Average Coordination Number")
    plt.xlabel("Removed Spheres")
    plt.ylabel("Coordination Number")
    plt.title("Coordination Number vs Removed Spheres")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    os.makedirs("graphs", exist_ok=True)
    plt.savefig("graphs/coordination_number_vs_removed_spheres.png")
    plt.show()

def build_cluster_count_graph():
    clusters_by_removed = read_clusters_distribution("clusters_distribution.txt")

    removed_spheres = sorted(clusters_by_removed.keys())
    cluster_counts = [len(clusters_by_removed[r]) for r in removed_spheres]

    plt.figure(figsize=(10, 6))
    plt.plot(removed_spheres, cluster_counts, marker="o", linestyle="-", color="red", label="Number of Clusters")
    plt.xlabel("Removed Spheres")
    plt.ylabel("Number of Clusters")
    plt.title("Cluster Count vs Removed Spheres")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    os.makedirs("graphs", exist_ok=True)
    plt.savefig("graphs/cluster_count_vs_removed_spheres.png")
    plt.show()

def build_timings_graph():
    removed_spheres, times = read_timings("timings.txt")

    plt.figure(figsize=(10, 6))
    plt.plot(removed_spheres, times, marker="o", linestyle="-", color="purple", label="Elapsed Time per Removal")
    plt.xlabel("Removed Spheres")
    plt.ylabel("Time (seconds)")
    plt.title("Computation Time vs Removed Spheres")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    os.makedirs("graphs", exist_ok=True)
    plt.savefig("graphs/computation_time_vs_removed_spheres.png")
    plt.show()

def build_cluster_size_distribution_subplots():
    clusters_by_removed = read_clusters_distribution("clusters_distribution.txt")
    output_dir = "graphs"
    os.makedirs(output_dir, exist_ok=True)

    target_removed = [9000, 18000, 27000]  # 30%, 60%, 90%
    colors = ["blue", "green", "red"]
    titles = ["30% removed", "60% removed", "90% removed"]

    fig, axes = plt.subplots(3, 1, figsize=(10, 15))
    fig.subplots_adjust(hspace=0.5)

    for ax, removed, title, color in zip(axes, target_removed, titles, colors):
        closest_removed = min(clusters_by_removed.keys(), key=lambda x: abs(x - removed))
        clusters = clusters_by_removed[closest_removed]

        x = []
        y = []
        for size, count in clusters:
            if size >= 2:  # тільки розміри >= 2
                x.append(size)
                y.append(count)

        ax.plot(x, y, marker="o", linestyle="-", color=color)
        ax.set_title(title)
        ax.set_xlabel("Cluster Size")
        ax.set_ylabel("Number of Clusters")
        ax.grid(True)
        ax.set_yscale("log")

        if x:  # якщо є хоча б один елемент
            if max(x) > 200:
                ax.set_xscale("log")
            else:
                ax.set_xscale("linear")
        else:
            print(f"No valid data for plotting at {removed} spheres removed.")

    print("\n--- Debug Info ---")
    print("Available removed spheres:", list(clusters_by_removed.keys()))

    plt.tight_layout()
    plt.savefig(os.path.join(output_dir, "cluster_size_distribution_subplots.png"))
    plt.show()

    print("\n✅ Cluster size distribution subplots saved!")

# ======= ЗАПУСК ВСІХ ГРАФІКІВ =======

if __name__ == "__main__":
    print("\nBuilding graphs...\n")
    build_coordination_number_graph()
    build_cluster_count_graph()
    build_timings_graph()
    build_cluster_size_distribution_subplots()
    print("\n✅ All graphs successfully built and saved!")
