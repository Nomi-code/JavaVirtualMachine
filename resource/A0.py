import sounddevice as sd
import wave
import numpy as np
from core import execute
import subprocess


# 录音设置
duration = 20  # 录音时长（秒）
sample_rate = 44100  # 采样率（标准为44100 Hz）
channels = 2  # 立体声（2声道）

# 录音函数
def record_audio(filename, duration, sample_rate, channels):
    # 使用 sounddevice 录制音频
    recording = sd.rec(int(duration * sample_rate), samplerate=sample_rate, channels=channels, dtype=np.int16)
    # 等待录音完成
    sd.wait()

    # 将录音保存为 wav 文件
    with wave.open(filename, 'wb') as wf:
        # 设置声道数
        wf.setnchannels(channels)
        # 采样宽度（2字节，16位）
        wf.setsampwidth(2)
        # 设置采样率
        wf.setframerate(sample_rate)
        # 写入录音数据
        wf.writeframes(recording.tobytes())

    print(f"Audio saved as {filename}")

def parse_result(result):
    count = 0
    for key in result.keys():
        count = count + int(result[key])
    print(count)
    if count <= 2:
        # 不疲劳
        print("不疲劳")
        subprocess.run(["gpioset", "gpiochip4", "2=1"])
    elif count <= 3:
        #疲劳
        print("轻度疲劳")
        subprocess.run(["gpioset", "gpiochip4", "3=1"])
    else:
        # 重度疲劳
        print("重度疲劳")
        subprocess.run(["gpioset", "gpiochip4", "4=1"])
        


if __name__ == '__main__':
    # 调用函数开始录音并保存
    subprocess.run(["gpioset", "gpiochip4", "2=0"])
    subprocess.run(["gpioset", "gpiochip4", "3=0"])
    subprocess.run(["gpioset", "gpiochip4", "4=0"])

    filename = "output.wav"
    record_audio(filename, duration, sample_rate, channels)
    result = execute(model_path='models.pkl', wav_path='output.wav')
    print(result)
    parse_result(result)