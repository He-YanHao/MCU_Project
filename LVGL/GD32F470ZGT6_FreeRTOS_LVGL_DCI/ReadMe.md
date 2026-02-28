## 引脚分配

```

```



## 地址分配

SDRAM有16MB空间，从地址 `0xC0000000` 开始，到 `0xC2000000` 结束，一共 32MB。

| 区域          | 地址       | 大小     | 用途   |
| ------------- | ---------- | -------- | ------ |
| FreeRTOS Heap | 0xC0000000 | 512KB    | 任务栈 |
| LVGL Heap     | 0xC0080000 | 512KB    | UI对象 |
| LVGL DrawBuf  | 0xC0100000 | 512KB    | 双缓冲 |
| LCD FB0       | 0xC0180000 | 1MB      | 显示   |
| LCD FB1       | 0xC0280000 | 1MB      | 双缓冲 |
| 预留区        | 0xC0380000 | 其余全部 | 未来   |



## 数据流

```
OV7670
   ↓ (PCLK + D0~D7)
DCI
   ↓ DMA
Camera Frame Buffer (SDRAM, 双缓冲)
   ↓ (CPU / DMA memcpy / blit)
LVGL Draw Buffer (SDRAM, 双缓冲)
   ↓ SPI DMA
SPI LCD
```

