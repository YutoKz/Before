import numpy as np
from qulacs import QuantumState
from qulacs.gate import X, RY, DenseMatrix

n = 3
state = QuantumState(n)
state.set_zero_state()
print(state.get_vector())

# 1st-qubitにX操作 (|000> -> |010>)
index = 1
x_gate = X(index)

print(type(x_gate))

x_gate.update_quantum_state(state)
print(state.get_vector())

# 1st-qubitをYパウリでpi/4.0回転
angle = np.pi / 4.0
ry_gate = RY(index, angle)
ry_gate.update_quantum_state(state)
print(state.get_vector())

# 2nd-qubitにゲート行列で作成したゲートを作用
dense_gate = DenseMatrix(2, [[0,1],[1,0]])


dense_gate.update_quantum_state(state)
print(state.get_vector())

# ゲートの解放
del x_gate
del ry_gate
del dense_gate