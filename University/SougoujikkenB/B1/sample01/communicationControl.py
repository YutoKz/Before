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
        self.SEGMENT = int(SAMPLING_RATE / 500)

        self.state = "SLEEP" #通信状態制御用
        self.tx_data = []
        self.freq0_threshold = 0
        self.freq1_threshold = 0
        self.rem = np.empty(0)

    def rx_test(self, rx_wave):
        rx_wave = np.hstack((self.rem, rx_wave))
        num_segment = int(len(rx_wave) / self.SEGMENT)
        acc0 = []
        acc1 = []
        accS = []
        dec_data = []
        for seg in range(num_segment):
            acc0temp = 0
            acc1temp = 0
            accStemp = 0
            start_idx = seg * self.SEGMENT
            end_idx0 = seg * self.SEGMENT + self.BIT0SHIFT
            end_idx1 = seg * self.SEGMENT + self.BIT1SHIFT
            end_idxS = int(seg * self.SEGMENT + self.BIT1SHIFT / 1.5)
            if end_idx0 + self.SEGMENT <= len(rx_wave) and end_idx1 + self.SEGMENT <= len(rx_wave):
                for idx in range(self.SEGMENT):
                    acc0temp += rx_wave[start_idx + idx] * rx_wave[end_idx0 + idx]
                    acc1temp += rx_wave[start_idx + idx] * rx_wave[end_idx1 + idx]
                    accStemp += rx_wave[start_idx + idx] * rx_wave[end_idxS + idx]
            else:
                #self.rem = rx_wave[start_idx:]
                break
            acc0.append(acc0temp)
            acc1.append(acc1temp)
            if acc0temp > self.freq0_threshold or acc1temp > self.freq1_threshold:
                if acc0temp > acc1temp:
                    dec_data.append(0)
                else:
                    dec_data.append(1)
            else:
                dec_data.append(-1)

        # print(acc0)
        # print(acc1)
        #print(dec_data)

    def calibration(self, init_frame):
        num_segment = int(len(init_frame) / self.SEGMENT)
        acc0 = np.empty(num_segment)
        acc1 = np.empty(num_segment)
        dec_data = np.empty(num_segment)
        for seg in range(num_segment):
            acc0temp = 0
            acc1temp = 0
            start_idx = seg * self.SEGMENT
            end_idx0 = seg * self.SEGMENT + self.BIT0SHIFT
            end_idx1 = seg * self.SEGMENT + self.BIT1SHIFT
            for idx in range(self.SEGMENT):
                if end_idx0 + idx < len(init_frame) and end_idx1 + idx < len(init_frame):
                    acc0temp += init_frame[start_idx + idx] * init_frame[end_idx0 + idx]
                    acc1temp += init_frame[start_idx + idx] * init_frame[end_idx1 + idx]
                acc0[seg] = acc0temp
                acc1[seg] = acc1temp
                if acc0temp > self.freq0_threshold or acc1temp > self.freq1_threshold:
                    if acc0temp > acc1temp:
                        dec_data[seg] = 0
                    else:
                        dec_data[seg] = 1
                else:
                    dec_data[seg] = -1

            if self.freq0_threshold < acc0[seg]:
                self.freq0_threshold = acc0[seg]
            if self.freq1_threshold < acc1[seg]:
                self.freq1_threshold = acc1[seg]

        print(acc0)
        print(acc1)
        print(dec_data)
        print(f"th0:{self.freq0_threshold}, th1{self.freq1_threshold}")

    def reset(self):
        self.tx_data = []
        self.freq0_threshold = 0
        self.freq1_threshold = 0
        print(f"th0:{self.freq0_threshold}, th1{self.freq1_threshold}")

    def tx_test(self):
        tx_wave = np.empty(0)
        bitlength = int(self.SAMPLING_RATE / self.BPS)
        bit0_data = np.sin(np.arange(bitlength) * (float(self.FREQUENCY0) * (math.pi * 2)) / self.SAMPLING_RATE)
        bit1_data = np.sin(np.arange(bitlength) * (float(self.FREQUENCY1) * (math.pi * 2)) / self.SAMPLING_RATE)
        stopbit_data = np.zeros(int(bitlength * 0.1))

        for bit in self.tx_data:
            if bit == 0:
                tx_wave = np.hstack((tx_wave, bit0_data))
                if self.IS_MANCHESTER:
                    tx_wave = np.hstack((tx_wave, bit1_data))
            elif bit == 1:
                tx_wave = np.hstack((tx_wave, bit1_data))
                if self.IS_MANCHESTER:
                    tx_wave = np.hstack((tx_wave, bit0_data))
            else:
                print(f"tx_data error: {bit} in {self.tx_data}")

            tx_wave = np.hstack((tx_wave, stopbit_data))
        return tx_wave