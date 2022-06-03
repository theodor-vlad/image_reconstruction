import json
import os
import numpy as np

with open('statistics.txt', 'w+') as output:
    for algorithm in ['ga', 'hcfi', 'hcbi', 'sa']:
        fitnesses = []
        d = '../image_processing/dna/' + algorithm + '/'
        for filename in os.listdir(d):
            full_path = os.path.join(d, filename)
            if os.path.isfile(full_path) and full_path.split('.')[-1] == 'json':
                with open(full_path) as f:
                    j = json.load(f)
                    if 'fitness' in j:
                        fitnesses.append(j['fitness'])
        
        output.write(f'-----------------{algorithm}-----------------\n')
        output.write(f'min fitness: {np.min(fitnesses)}\n')
        output.write(f'max fitness: {np.max(fitnesses)}\n')
        output.write(f'avg fitness: {np.average(fitnesses)}\n')
        output.write(f'fitness stdev: {np.std(fitnesses)}\n')
        output.write(f'fitness variance: {np.var(fitnesses)}\n\n')

