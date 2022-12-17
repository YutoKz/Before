def list_repeatconcat(tx1, tx2,rep_num): 
    tx3=[] #新しい領域の確保
    if len(tx2) < len(tx1):
        #tx1がtx2より長かった場合，tx3にtx1のリストごとrep_num回追加し，最後にtx2を追加 
        for i in range(rep_num):
            tx3.append(tx1)
        tx3.append(tx2)
    else: 
        #tx2がtx1より長かった場合，tx3にtx2の要素全てをrep_num回追加し，最後にtx1を追加 
        for i in range(rep_num):
            tx3 += tx2
        tx3 += tx1
    return tx3

if __name__ == '__main__':
    val = list_repeatconcat([1,0,1], [1,1,1,0,1,0],2) 
    print(f"{val=}")
    val = list_repeatconcat([1,1,1,0,1,0], [1,0,1],3) 
    print(f"{val=}")
    val = list_repeatconcat([1,0,1,0], [1,1,1,0,1,0],4) 
    print(f"{val=}")