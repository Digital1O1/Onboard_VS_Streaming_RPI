import matplotlib.pyplot as plt
import numpy as np

# Sample data
categories = ['Category 1', 'Category 2', 'Category 3']
values = [np.random.rand(10) for _ in range(3)]

# Create subplots
fig, axes = plt.subplots(1, 3, figsize=(12, 4))

# Plot each category in a separate subplot
for i, category in enumerate(categories):
    axes[i].scatter(range(10), values[i])
    axes[i].set_title(category)

plt.tight_layout()
plt.show()