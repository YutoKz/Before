from qulacs import QuantumState

n = 5
state = QuantumState(n)
state.set_zero_state()

# 状態ベクトルをnumpy arrayとして取得
data = state.get_vector()
print(data)