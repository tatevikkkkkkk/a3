import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('results.csv')
stream0 = df[df['stream_id'] == 0]

plt.figure(figsize=(10, 6))
plt.plot(stream0['stream_size'], stream0['true_count'], label='F_t^0', marker='o')
plt.plot(stream0['stream_size'], stream0['estimated_count'], label='N_t', marker='s')
plt.xlabel('Размер потока')
plt.ylabel('Количество уникальных')
plt.legend()
plt.grid(True)
plt.savefig('graph1.png')

stats = df.groupby('stream_size').agg({
    'estimated_count': ['mean', 'std'],
    'true_count': 'mean'
}).reset_index()

plt.figure(figsize=(10, 6))
plt.plot(stats['stream_size'], stats['estimated_count']['mean'], label='E(N_t)')
plt.fill_between(stats['stream_size'],
                 stats['estimated_count']['mean'] - stats['estimated_count']['std'],
                 stats['estimated_count']['mean'] + stats['estimated_count']['std'],
                 alpha=0.3)
plt.plot(stats['stream_size'], stats['true_count']['mean'], '--', label='F_t^0')
plt.xlabel('Размер потока')
plt.ylabel('Количество уникальных')
plt.legend()
plt.grid(True)
plt.savefig('graph2.png')

print('Графики сохранены!')
