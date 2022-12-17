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

        self.state = "SLEEP" #通信状態制御用
        self.tx_data = []
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
        while idx < len(data_in) and data_in[idx] != 1:
            idx += 1
        if idx + 1 >= len(data_in):
            return []
        # print(f"preamble{data_in[:idx + 1]}")
        # print(f"data{data_in[idx + 1:]}")
        return data_in[idx + 1:]

    def network_layer_rx(self, data_in):
        #１対１の通信なので何もしない。
        return data_in

    def application_layer_rx(self, data_in):
        if len(data_in) > 8:
            print(data_in)
            width = 0
            height = 0
            for i in range(4):
                width += (data_in[i] << (3 - i))
                height += (data_in[i + 4] << (3 - i))
            print(f"width{width}, height{height}")
            for i in range(height):
                for j in range(width):
                    if i * width + j + 8 < len(data_in):
                        if data_in[i * width + j + 8] == 0:
                            print("□", end='')
                        else:
                            print("■", end='')
                    else:
                        print("*", end='')
                print(" ")

            return True
        else:
            return False

    def rx(self, rx_wave):
        dec_data = self.phy_layer_rx(rx_wave)
        #if self.demod_continue_flag:
        if len(dec_data) == 0:
            return False
        mac_rx_out = self.mac_layer_rx(dec_data)
        net_rx_out = self.network_layer_rx(mac_rx_out)
        app_rx_out = self.application_layer_rx(net_rx_out)

        return app_rx_out

    def calibration(self, init_frame):
        self.frequency_analysis_rx(init_frame, True)
        print(f"th0:{self.freq0_threshold}, th1:{self.freq1_threshold}")

    def reset(self):
        self.tx_data = []
        self.freq0_threshold = 0
        self.freq1_threshold = 0
        print(f"th0:{self.freq0_threshold}, th1:{self.freq1_threshold}")

    def tx_test(self):
        tx_wave = np.empty(0)
        bit0_data = np.sin(np.arange(self.BITLENGTH) * (float(self.FREQUENCY0) * (math.pi * 2)) / self.SAMPLING_RATE)
        bit1_data = np.sin(np.arange(self.BITLENGTH) * (float(self.FREQUENCY1) * (math.pi * 2)) / self.SAMPLING_RATE)
        stopbit_data = np.zeros(int(self.BITLENGTH * 0.1))

        tx_data_mac = np.hstack((np.array([0, 0, 0, 1]), self.tx_data, np.array([0, 0, 0, 0])))
        for bit in tx_data_mac:
            if bit == 0:
                tx_wave = np.hstack((tx_wave, bit0_data))
                if self.IS_MANCHESTER:
                    tx_wave = np.hstack((tx_wave, bit1_data))
            elif bit == 1:
                tx_wave = np.hstack((tx_wave, bit1_data))
                if self.IS_MANCHESTER:
                    tx_wave = np.hstack((tx_wave, bit0_data))
            else:
                print(f"tx_data error: {bit} in {tx_data_mac}")

        tx_wave = np.hstack((tx_wave, stopbit_data))
        return tx_wave