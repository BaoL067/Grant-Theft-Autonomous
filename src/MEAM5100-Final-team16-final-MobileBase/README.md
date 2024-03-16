# MEAM5100-Final

## 这里可以找到 ESP32 支持的库

https://www.arduinolibraries.info/architectures/esp32

## Requirement

[ESP32Encoder by K.H.](https://www.arduino.cc/reference/en/libraries/esp32encoder/)
[文档](https://madhephaestus.github.io/ESP32Encoder/classESP32Encoder.html)

[L298N by A.L.](https://www.arduino.cc/reference/en/libraries/l298n/)
我更改了这个库叫做 L298N-ESP32，添加了 `setVelocity` method 更方便接受 PID 的输出。

[PID by B.B.](https://www.arduino.cc/reference/en/libraries/pid/)

## Debug

Encoder by P.S.
https://www.arduino.cc/reference/en/libraries/encoder/
这个库的中断写法不支持 ESP32，会导致一直 reboot。
这是因为 ESP32 上的中断会在前面加上 IRAM_ATTR（意思是把中断放在 IRAM 中，而不是在较慢的闪存中），是 ESP32 里特殊的写法。
在 include 前面加上 #define ENCODER_DO_NOT_USE_INTERRUPTS 能避免使用中断（用轮询）以解决这个问题，但 document 说这样会丢失精度。

issue 里提出的解决方案：
1 RLars fork 的库  
2 用 NewEncoder 库
https://github.com/gfvalvo/NewEncoder  
3 ESP32Encoder by K.H.  
https://www.arduino.cc/reference/en/libraries/esp32encoder/  
https://madhephaestus.github.io/ESP32Encoder/classESP32Encoder.html  
4 https://github.com/igorantolic/ai-esp32-rotary-encoder

我们最终选择了第三个方案（star 多）。初步测试正常。需要使用 attachFullQuad 模式，轮子转一圈会有 64\*210 个脉冲。

[L298N by A.L.](https://www.arduino.cc/reference/en/libraries/l298n/)
因为 analogWrite，好像也存在不支持 ESP32 的问题。
用 [ESP32_AnalogWrite](https://github.com/erropix/ESP32_AnalogWrite) 或者 [AnalogWrite_ESP32](https://github.com/pablomarquez76/AnalogWrite_ESP32) 能直接将 ESP32 的 PWM API 改成了和 arduino 一样。
但如[pablomarquez76](https://github.com/pablomarquez76/AnalogWrite_ESP32/issues/6)所言，ESP32 在 2.0.11 之后的新版本已经支持 analogWrite。官方文档[在这](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/ledc.html?highlight=analogWrite#analogwrite)。resolution 为 0 到 255，在此范围内测试正常，没有试过超过这个值会怎么样。

team33 用 CPM 速度作为 PID 输入，ledc 14 bit PWM 作为 PID 输出，也就是范围是 0-55 输入，0-16382 输出，PID 参数取 400 50 3 能获得较好效果。输出是输入的 297.85 倍。
team16 用 m/s 速度作为 PID 输入，analogWrite 作为 PID 输出，也就是范围是 0-0.25 输入，0-255 输出，输出是输入的 1020 倍。那理论上取 3.42 倍的参数也能获得较好效果，即 1370 170 10。测试结果很好。
