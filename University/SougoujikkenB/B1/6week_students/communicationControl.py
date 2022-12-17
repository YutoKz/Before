import numpy as np
import math

class CommunicationControl:
    def __init__(self, FREQUENCY0, FREQUENCY1, SAMPLING_RATE, BPS, IS_MANCHESTER):
        """インスタンス変数の初期化"""
        self.FREQUENCY0 = FREQUENCY0
        self.FREQUENCY1 = FREQUENCY1
        self.SAMPLING_RATE = SAMPLING_RATE
        self.BPS = BPS
        self.IS_MANCHESTER = IS_MANCHESTER
        self.BIT0SHIFT = int(SAMPLING_RATE / FREQUENCY0)
        self.BIT1SHIFT = int(SAMPLING_RATE / FREQUENCY1)
        self.SEGMENT = 96#int(SAMPLING_RATE / 500)

        self.my_id = [0, 0, 0, 1]
        self.receiver_id = [1, 1, 0, 0]
        self.state = "SLEEP" #通信状態制御用
        self.freq0_threshold = 0
        self.freq1_threshold = 0
        self.rem = np.empty(0)
        self.acc0 = []
        self.acc1 = []
        self.demod = []
        self.demod_temp_data = []
        self.demod_continue_data = [] #2022.11.2追加　データ保持用
        self.BITLENGTH = int(self.SAMPLING_RATE / self.BPS)
        self.DEC_TH = self.BITLENGTH / self.SEGMENT * 0.5 #ヒューリスティック
        self.demod_val = 1 #2022.11.2変更
        self.demod_count = 0
        self.demod_continue_flag = False
        self.demod_final_flag = False #2022.11.2　追加　連続処理判定用

        self.f = open('test.txt', 'w')

    def reset(self):
        self.freq0_threshold = 0
        self.freq1_threshold = 0
        print(f"th0:{self.freq0_threshold}, th1:{self.freq1_threshold}")

    def calibration(self, init_frame):
        self.reset()
        self.frequency_analysis_rx(init_frame, True)
        print(f"th0:{self.freq0_threshold}, th1:{self.freq1_threshold}")

    #data_in（リスト）からパリティを計算する。
    def calc_parity(self, data_in):
        parity = 0
        #ここに偶数パリティを計算するコードを書く。




        return parity

    def decimal_to_binarylist(self, dec, length):
        bin_list = []
        for _ in range(length):
            bin_list.append(dec % 2)
            dec = (dec >> 1)
        return bin_list[::-1]

    def binarylist_to_decimal(self, bin_list):
        dec = 0
        for bit in bin_list:
            dec = (dec << 1)
            dec += bit
        return dec

    def amplitude_analysis_rx(self, rx_wave, calib_flag): #ASK復調実現のために追加した関数です
        num_segment = int(len(rx_wave) / self.SEGMENT)
        self.acc0 = []
        self.acc1 = []
        self.demod = []
        W=16 #LPFでの設定値をいろいろと変えて検討してみてください

        #2022.11.9 これ以降でBPFを実装
        f_L = 3000    #条件変更時反映！！
        f_H = 4000    #条件変更時反映！！
        f_Hs = 2.0 * f_H / self.SAMPLING_RATE
        f_Ls = 2.0 * f_L / self.SAMPLING_RATE

        N = 150       

        rx_bpf = np.zeros(len(rx_wave))
        h_bpf  = np.zeros(N)
        
        '''
        失敗
        for i in range(len(rx_wave)):
            for k in range(N):
                if ((i + k - 1) < len(rx_wave)):
                    rx_bpf[i] += h_bpf[k] * rx_wave[i + k - 1]
                else:
                    rx_bpf[i] = rx_wave[i]
                    break
        '''

        '''
        失敗
        array1= np.arange(-0.5 * N, 0.5*N)
        h_bpf = f_Hs * np.sinc(f_Hs * array1) - f_Ls * np.sinc(f_Ls * array1)
     
        rx_bpf[0] = rx_wave[0]

        tmp = len(rx_wave) - N + 1

        array2= np.arange(1, tmp)
        rx_bpf_tmp = h_bpf[0:N] * rx_wave[(array2 - 1):(array2 + N - 1)]
        rx_bpf[1:tmp] = map(lambda x: x.sum, rx_bpf_tmp)

        rx_bpf[tmp:-1] = rx_wave[tmp:-1]
        '''

        # 畳み込み積分に既存の関数を利用
        array1= np.arange(-0.5 * N, 0.5*N)
        h_bpf = f_Hs * np.sinc(f_Hs * array1) - f_Ls * np.sinc(f_Ls * array1)
        rx_bpf = np.convolve(rx_wave, h_bpf, mode="same")


        #2022.11.9 Mixingでnp.rx_mixに代入するものをrx_waveから変更する
        #受信信号rx_waveの全要素を2乗し，その値をrx_mixに代入する
        #今はrx_waveと同じ長さの0を代入しているので，ここを変更してください
        np.rx_mix = np.power(rx_bpf, 2)
                
        for seg in range(num_segment):
            acc0temp = 0
            acc1temp = 0
            start_idx = seg * self.SEGMENT
            end_idx0 = seg * self.SEGMENT + self.BIT0SHIFT
            end_idx1 = seg * self.SEGMENT + self.BIT1SHIFT

            #以下でLPFを実装 2022.10.26課題
            #acc1tempに2乗したrx_minの信号の移動平均を入力する
            if end_idx0 + self.SEGMENT <= len(np.rx_mix) and end_idx1 + self.SEGMENT <= len(np.rx_mix):
                for idx in range(W): #このままだとおそらくエラーが出ます．平均ウインドウW分だけfor文が回るように変更してください．
                    acc0temp += rx_wave[start_idx + idx] #比較用のためにrx_waveを加算しています．
                    acc1temp += np.rx_mix[start_idx + idx] #rx_waveを参考に，np.rx_mixを加算するための式に変更してください
            else:
                #self.rem = rx_wave[start_idx:]
                break
                
            #LPFを完成させるために，acc0temp, acc1tempの平均を求め，同じ変数(acc0temp, acc1temp)に代入するコードを記入してください．
            #重要なのはacc1tempだけですが，結果の表示・比較のため，acc0tempの方も同じく平均を求めてください．


            #結果を表示させるためのコード
            #2022.11.2 第4回目の結果が確認できたら，"20"は削除，もしくは変更してください
            self.acc0.append(acc0temp*0.1) #デバッググラフ表示用 20という値は音量・環境によって調整してください（グラフの赤色の線が振り切っていたりした場合など）
            self.acc1.append(acc1temp*0.1) #デバッググラフ表示用 20という値は音量・環境によって調整してください（グラフの赤色の線が振り切っていたりした場合など）

            #2022.11.2 以下のif-else文をコメントアウトしてください
            #if acc1temp > self.freq1_threshold:
            #    self.demod.append(1)
            #else:
            #    self.demod.append(0)

            if calib_flag:
                if self.freq0_threshold < acc0temp:
                    self.freq0_threshold = acc0temp * 10
                if self.freq1_threshold < acc1temp:
                    self.freq1_threshold = acc1temp * 10
        
        #2022.11.2 これ以下でacc1の平均値を求めて，それを閾値と設定する．
        #閾値を基準として，acc1の0-1を判定し，その結果をdemod, demod__continue_dataに追加
        #インデントの位置に注意（for seg in range(num_segment)の外側で実行が必要）
        #スマホからの信号が入る前の判定も必要(閾値の値が余りにも小さい場合はdemodなどに0が入るように設定)
        #demodなどへの追加方法は上のコメントアウトした部分を参考にしてください．
        #demodに値を入れないまま実行するとエラーが出て止まります．

        threshold = np.mean(self.acc1)
        for a in self.acc1:
            if(a >= threshold):
                self.demod.append(1)
                self.demod_continue_data.append(1)
            else:
                self.demod.append(0)
                self.demod_continue_data.append(0)

        

        return self.demod
 
    def frequency_analysis_rx(self, rx_wave, calib_flag):
        #rx_wave = np.hstack((self.rem, rx_wave))
        num_segment = int(len(rx_wave) / self.SEGMENT)
        self.acc0 = []
        self.acc1 = []
        self.demod = []
        for seg in range(num_segment):
            acc0temp = 0
            acc1temp = 0
            start_idx = seg * self.SEGMENT
            end_idx0 = seg * self.SEGMENT + self.BIT0SHIFT
            end_idx1 = seg * self.SEGMENT + self.BIT1SHIFT
            # end_idxS = int(seg * self.SEGMENT + self.BIT1SHIFT / 1.5)
            if end_idx0 + self.SEGMENT <= len(rx_wave) and end_idx1 + self.SEGMENT <= len(rx_wave):
                for idx in range(self.SEGMENT):
                    acc0temp += rx_wave[start_idx + idx] * rx_wave[end_idx0 + idx]
                    acc1temp += rx_wave[start_idx + idx] * rx_wave[end_idx1 + idx]
                    # accStemp += rx_wave[start_idx + idx] * rx_wave[end_idxS + idx]
            else:
                #self.rem = rx_wave[start_idx:]
                break
            self.acc0.append(acc0temp) #デバッググラフ表示用
            self.acc1.append(acc1temp) #デバッググラフ表示用
            if acc0temp > self.freq0_threshold or acc1temp > self.freq1_threshold:
                if acc0temp > acc1temp:
                    self.demod.append(0)
                else:
                    self.demod.append(1)
            else:
                self.demod.append(-1)

            if calib_flag:
                if self.freq0_threshold < acc0temp:
                    self.freq0_threshold = acc0temp
                if self.freq1_threshold < acc1temp:
                    self.freq1_threshold = acc1temp

        return self.demod

    def noise_reduction_rx(self, demod_data):
        for i in range(len(demod_data)):
            if i > 0 and i < len(demod_data) - 1:
                if demod_data[i - 1] == demod_data[i + 1] and demod_data[i] != demod_data[i - 1]:
                    demod_data[i] = demod_data[i - 1]
                if demod_data[i - 1] != -1 and demod_data[i + 1] != -1 and demod_data[i] == -1:
                    demod_data[i] = demod_data[i + 1]
        return demod_data

    def demodulation_rx(self, demod_data):
        #print(f"demod_data{demod_data}")
        demod_data = self.noise_reduction_rx(demod_data)
        #print(f"demod_data{demod_data}")
        return_demod_data = []

        for i in range(len(demod_data)):
            if self.demod_val == demod_data[i]:
                self.demod_count += 1
            else:
                if self.demod_val != -1 and self.demod_count > self.DEC_TH:
                    self.demod_temp_data.append(self.demod_val)
                    if self.demod_count > self.DEC_TH * 2:
                        self.demod_temp_data.append(self.demod_val)

                if demod_data[i] == -1:
                    if len(self.demod_temp_data) > 8:
                        return_demod_data.append(self.demod_temp_data)
                    self.demod_temp_data = []
                    self.demod_continue_flag = False
                else:
                    self.demod_continue_flag = True

                self.demod_val = demod_data[i]
                self.demod_count = 1

        # print(f"self.demod_val{self.demod_val}")
        # print(f"self.demod_count{self.demod_count}")
        # print(f"self.demod_temp_data{self.demod_temp_data}")
        # print(f"return_demod_data{return_demod_data}")
        return return_demod_data
    
    def demodulation_rx_ask(self, demod_data): #2022.11.2追加：ASK用demodulation関数の作成
        #print(f"demod_data{demod_data}")
        demod_data = self.noise_reduction_rx(demod_data)
        #print(f"demod_data{demod_data}")
        return_demod_data = []

        #2022.11.2: 以下でビットの判定を実施
        #まず，flagで1個目のビット判定か，2個目以降のビット判定かを振り分けます
        #if文をうまく活用してください
        
        
        #1個目のビット判定が終わったら，2個目以降のビット判定を行ってください

        for i in range(len(demod_data)):
            if(self.demod_continue_flag):
                self.demod_count += 1
                if(self.demod_count >= self.DEC_TH * 2.0):
                    return_demod_data.append(demod_data[i])
                    self.demod_count = 0
            else:
                if(demod_data[i] == 1):
                    self.demod_count += 1
                else:
                    self.demod_count = 0

                if(self.demod_count > self.DEC_TH):
                    self.demod_continue_flag = True
                    self.demod_count = 0
                    return_demod_data.append(demod_data[i])

            

        #self.f.write('\n')
        for i in return_demod_data:
            self.f.write(str(i) + ' ')
        



        #RXモードで処理している場合，CHUNKごとに処理が途切れるので，途中の処理の情報を保存することができるといいかもしれません
        #self.demod_val, self.demod_countを活用してください
        

        # print(f"self.demod_val{self.demod_val}")
        # print(f"self.demod_count{self.demod_count}")
        # print(f"self.demod_temp_data{self.demod_temp_data}")
        # print(f"return_demod_data{return_demod_data}")
        return return_demod_data

    def decode_rx(self, demod_data):
        decode_data = []
        if len(demod_data) != 0:
            print(demod_data)
            # print(demod_data[0])
            demod_data_1 = demod_data[0]
            idx = 0
            while idx < len(demod_data_1) and demod_data_1[idx] == 0:
                idx += 1

            while idx > 0 and idx < len(demod_data_1):
                if demod_data_1[idx] == 0 and demod_data_1[idx - 1] == 1:
                    decode_data.append(1)
                elif demod_data_1[idx] == 1 and demod_data_1[idx - 1] == 0:
                    decode_data.append(0)
                else:
                    break
                idx += 2

        # print(f"decode_data{decode_data}")
        return decode_data
    
    def decode_rx_ask(self, demod_data): #2022.11.2追加
        decode_data = []
        if len(demod_data) != 0:
            print("DECNUM =", len(demod_data), " data: ",demod_data) 
            demod_data_1 = demod_data
            
            #2022.11.2: 最初の1に対する処理
            idx = 0
            if len(demod_data) != int(len(self.demod)/(self.BITLENGTH / self.SEGMENT)):
                if demod_data_1[idx] == 1:
                    decode_data.append(0)
                    idx = 2
            else:
                idx = 1

            while idx > 0 and idx < len(demod_data_1):
                if demod_data_1[idx] == 0 and demod_data_1[idx - 1] == 1:
                    decode_data.append(1)
                elif demod_data_1[idx] == 1 and demod_data_1[idx - 1] == 0:
                    decode_data.append(0)
                elif demod_data_1[idx] == 1 and demod_data_1[idx - 1] == 1:
                    idx -= 1 #1ビット分だけエラーがあるとして飛ばすため(idxが最後に2足されることを考慮)
                elif demod_data_1[idx] == 0 and demod_data_1[idx - 1] == 0:
                    idx -= 1 #1ビット分だけエラーがあるとして飛ばすため(idxが最後に2足されることを考慮)
                else:
                    break
                idx += 2

        print("DECOND_DATA: ",decode_data)
        # print(f"decode_data{decode_data}")
        return decode_data

    def phy_layer_rx(self, rx_wave):
        #demodulationとマンチェースターを復号
        #demod_data1 = self.frequency_analysis_rx(rx_wave, False) #FSK復調に関わる部分は一旦コメントアウト
        if self.demod_final_flag == False: #2022.11.2 以下，追加・変更
            demod_data1 = self.amplitude_analysis_rx(rx_wave,False) #ASK復調のための関数を追加
        else:
            demod_data1 = self.demod_continue_data
    
        #demod_data2 = self.demodulation_rx(demod_data1) #2022.11.2: FSK復調に関わる部分は一旦コメントアウト
        demod_data2 = self.demodulation_rx_ask(demod_data1) #2022.11.2: ASK復調のための関数を追加
        #decode_data = self.decode_rx(demod_data2) #2022.11.2 コメントアウト
        decode_data = self.decode_rx_ask(demod_data2) #2022.11.2 マンチェスタ符号を考慮した復調処理を行うためのdecode_rx関数を追加，処理の関係上，上手くいかない場合もあります．
        if not decode_data: #2022.11.2 decode_rx_ask関数の後処理
            decode_data = np.zeros(len(self.acc1))
        return decode_data

    def mac_layer_rx(self, data_in):
        idx = 0

        #プリアンブルを探す
        while idx < len(data_in) and data_in[idx] != 1:
            idx += 1
        if idx + 1 >= len(data_in):
            #プリアンブルが見つけられなかったら空配列を返す（受信データなし）
            return [], False
        preamble = data_in[:idx + 1]
        print(f"preamble{preamble}")

        if idx + 17 > len(data_in):
            #ヘッダよりデータが短かったら受信失敗
            return [], False

        #次の4bitを宛先IDとして扱う
        receiver_id = data_in[idx + 1:idx + 5]
        #次の4bitを送信元IDとして扱う
        sender_id = data_in[idx + 5:idx + 9]
        #次の8bitをデータ長として扱う
        data_length = data_in[idx + 9:idx + 17]
        print(f"receiver id{receiver_id}")
        print(f"sender id{sender_id}")
        print(f"data_length{data_length}")
        if receiver_id != self.my_id:
            return [], False

        if idx + 17 + self.binarylist_to_decimal(data_length) > len(data_in):
            #データ長よりデータが短かったら受信失敗
            return [], False

        #データ
        payload = data_in[idx + 17:idx + 17 + self.binarylist_to_decimal(data_length)]
        print(f"payload{payload}")

        #パリティ
        parity = data_in[idx + 17 + self.binarylist_to_decimal(data_length)]
        print(f"parity{parity}")
        if parity == self.calc_parity(payload):
            print("パリティが一致しました。")
            return payload, False
        else:
            print("パリティが一致しません。")
            return payload, True


    def network_layer_rx(self, data_in):
        #１対１の通信なので何もしない。
        return data_in

    def rx(self, rx_wave):
        dec_data = self.phy_layer_rx(rx_wave)
        #if self.demod_continue_flag:
        if len(dec_data) == 0:
            return [], False
        mac_rx_out, nack_flag = self.mac_layer_rx(dec_data)
        net_rx_out = self.network_layer_rx(mac_rx_out)
        return net_rx_out, nack_flag

    def network_layer_tx(self, data_in):
        #１対１の通信なので何もしない。
        return data_in

    def mac_layer_tx(self, data_in):
        mac_tx_out = []

        preamble = [0, 0, 0, 1]
        #プリアンブルを連結
        mac_tx_out += preamble

        #宛先IDと送信元IDを連結
        mac_tx_out += self.receiver_id
        mac_tx_out += self.my_id

        #ペイロードサイズ（8bit）を連結
        mac_tx_out += self.decimal_to_binarylist(len(data_in), 8)
        #ペイロード（データ）を連結
        mac_tx_out += data_in
        #ペイロードのパリティを計算、連結
        mac_tx_out += [self.calc_parity(data_in)]

        return mac_tx_out

    def phy_layer_tx(self, data_in):
        tx_wave = np.empty(0)
        bit0_data = np.sin(np.arange(self.BITLENGTH) * (float(self.FREQUENCY0) * (math.pi * 2)) / self.SAMPLING_RATE)
        bit1_data = np.sin(np.arange(self.BITLENGTH) * (float(self.FREQUENCY1) * (math.pi * 2)) / self.SAMPLING_RATE)
        
        #ASK（On-Off Keying）の場合，bit0_dataが0，bit1_dataが1になるようにする
        #そこでこの下に，bit0_dataの信号をOFFにするためのコードを追加する
        #bit0_dataもしくはbit1_dataと同じ長さの"0"信号をいれた"bit0_data"を準備する
        #(ASK変調の場合，周波数を揃えることでbit0_dataとbit1_dataは同じ長さになります)

        #Pyaudioの再生が最後途切れる場合があるので、ダミーデータをくっつける
        data_in += [0, 0, 0, 0, 0]
        print(data_in)

        for bit in data_in:
            if bit == 0:
                tx_wave = np.hstack((tx_wave, bit0_data))
                if self.IS_MANCHESTER:
                    tx_wave = np.hstack((tx_wave, bit1_data))
            elif bit == 1:
                tx_wave = np.hstack((tx_wave, bit1_data))
                if self.IS_MANCHESTER:
                    tx_wave = np.hstack((tx_wave, bit0_data))
            else:
                print(f"tx_data error: {bit} in {data_in}")
        return tx_wave

    def tx(self, tx_data):
        net_tx_out = self.network_layer_tx(tx_data)
        mac_tx_out = self.mac_layer_tx(net_tx_out)
        tx_wave = self.phy_layer_tx(mac_tx_out)

        return tx_wave

    def tx_acknack(self, tx_data):
        tx_wave = self.phy_layer_tx([0, 0, 0, 1] + tx_data)
        return tx_wave

    def mac_layer_rx_acknack(self, data_in):
        idx = 0

        #プリアンブルを探す
        while idx < len(data_in) and data_in[idx] != 1:
            idx += 1
        if idx + 1 >= len(data_in):
            #プリアンブルが見つけられなかったら空配列を返す（受信データなし）
            return [], False
        preamble = data_in[:idx + 1]
        print(f"preamble{preamble}")

        #ここにACK/NACKの判定処理を書きます。
        #ヒント：data_in[idx + 1]とdata_in[idx + 2]にACKかNACKが入ってます。
        #返り値の仕様を以下のように定義します。
        # ・ACKを受けた場合はdata_in[idx + 1:idx + 2]とFalseを返す
        # ・NACKを受けた場合はdata_in[idx + 1:idx + 2]とTrueを返す
        # ・それ以外の場合は空配列とFalseを返す（300行目と同じ）

    def rx_acknack(self, rx_wave):
        dec_data = self.phy_layer_rx(rx_wave)
        if len(dec_data) == 0:
            return [], False

        acknack, nack_flag = self.mac_layer_rx_acknack(dec_data)
        return acknack, nack_flag
