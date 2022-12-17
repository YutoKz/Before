i = 0
print(i)

test = [] #空のリストを作成 
for i in range(10):
    test.append(i)
print(test) # [0,1,2,3,4,5,6,7,8,9]
print(len(test)) #リストの長さを取得

#いろいろなリスト要素へのアクセス方法 
print(test[0]) #最初の要素
print(test[-1]) #最後の要素
print(test[-2]) #最後から二番目の要素 
print(test[:]) #すべての要素
print(test[1:]) #2番目~最後まで
print(test[1:5]) #2番目~5番目
print(test[:-1]) #最初の要素~最後から2番目 
print(test[1:-1]) #2番目~最後から2番目