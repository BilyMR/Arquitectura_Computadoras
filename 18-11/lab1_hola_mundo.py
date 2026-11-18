from qiskit import QuantumCircuit
from qiskit_aer import AerSimulator

# 1. Circuito con 2 cúbits y 2 bits clásicos
qc = QuantumCircuit(2, 2)

# 2. Superposición en cúbit 0 y entrelazamiento con cúbit 1
qc.h(0)
qc.cx(0, 1)

# 3. Medición
qc.measure([0, 1], [0, 1])

print("CIRCUITO CUÁNTICO:")
print(qc.draw(output="text"))

# 4. Simulación de 10,000 disparos
backend = AerSimulator()
shots = 10000
counts = backend.run(qc, shots=shots).result().get_counts(qc)

print(f"\nRESULTADOS DE LA MEDICIÓN ({shots} disparos):")
for bitstring, count in sorted(counts.items()):
    porcentaje = (count / shots) * 100
    print(f"Estado |{bitstring}>: {count} ocurrencias ({porcentaje:.2f}%)")
