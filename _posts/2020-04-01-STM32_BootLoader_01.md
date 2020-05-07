---
layout: post
title:  "[Bootloader] 1. Bootloader 개요"
date:   2020-04-01
categories: ST Bootloader
---

이 포스팅은 `ST 보드의 Bootloader와 관련한` 포스팅임을 알려드립니다.

---
### 부트도더란?

부트로더는 작은 코드 덩어리로 구성되어 있다. MCU ROM 혹은 FLASH에 저장되어 있다. 부트로더의 주 역할은 `Application Loader`라고 말할 수 있다. 이를 포함해서 요구될 때마다 Application을 업데이트하는 메커니즘도 가지고 있다

아두이노 우노 보드를 예를 들도록 하자

1. 보드는 부트로더를 가지고 있나?

    그렇다. Chip 안에 부트로더가 내장되어 있다

2. MCU가 reset 때마다 동작을 하는가?

    그렇다. 리셋 후에 바로 부트로더가 동작을 한다

3. 부트로더의 주 역할은?

    주로 아두이노 펌웨어(혹은 Sketch)를 다운로드 하는데 쓰인다

---
다른 보드인 Nucleo 64를 예를 들어보자

1. 보드는 부트로더를 가지고 있나?

    그렇다. Chip 안에 부트로더가 내장되어 있다

2. MCU가 reset 때마다 동작을 하는가?

    __아니다. Boot Pin의 상태를 바꿔야만 Boot 코드가 동작한다__

3. 부트로더의 주 역할은?

    주로 바이너리를 다운로드, 업데이트 때 사용된다

__이 보드는 Bootloader가 매번 필요가 없는 이유는 In-System-Programming(In -Circuit-Debugger)으로 ST-Link를 제공하기 때문이다. 이는 해당 보드에 국한된 내용이 아닌 TM4C123G인 TI 보드에도 적용되는 내용이다.__ 따라서 ISP가 없는 보드들은 BootLoader를 통해 어플리케이션을 다운로드해야만 하는 것이다

---
### Memory Organization

부트로더를 작성하기 위해서는 MCU의 Memory Map을 꿰고 있어야만 한다. 따라서 이번 포스팅에서 사용할 ST의 메모리 맵을 한 번 짚고 가보려고 한다. STM32F446RE에 대한 Reference Manual을 열어보도록 한다. Memory Map 그림을 보면 Peripheral을 제외한 메모리는 다음과 같다

![01](https://drive.google.com/uc?id=1wnczFNYfgH2EzjAyQSShbRn3q3kP58Jl)

주 메모리들을 정리하면, 

1. Flash Memory(512KB)

    쉽게 생각해 main.c 코드가 들어있다. 어플리케이션이 동작하기에 필요한 코드와 데이터는 여기에 담겨져 있다고 생각하면 된다. 보통 ST의 Internal Flash는 0x0800_0000에서 0x0807_FFFF까지다. 그리고 Flash이기 때문에 비활성메모리다

2. SRAM1(112KB)

    0x2000_0000에서 0x2001_BFFF까지 영역을 말한다. RAM 메모리로 읽기, 쓰기가 가능하다. 따라서 Stack, Heap 영역이 주 목적으로 사용된다.

3. SRAM2(16KB)

    0x2001_C000에서 0x2001_FFFF까지 영역을 말하며 크기만 작을 뿐 SRAM1과 목적은 거의 비슷하다. 

4. System Memory(ROM, 30KB)

    __ST 부트로더가 저장된 공간이다. default로 MCU는 이 영역을 실행하지 않고 Configuration으로 이 영역으로 Jump할 수 있다면 실행이 된다. 왜냐하면 기본적으로 ISP가 내장되어 있기 때문이다.__

5. OTP memory

    One Time Programming의 약자로, 제품이 생산될 때 이미 한 번 프로그램이 된 메모리다. 제품에 대한 설명같은 부분이 담겨져 있다


RAM을 제외한 메모리 영역을 다시 보면 다음 사진으로 표현할 수 있다. Flash Memory는 총 6가지 섹션으로 나눠지고, Default로 IDE는 0x0800 번지부터 코드를 Writing 하게 된다

![02](https://drive.google.com/uc?id=1ndCqbdjWC32sL0ECFtfRjNA9poawZktc)