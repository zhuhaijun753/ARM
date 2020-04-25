---
layout: post
title:  "[Bootloader] ST 시스템 메모리 부트로더 실행"
date:   2020-04-04
categories: ST Bootloader
---

이 포스팅은 `ST 보드의 Bootloader와 관련한` 포스팅임을 알려드립니다.

---
### ROM Bootloader

이전의 KEIL IDE를 통해서 ST MCU가 Flash 메모리인 0x0800_0000번지에서 시작한다는 것을 알게 됐다. 그런데 ST에는 Flash 외에도 SRAM과 System Memory(ROM) 옵션도 있다. 특히 System Memory에는 내장 Bootloader가 존재하고 있다. 따라서 따로 configure 핀을 설정해야만 bootloader로 실행할 수 있게 된다.


![01](https://drive.google.com/uc?id=1It1z4jpUGEO_ele09FborIjHB7lqCl_Q)


따라서 BOOT1=0, BOOT0=1로 설정해야만 한다. 따라서 User Manual 문서롤 보면서 BOOT1, BOOT0 핀을 회로도에서 찾도록 할 것이다


![02](https://drive.google.com/uc?id=1ZTfOl8sIB21yq0x99d0g7_KAIYT955yZ)


![03](https://drive.google.com/uc?id=1nn0oWzZ0U61iMU5EtUTaFlOQ9nphJA6t)


실제 보드를 가지고 있다면, 보드를 뒤집어 보도록 한다. 뒤집으면 BOOT0는 표기가 되어있고, BOOT1은 PB2를 찾으면 된다. 각각 5V와, GND 핀과 연결하도록 한다

---
### ST Native Bootloader

ST 문서 중 Application note에는 System Memory Boot mode 설명이 자세히 나와있다. 이 부분을 참고해서 해보려고 한다. [링크](https://www.st.com/resource/en/application_note/cd00167594-stm32-microcontroller-system-memory-boot-mode-stmicroelectronics.pdf)는 다음과 같다. 현재 보드는 429ZI-DISC1이기 때문에 섹선 33으로 이동할 것이다


![04](https://drive.google.com/uc?id=15w_-7EMHj7AH5TV716qcwYX_aKR-35Qd)


그런데 429는 부트로더 버전이 2개가 있는 것을 알 수 있다. 다음 메모리를 읽어야만 실제로 버전을 알 수 있다


![06](https://drive.google.com/uc?id=1pB6-krGNw7xC3Hgy7RTA6vIaXTmK71Vy)


![05](https://drive.google.com/uc?id=1raIRleslUiMhZFA78vjUcod-12vJjEvE)


USART1 bootloader를 이용해, 부트로더와 통신할 것이다. 실제로 ST-LINK의 VCP는 USART1에 연결되어 있어, 따로 UART 전용 칩이 필요가 없다


![07](https://drive.google.com/uc?id=1YiPY3r06eJHX5On6DigUM5HnKHXv6WCd)


---
### Download STM32 FLAHSER

해당 [링크](https://www.st.com/en/development-tools/flasher-stm32.html)로 가서 STM32-FLAHSER를 다운로드 받도록 한다. 이 소프트웨어는 UART 통신을 통해서 시스템 메로리에 있는 STM32 부트로더와 통신하는 것이다. 다운로드가 완료되면 실행파일을 켜도록 한다


![08](https://drive.google.com/uc?id=1N2_4wQzjyEwraIUBgyZ15-L3To52ZgBV)


그리고 현재 VCP 포트를 장치관리자에서 확인한 후 그에 맞는 COM Port로 설정한다. 나머지 설정은 그대로 가져간다. __그리고 반드시 BOOT0와 BOOT1은 위에서 설정한대로 System Memory로 섷정이 되어야만 한다. 그렇지 않으면 다음 화면으로 넘어가지 않는다. next를 눌러 넘어간다__


![09](https://drive.google.com/uc?id=1lKZ0L3vHuBpDS276SvNIMmuGglzcCoaR)


![10](https://drive.google.com/uc?id=1s26WSqbr7fQu6M1tQXZQXNwg2XNSjRe6)


다운로드 후에 0x0800_0000번지로 넘어가도록 빨간색 박스에 대한 항목을 체크한다


![11](https://drive.google.com/uc?id=1rdvJNq8yqT074xKlyEvxw1-QCpALiAL3)


그리고 넘어가면 다운로드가 올바르게 될 것이다. 이 때 바로 user application으로 메모리가 넘어가게 된다. 다시 시스템 메모리 부팅 옵션으로 넘어가려면, 리셋 버튼을 눌러야만 한다
