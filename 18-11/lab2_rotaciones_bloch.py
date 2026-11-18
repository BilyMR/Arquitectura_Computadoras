import math
from qiskit import QuantumCircuit
from qiskit_aer import AerSimulator

def simular_angulo(theta: float):
    qc = QuantumCircuit(1, 1)
    qc.ry(theta, 0)
    qc.measure(0, 0)
    
    backend = AerSimulator()
    return backend.run(qc, shots=10000).result().get_counts()

angulos = [
    (0.0, "Polo Norte (theta = 0)"),
    (math.pi / 4, "Ángulo 45° (theta = pi/4)"),
    (math.pi / 2, "Ecuador (theta = pi/2, Estado |+>)"),
    (math.pi, "Polo Sur (theta = pi, Estado |1>)")
]

for theta, desc in angulos:
    c = simular_angulo(theta)
    p0 = (c.get('0', 0) / 10000) * 100
    p1 = (c.get('1', 0) / 10000) * 100
    print(f"{desc:35s} -> P(|0>): {p0:5.1f}% | P(|1>): {p1:5.1f}%")
