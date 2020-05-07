---
layout: post
title:  "[Bootloader] 2. STM32 Boot Configuration 살펴보기"
date:   2020-04-02
categories: ST Bootloader
---

이 포스팅은 `ST 보드의 Bootloader와 관련한` 포스팅임을 알려드립니다.

---
### Memory View

MCU가 리셋이 되면, 프로세서의 PC는 0번지를 가리키게 된다. 그리고 0번지에 위치한 값을 처음에 읽게 된다. 이때 MSP(Main Stack Pointer)는 0번지 값이 되게 된다. 그리고 다음 4번지는 리셋 핸들러의 주소를 가리킨다. 그래서 그 주소로 Jump를 하게 된다.

그런데 이상한 점은 ST는 bootloader가 기본적으로 flash로 되어있는데, 이는 0x0800_0000이다. 하지만 실제 코어는 0x0000_0000에서 시작하게 된다. 이는 Aliasing으로 가능한 얘기가 된다


![03](https://drive.google.com/uc?id=1MwI7K2_1Do_YL5It7R3GP2VlLufhV_vD)


![04](https://drive.google.com/uc?id=1LGKGV8VI5Nv57CAEmU-R5gSi1ZeSgGrq)


실제로 0번지를 찍어보면 똑같게 나온다


![05](https://drive.google.com/uc?id=1IjeAOhCNvc3uvfJuYcuo8l8UHa1tGOSP)


__나머지 NMI Handler, Main 함수 같은 경우는 Disassembly 창에서 확인할 수 있다.__

![06](https://drive.google.com/uc?id=1myDvNslLlUYnLz0-CqzGFW5HGgYVUkoE)

---
### Boot Configuration

ST에서 테크닉적으로 동작하는 메모리 aliasing을 볼 수 있었다. 따라서 Boot Configuration이 정해지면, 리셋 핸들러는 해당 메모리 번지로 aliasing 되어 Jump하게 된다. STM32F446RE에 대한 Boot Configuration은 다음과 같다. 다시 말해 서로 다른 번지의 메모리지만 0x0000_0000번지가 해당 메모리로 alias 된다는 것이다.

![01](https://drive.google.com/uc?id=1xHzn7jZBFokzQ3emTfSZ390Mv1lTAOBE)

하지만 TI 보드의 Memory Map을 확인하면 Flash의 번지가 리셋 주소인 0x0000_0000과 같게 된다. 따라서 TI는 Flash가 Boot인 경우에는 alias가 필요없다는 것을 알 수 있었다. 그래서 반드시 RM 문서를 참고해야만 알 수 있는 것이다

![02](https://drive.google.com/uc?id=1SMS8LACF0dgezSkpCGfnof-iCFX1FX0p)