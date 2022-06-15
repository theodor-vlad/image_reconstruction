import json
import os
import numpy as np

with open('../output/statistics.txt', 'w+') as output:
    fitnesses_over_time = []
    for algo_index, algorithm in enumerate(['ga', 'hcfi', 'hcbi', 'sa']):
        fitnesses_over_time.append([])
        for _ in range(1800):
            fitnesses_over_time[algo_index].append(0)

        fitnesses = []
        d = '../output/dna/' + algorithm + '/'
        for filename in os.listdir(d):
            full_path = os.path.join(d, filename)
            if os.path.isfile(full_path) and full_path.split('.')[-1] == 'json':
                with open(full_path) as f:
                    j = json.load(f)
                    if 'fitness' in j:
                        fitnesses.append(j['fitness'])
                    if 'fitness_over_time' in j:
                        fitness_over_time = j['fitness_over_time']
                        for sec, score in fitness_over_time:
                            fitnesses_over_time[algo_index][sec - 1] += score

        if len(fitnesses) > 0:
            for i in range(1800):
                fitnesses_over_time[algo_index][i] /= len(fitnesses)

            seconds_to_hit_96 = next(i for i, x in enumerate(fitnesses_over_time[algo_index]) if (1.0 - x ** (-0.5) >= 0.96))
            seconds_to_hit_97 = next(i for i, x in enumerate(fitnesses_over_time[algo_index]) if (1.0 - x ** (-0.5) >= 0.97))
            seconds_to_hit_98 = next(i for i, x in enumerate(fitnesses_over_time[algo_index]) if (1.0 - x ** (-0.5) >= 0.98))

            with open('../output/' + algorithm + '_fitness_over_time.txt', 'w+') as algorithm_fitness_over_time:
                for i in range(1800):
                    algorithm_fitness_over_time.write(f'({i + 1}, ' + format(fitnesses_over_time[algo_index][i], '.4f') + ')\n')

            output.write(f'-----------------{algorithm}-----------------\n')
            output.write(f'sample size: {len(fitnesses)}\n')
            output.write(f'min fitness: {np.min(fitnesses)}\n')
            output.write(f'max fitness: {np.max(fitnesses)}\n')
            output.write(f'avg fitness: {np.average(fitnesses)}\n')
            output.write(f'fitness stdev: {np.std(fitnesses)}\n')
            output.write(f'fitness variance: {np.var(fitnesses)}\n')
            output.write(f'average time to hit 96% likeness: {seconds_to_hit_96}s\n')
            output.write(f'average time to hit 97% likeness: {seconds_to_hit_97}s\n')
            output.write(f'average time to hit 98% likeness: {seconds_to_hit_98}s\n\n')

