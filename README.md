# STM32_PPM_encoder
做了一个PPM信号编码器，可以将大疆的D-BUS协议（DR16接收机专用）解析，并将6个通道的数据编码成PPM信号。主控用的STM32F103C8T6，接插件都用的STM1.25间距的接头，D-BUS和PPM采用的是焊盘的形式，如果不喜欢直接把信号线焊上去的可以加一个间距为2.54的通用接插件焊上去就行，板子资源有共阳贴片RGB灯，引出了两路I2C接口，两路串口，6路GPIO输出（可复用为PWM输出脚），晶振用的8MHz3225封装的，供电采用双路LDO供电（单路也行）。
# 能干啥
正好有一个大疆DR16接收机和遥控器，但是我用的是Pixhawk，Pixhawk没法用DR16接收机，所以做了个这个板子解析DR16的DBUS信号，再用PPM编码，这样Pixhawk就能识别了。做了很多额外接口，其实都没使用，做上去纯属没事儿干。另外板子上也画了Pixhawk的安全开关电路，直接接上飞控就能用做安全开关。
# PCB正面
![PCB_F](https://github.com/ZhiyangZhou24/STM32_PPM_encoder/blob/master/PCB_3D_Front.jpg "PCB_F")

# PCB背面
![PCB_B](https://github.com/ZhiyangZhou24/STM32_PPM_encoder/blob/master/PCB_3D_Back.jpg "PCB_B")

# 最后
> 如果有任何疑问，企鹅号778733609
