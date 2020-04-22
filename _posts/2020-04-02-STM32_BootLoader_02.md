---
layout: post
title:  "[Bootloader] STM32 Boot Configuration"
date:   2020-04-02
categories: ST Bootloader
---

이 포스팅은 `ST 보드의 Bootloader와 관련한` 포스팅임을 알려드립니다.

---
실제 Keil에서 ...

---
### Boot Configuration

ST에서 테크닉적으로 동작하는 메모리 aliasing을 볼 수 있었다. 따라서 Boot Configuration이 정해지면, 리셋 핸들러는 해당 메모리 번지로 aliasing 되어 Jump하게 된다. STM32F446RE에 대한 Boot Configuration은 다음과 같다. 다시 말해 서로 다른 번지의 메모리지만 0x0000_0000번지가 해당 메모리로 alias 된다는 것이다.

![01](https://drive.google.com/uc?id=1xHzn7jZBFokzQ3emTfSZ390Mv1lTAOBE)

하지만 TI 보드의 Memory Map을 확인하면 Flash의 번지가 리셋 주소인 0x0000_0000과 같게 된다. 따라서 TI는 Flash가 Boot인 경우에는 alias가 필요없다는 것을 알 수 있었다. 그래서 반드시 RM 문서를 참고해야만 알 수 있는 것이다

![02](https://drive.google.com/uc?id=1SMS8LACF0dgezSkpCGfnof-iCFX1FX0p)