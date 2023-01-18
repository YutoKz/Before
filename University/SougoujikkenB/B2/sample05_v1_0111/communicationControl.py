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
        self.BITLENGTH = int(self.SAMPLING_RATE / self.BPS)
        self.DEC_TH = self.BITLENGTH / self.SEGMENT * 0.7 #ヒューリスティック
        self.demod_val = -1
        self.demod_count = 0
        self.demod_continue_flag = False

    def reset(self):
        self.freq0_threshold = 0
        self.freq1_threshold = 0
        print(f"th0:{self.freq0_threshold}, th1:{self.freq1_threshold}")

    def calibration(self, init_frame):
        self.reset()
        self.frequency_analysis_rx(init_frame, True)
        print(f"th0:{self.freq0_threshold}, th1:{self.freq1_threshold}")

    #data_in（リスト）から水平垂直パリティを計算する。
    def calc_parity(self, data_in):
        parity = []
        parity_local = 0
        #ここに水平垂直パリティを計算するコードを書く。

        num_of_VRC = math.floor((len(data_in) - 1) / 7 + 1)
        num_of_last = len(data_in) - 7 * (num_of_VRC - 1)

        # VRC
        for i in range(num_of_VRC):
            if(i != num_of_VRC - 1):
                for j in range(7):
                    parity_local += data_in[7 * i + j]
            else:
                for j in range(num_of_last):
                    parity_local += data_in[7 * i + j]

            parity_local = parity_local % 2
            #parity = np.append(parity, parity_local)
            parity.append(parity_local)
            parity_local = 0
        
        # LRC
        # まず7個
        for i in range(7):
            if(i <= num_of_last - 1):
                for j in range(num_of_VRC):
                    parity_local += data_in[7 * j + i]
            else:
                for j in range(num_of_VRC - 1):
                    parity_local += data_in[7 * j + i]

            parity_local = parity_local % 2
            #parity = np.append(parity, parity_local)
            parity.append(parity_local)
            parity_local = 0
        
        # 最後の1個
        for i in range(num_of_VRC):
            parity_local += parity[i]

        parity_local = parity_local % 2
        #parity = np.append(parity, parity_local)
        parity.append(parity_local)
        

        # VRC LRC の順に配列として返す
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

    def phy_layer_rx(self, rx_wave):
        #demodulationとマンチェースターを復号
        demod_data1 = self.frequency_analysis_rx(rx_wave, False)
        demod_data2 = self.demodulation_rx(demod_data1)
        decode_data = self.decode_rx(demod_data2)
        return decode_data

    def mac_layer_rx(self, data_in):
        idx = 0
        print(f"total length : {len(data_in)}")

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
        print(f"payload    {payload}")


        


        #パリティ
        #parity = data_in[idx + 17 + self.binarylist_to_decimal(data_length)]
        #print(f"parity{parity}")

        Vparity_flag = True
        Lparity_flag = True
        last_index = idx + 17 + self.binarylist_to_decimal(data_length) + math.floor((self.binarylist_to_decimal(data_length) - 1) / 7 + 1) + 8
        data_in_parity = data_in[idx + 17 + self.binarylist_to_decimal(data_length) : last_index]
        calculated_parity = self.calc_parity(payload)

        # 受信parityとparityの計算の２つは正しく行えているので、問題はこの関数のこれ以降
        print(f"received_parity  {data_in_parity}")
        print(f"calculated_parity{calculated_parity}")

        payload_add = []
        payload_add += payload
        for i in range(7 - self.binarylist_to_decimal(data_length) % 7):
            payload_add.append(0)

        Vparity_fail_index = []
        Lparity_fail_index = []

        num_of_VRC = math.floor((self.binarylist_to_decimal(data_length) - 1) / 7 + 1)

        print(f"num_of_VRC : {num_of_VRC}/n")

        if(len(calculated_parity) < (num_of_VRC+8) or len(data_in_parity) < (num_of_VRC+8)):
            return [], True

        for i in range(num_of_VRC):
            if(calculated_parity[i] != data_in_parity[i]):
                Vparity_fail_index.append(i)
                Vparity_flag= False


        # (コードミス修正済み != が == になっていた)
        for i in range(7):
            if(calculated_parity[num_of_VRC + i] != data_in_parity[num_of_VRC + i]):
                Lparity_fail_index.append(i)
                Lparity_flag = False


        print(f"Failed index of VRC : {Vparity_fail_index}")
        print(f"Failed index of LRC : {Lparity_fail_index}")


        # 単一間違いの場合のみ位置特定可能、すなわち訂正可能
        revisable_flag = True
        if(len(list(set(Vparity_fail_index))) == 1 and len(list(set(Lparity_fail_index))) == 1):
            print("One mistake was ditected.")
            error_v = Vparity_fail_index[0]
            error_l = Lparity_fail_index[0]
        elif(len(list(set(Vparity_fail_index))) == 0 and len(list(set(Lparity_fail_index))) == 0):
            print(("There is no mistake."))
            error_v = -1
            error_l = -1
        else:
            print("Multiple mistakes were ditected.")
            revisable_flag = False
            error_v = -1
            error_l = -1


        # 誤りは確実なもののみ表示
        # 誤りが複数ある場合、位置の特定は不可能であり、わかるのは「誤りが複数ある」ことだけ
        print('')
        print('RxData=')
        for i in range(num_of_VRC):
            for j in range(7):
                if(i == error_v and j == error_l):
                    print('*', end=" ")
                else:
                    print(payload_add[7 * i + j], end=" ")
            print("|", end=" ")
            print(int(data_in_parity[i]), end=" ")
            print('')
        print('----------------------------')
        for i in range(8):
            tmp = num_of_VRC + i
            print(int(data_in_parity[tmp]), end=" ")
            if(i==6):
                print("|", end=" ")
        print("\n")
        
        print('Vparity', end=" ")
        if(Vparity_flag):
            print('OK', end=", ")
        else:
            print('FAIL', end=", ")
        print('Lparity', end=" ")
        if(Lparity_flag):
            print('OK', end=", ")
        else:
            print('FAIL', end=", ")

        if(Vparity_flag and Lparity_flag):
            print('Correct data')
        else:
            print('Incorrect data')

        # OPTION 訂正可能ならACK, 不可能ならNACK
        if(revisable_flag):
            print("ACK\n")
            return payload, False
        else:
            print("NACK\n")
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

        if(data_in[idx+1]==1 and data_in[idx+2]==1):
            return data_in[idx+1:idx+2], False
        elif(data_in[idx+1]==0 and data_in[idx+2]==0):
            return data_in[idx+1:idx+2], True
        else:
            return [], False



            

    def rx_acknack(self, rx_wave):
        dec_data = self.phy_layer_rx(rx_wave)
        if len(dec_data) == 0:
            return [], False

        acknack, nack_flag = self.mac_layer_rx_acknack(dec_data)
        return acknack, nack_flag
