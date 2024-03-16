# MEAM5100-Final

## Requirement

[Adafruit_VL53L0X](https://www.arduino.cc/reference/en/libraries/adafruit_vl53l0x/)

[ESP32Servo by K.H.](https://www.arduino.cc/reference/en/libraries/esp32servo/)
[文档](https://madhephaestus.github.io/ESP32Servo/annotated.html)

## 代码逻辑

我会用另一块 ESP32 C3（暂且叫它 M 板）来给这块板（暂且叫它 C 板）发信息。

我希望给他发 WASD 前后左右 QE 方向微调 P 停车 的时候他直接传达给下层板，也就是手动控制模式。
然后会有三个模式，分别是 1 2 3。分别对应 wall following, move police, navigate to trophy。

wall following

如果正面 tof 小于某个值，a + delay

如果侧面前 tof 大于某个值，e

如果侧面前 tof 小于某个值，q

如果侧面前 tof 数据都在某个范围内（无论侧面后 tof 是怎么样的） w

move police

得到自己的位置，和朝向
得到 police 位置
转弯，w

走到 police 面前
转弯，w
