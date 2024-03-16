# Grant Theft Autonomous 2023 
 This repository contains the source code for the annual competition of MEAM 510.

# TEAM 16, WE ARE THE CHAMPION !!!

# Video Link

- **Evaluation of functionality:** [**https://youtu.be/02jUuSZZPl0**](https://youtu.be/02jUuSZZPl0)

- **Final competition:** [**https://youtu.be/HylkRfzkrz0**](https://youtu.be/HylkRfzkrz0)

- **Compete Competition recording** at [GRASP Lab Youtube Channel.](https://www.youtube.com/watch?v=Pt5Qd4mry5I&t=9516s)

![img](https://lh7-us.googleusercontent.com/kso3s3ZOTSkDRWEmCr_BuxCN1hRN8cCMBc-IBTjwY6mCad-0vZBEvTp-5VF1_J__OXvqRH4qTnFc4vNTRj7JempvYU3lOWIPEN8whSWlSbLr5aXfciFHOGMTb-zxrdi5ayFF7ny3VXRjsk00saTfRgI)

---

![img](https://lh7-us.googleusercontent.com/HOdCZCFWYqKb0XaJinlHCBFVFbVOcvReB564sv2R9l9IfYjKA8drwFnG_IjlqWxzr6NQBvUwqJvdb9ZUSM57624-r5rI657nqwVMuL3xqfa_uUFWiF5zOuhx50J3H4FApMXlDsVvaBm4YE2gMocg3Gg)

---

# Functionality

- Autonomous Wall Following
- Autonomous Beacon Tracking, Grasping Trophy
- Autonomous Navigate and Push the Police car
- Real-time positioning of XY location, and broadcasting via UDP
- Manual Wireless Control through Key-board

# Electrical Design

- ## Vive Localization System

  A single Vive chip can localize the position of robot, while two combined can determine the direction of it.

  **![img](https://lh7-us.googleusercontent.com/GCQXHQMqaWdwma0M3J9rL1E3ntH8_DoejpKTzCkDAwusTMTn06QFv0-0_vUYmwmtgMhuLvFWJmUC1P2LRf0vYcnqCPi3juf3SeCpIV6kUI_RZcKRVWFaltnMYk8PWkm2cBvDOajXircFdfuCtbV6hko)**

<img src="https://lh7-us.googleusercontent.com/HNoPdhf0tED2sQ9hLCtby5nxjhDtJ0p4_HDndf0pcVJFHQcIkjlffN85a8HN49lGS5jzGWrn380xpBFiG_l51WDBNBRYsYddF7MuXyqSdlSSkzRh0i4p1Re0BiX7jJBbbhm_J4WEHYSB1MlD8z2z60s" alt="图片" style="max-width: 50%;"><img src="https://lh7-us.googleusercontent.com/AvaXA8mvyG3OUU0c0oTNV2-RUQMBU4_vb9pYd_pxpVcxqWyi-4SHb4LWfNn-EcCYnXez-SC_rG1hUtvRzVa6u1YVYWwqjOfnEKvqNJ_NgyTQhyH9nGyzvGZtpt0bxw_7d8IsLdH8WoaVK8zhJ8KCSYI" alt="图片" style="max-width: 45%;">

- ## TOF Distance Measurement System

  Three TOF sensors are used in total, with two placed on the sides of the vehicle and one placed at the front.

  **![img](https://lh7-us.googleusercontent.com/bRdQojjujxUWxMtkc1oBa_sHLqU_jmFU0eBiUF2j37RioWwBdpjtHIXteggRWmvrHvIwhMKa4PSuBgzsXBEcU7XLhnvYGsKKBoMQujUMEV_o307sZXOxzgqTNtihTlMCS6pO5RD5spb5w7-VupqOghw)**

  ![img](https://lh7-us.googleusercontent.com/rSXZo5mOZFK947bbMrHLC7OB383NKWiPRzR4KF6eDs1xYL8XatKZVoS7aLdJRUa3q_9qE8qVww3Nr2BCOjILolUX_LMdS60PdxFv92d6yB84nA3QZuIuM7SfeJ98OzvwK2yoCr6zyyyBC4oT9kCrwkU)

- ## IR Beacon Tracking System

Two infrared sensors are placed at the front of the vehicle, each isolated with shading material, to increase detection range and enhance detection accuracy.

**![img](https://lh7-us.googleusercontent.com/UWi6hNN89ws_2Ebx77jjpW6lGpXnzHMO4iSvFhu3Ec8BLf9fGYS4xRUuwMxfDL_XP_LX68aF6HH-s9z9tnpqfq1-NVUG8TsXUNz0-xbZiNDVuVYUx7svXBbc4m3oANpbGlrw_MjxGMDhSa-nIu_3zSc)**

<img src="https://lh7-us.googleusercontent.com/-IRea0SRN-4OMeCsb7sF3BR_AD01xX0Jv5kluX6MEEpUsweCeEQFhq52AhDED20aA0R70HMytsL4e6WhFlJMPcg4fIFkqLyRABE6Fv-dVbYjtMhuP2LjPOo25MCQ4kUWaDPBj0ISpcPbY5q1Yn5ezAU" alt="图片" style="max-width: 50%;"><img src="https://lh7-us.googleusercontent.com/YF573x2CL2SsN18P0R9q0n694I281nW9AEz1FMdR4Eq_cxf-Yhhjzp6527zY7J8dTjBCQYK6SdxeTolPQojqqSlEmJ9ti-fZGAX9hY2bjyuhg_iKBiVR4rzkudDVRzzxIpz88wTSOYmk2PpVoVt0Eas" alt="图片" style="max-width: 50%;">

- ## Motor Driver System

  ![img](https://lh7-us.googleusercontent.com/fYOTLufaV9hiXZkEkfteKAV4ZalKRtuxll9NiKaISJIWeAfxmOMg2RpDCOJ8kHBGG7yZ6QuYQ-RAlCtxNovPzoWZJmkCjDWGPrPXOlL3YMDmDHcFPqAr3JBgc-sfNYepWe8Z_ypjL-WM01xRvHmJGWU)

  ![img](https://lh7-us.googleusercontent.com/cv772BqjwLXmzzku0Z58KhfikiRn-FP1FZc4NV86Cw4yBsn-B6Pz_NfACN5roASnHE4hVMccJRqszQms3rOFzYWUeKK4K1FSsfmfc4ibbwVw3gsEalIp4RjaE0konBgmqqX_3DDYm0C0kBulops4Oo4)

# BOM

| **Item**         | **Quantities** | **Price** |
| ---------------- | -------------- | --------- |
| DC Motors        | 2              | $68.0     |
| Wheels           | 2              | $7.50     |
| ToF sensors      | 6              | $17.0     |
| Claw             | 1              | $18.0     |
| Lipo Battery     | 1              | $17.5     |
| Wheel connectors | 2              | $3.75     |

# Reference Datasheet

- **Motor(CQRobot Ocean):** [**http://www.cqrobot.wiki/index.php/DC_Gearmotor_SKU:_CQR37D**](http://www.cqrobot.wiki/index.php/DC_Gearmotor_SKU:_CQR37D)
- **Motor Driver(L298N):**[**L298N-Motor-Driver-Datasheet.pdf (components101.com)**](https://components101.com/sites/default/files/component_datasheet/L298N-Motor-Driver-Datasheet.pdf)
- **TOF(VL53L0X):**[**Time-of-Flight ranging sensor (st.com)**](https://www.st.com/resource/en/datasheet/vl53l0x.pdf)
- **Gripper:** [**Amazon.com: Mechanical Claw BigClaw Robot Gripper (without Servo) : Toys & Games**](https://www.amazon.com/dp/B08YDH27YK?ref=ppx_yo2ov_dt_b_product_details&th=1)

- **Battery(4s Lipo):**[**Amazon.com: HRB 4S 3300mAh Lipo Battery 60C 14.8V RC Lipo Battery Pack with XT60 Plug for RC Airplane, RC Helicopter, RC Car, RC Truck, RC Boat (EC3/Deans/TR/Tamiya) : Toys & Games**](https://www.amazon.com/dp/B06XKWFPRZ?psc=1&ref=ppx_yo2ov_dt_b_product_details)

