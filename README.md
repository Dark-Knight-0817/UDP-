# 弱网可靠传输机制研究

在裸 UDP 上自实现简化的可靠传输协议(序列号 / ACK-NACK / 超时重传),
并与 Fast-DDS 的可靠性机制对比,在弱网环境(tc netem)下验证 QoS 选型策略。

## 目标
- 自实现可靠 UDP,在丢包环境下保证可靠送达
- 对比 Fast-DDS RELIABLE / BEST_EFFORT 机制
- 结合 MQTT 弱网量产经验,总结控制类/传感类数据的 QoS 选型

## 进度
- [ ] Day0: 基础 UDP 收发 + 丢包基线
- [ ] Day1: 序列号 + ACK + 超时重传
- [ ] Day2: DDS 双通道对比
- [ ] Day3: 重传风暴排查 + Nav2 验证
