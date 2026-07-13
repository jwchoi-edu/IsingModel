import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from os import path
from glob import glob
from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument(
    "--file", help="input file name (default: latest output file)", default=None
)
parser.add_argument(
    "--interval",
    type=int,
    help="interval (ms/frame) for animation (default: 30)",
    default=30,
)
args = parser.parse_args()

steps = []
magnetizations = []
grids = []

if args.file:
    latest_file = args.file
else:
    file_list = glob(path.join("outputs", "*.txt"))

    latest_file = sorted(file_list)[-1]

print(f"Reading data from {latest_file}")

with open(latest_file, "r") as f:
    first_line = f.readline().strip().split()
    if not first_line:
        print("File is empty")
        exit(1)

    N = int(first_line[0])
    J = float(first_line[1])
    T = float(first_line[2])
    STEPS = int(first_line[3])
    step_interval = int(first_line[4])

    print(f"N={N}, J={J}, T={T}, STEPS={STEPS}, step_interval={step_interval}")

    lines = f.readlines()
    for i in range(0, len(lines), 2):
        if i + 1 >= len(lines):
            break

        step, mag = map(float, lines[i].strip().split())
        steps.append(int(step))
        magnetizations.append(mag)

        grid_flat = list(map(int, lines[i + 1].strip().split()))
        grid_2d = np.array(grid_flat).reshape(N, N)
        grids.append(grid_2d)

print(f"Total snapshots read: {len(grids)}")

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

im = ax1.imshow(grids[0], cmap="gray", vmin=-1, vmax=1)
plt.colorbar(im, ax=ax1, label="Spin Direction")
ax1.axis("off")

(line,) = ax2.plot([], [], color="blue", linewidth=1.5, label="Magnetization")
ax2.set_xlim(steps[0], steps[-1])
ax2.set_ylim(-1.1, 1.1)
ax2.set_xlabel("Monte Carlo Steps")
ax2.set_ylabel("Average Magnetization <M>")
ax2.set_title(f"Magnetization over Time (J={J})")
ax2.axhline(y=0, color="gray", linestyle="--", linewidth=0.8)
ax2.grid(True, linestyle=":", alpha=0.6)
ax2.legend(loc="upper right")


def update(frame):
    im.set_array(grids[frame])
    line.set_data(steps[: frame + 1], magnetizations[: frame + 1])
    return [im, line]


ani = animation.FuncAnimation(
    fig, update, frames=len(grids), interval=args.interval, blit=True
)

plt.tight_layout()
plt.show()
