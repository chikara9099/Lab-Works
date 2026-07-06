import matplotlib.pyplot as plt
import numpy as np

# Data structured from the metric logs
weights = np.array([1.0, 1.2, 2.0, 5.0])

data = {
    "Case 1":  {"moves": [48, 48, 50, 110],  "nodes": [143357, 143357, 495, 562],   "time": [2717.0, 2945.1, 7.9, 12.9]},
    "Case 2":  {"moves": [49, 49, 61, 93],   "nodes": [74885, 74885, 1192, 463],   "time": [1388.7, 1426.4, 36.9, 8.3]},
    "Case 3":  {"moves": [53, 53, 61, 93],   "nodes": [2425638, 2425638, 904, 362], "time": [50588.8, 59384.5, 17.2, 8.2]},
    "Case 4":  {"moves": [50, 50, 66, 102],  "nodes": [35498, 35498, 1105, 1070],  "time": [700.3, 629.9, 21.8, 21.4]},
    "Case 5":  {"moves": [58, 58, 70, 94],   "nodes": [6473694, 6473694, 7420, 365], "time": [140571.0, 167406.8, 142.0, 6.9]},
    "Case 6":  {"moves": [45, 45, 51, 51],   "nodes": [107906, 107906, 792, 86],    "time": [2483.5, 2316.8, 14.4, 1.9]},
    "Case 7":  {"moves": [47, 47, 51, 73],   "nodes": [245337, 245337, 1314, 401],  "time": [5538.8, 5453.1, 27.0, 9.2]},
    "Case 8":  {"moves": [50, 50, 68, 92],   "nodes": [110953, 110953, 10351, 1816],"time": [2403.6, 2794.2, 223.7, 39.5]},
    "Case 9":  {"moves": [55, 55, 65, 93],   "nodes": [1606130, 1606130, 2136, 363], "time": [35473.9, 39797.1, 39.9, 6.3]},
    "Case 10": {"moves": [47, 47, 61, 75],   "nodes": [375353, 375353, 1155, 320],  "time": [8145.8, 8408.3, 22.7, 6.8]}
}

# Setup style aesthetics
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in plt.style.available else 'default')
fig_size = (7, 5)

# --- Graph 1: Nodes Explored vs Weights ---
plt.figure(figsize=fig_size)
for case_name, metrics in data.items():
    plt.plot(weights, metrics["nodes"], marker='o', label=case_name, alpha=0.8, linewidth=1.5)

plt.yscale('log')
plt.xlabel('Heuristic Weight ($W$)', fontsize=11, fontweight='bold')
plt.ylabel('Nodes Explored (Log Scale)', fontsize=11, fontweight='bold')
plt.title('Search Space Complexity vs. Exploration Weight', fontsize=12, fontweight='bold', pad=12)
plt.xticks(weights)
plt.legend(bbox_to_anchor=(1.04, 1), loc="upper left", frameon=True)
plt.tight_layout()
plt.savefig('nodes_vs_weight.png', dpi=300)
plt.close()

# --- Graph 2: Runtime vs Weights ---
plt.figure(figsize=fig_size)
for case_name, metrics in data.items():
    plt.plot(weights, metrics["time"], marker='s', linestyle='--', label=case_name, alpha=0.8, linewidth=1.5)

plt.yscale('log')
plt.xlabel('Heuristic Weight ($W$)', fontsize=11, fontweight='bold')
plt.ylabel('Runtime (ms) (Log Scale)', fontsize=11, fontweight='bold')
plt.title('Execution Computation Windows vs. Exploration Weight', fontsize=12, fontweight='bold', pad=12)
plt.xticks(weights)
plt.legend(bbox_to_anchor=(1.04, 1), loc="upper left", frameon=True)
plt.tight_layout()
plt.savefig('runtime_vs_weight.png', dpi=300)
plt.close()

# --- Graph 3: Moves vs Weights (Optional Quality Check) ---
plt.figure(figsize=fig_size)
for case_name, metrics in data.items():
    plt.plot(weights, metrics["moves"], marker='^', linestyle='-', label=case_name, alpha=0.8, linewidth=1.5)

plt.xlabel('Heuristic Weight ($W$)', fontsize=11, fontweight='bold')
plt.ylabel('Solution Length (Moves)', fontsize=11, fontweight='bold')
plt.title('Solution Suboptimality Path Drift vs. Exploration Weight', fontsize=12, fontweight='bold', pad=12)
plt.xticks(weights)
plt.legend(bbox_to_anchor=(1.04, 1), loc="upper left", frameon=True)
plt.tight_layout()
plt.savefig('moves_vs_weight.png', dpi=300)
plt.close()

print("Plot output complete. Saved files: 'nodes_vs_weight.png', 'runtime_vs_weight.png', and 'moves_vs_weight.png'.")