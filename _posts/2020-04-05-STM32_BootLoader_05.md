---
layout: post
title:  "[Bootloader] Custom BootLoader 블록다이어그램 개요"
date:   2020-04-05
categories: ST Bootloader
---

이 포스팅은 `ST 보드의 Bootloader와 관련한` 포스팅임을 알려드립니다.

---
### Block Diagram

전에 Native BootLoader를 실행해봤다. 이제 Custom 부트로더를 만들어보려고 한다. 큰 Block Diagram은 다음과 같다


![01](https://drive.google.com/uc?id=1kM6vdEHMLCI8bn6P53VrdGp9hYEBVaqx)


그리고 BootLoader의 위치는 Flash Memory 첫 두 섹터로 잡았다. 그리고 나머지 Flash 영역은 User Application으로 잡는다. 실제로 내장 부트로더가 있는 시스템 메모리는 약 30KB로, Flash 첫 두 섹터는 합쳐서 32KB다.


![02](https://drive.google.com/uc?id=1ktNn-ePV5ADCpjv7pS2Ly1HGscwmqKan)


그리고 자세한 부트로더 커맨드는 [다음](https://github.com/niekiran/BootloaderProjectSTM32/blob/master/Docs/Bootloader%20Commands.pdf)을 참조하도록 할 것이다. 이는 부트로더 버전부터 시작해서 Flash Update에 맞춰서 하나씩 구현하면서 자세히 살펴볼 것이다


대략적으로 방향은 다음과 같을 것이다

1. MCU에서는 Reset 후 리셋 핸들러를 거쳐 메인 함수로 올 것이다. 메인에서는 Clock, UART 설정 등을 마치고 HOST(PC)에서의 명령을 기다릴 것이다.

2. 만일 Command가 오면 CRC(데이터 오류 검사) 체크를 한 이후, CRC OK가 확인되면 그리고 그에 대한 길이와 답을 줄 것이다

---
### 프로젝트 생성

1. CubeMX를 열어서 위에서 정의한 Communication과 Debug UART 포트를 configuration 하도록 한다


    ![03](https://drive.google.com/uc?id=11od0s5PmnxmWKZR7_1BtKlyMx4ZXqb-W)


    ![04](https://drive.google.com/uc?id=1x7pt8IGaCDNo1jYpLm-QsT_y74dVbNd4)


2. 그리고 Clock은 데이터시트를 참고해야 하는데, 현재는 UART 통신을 사용하기 때문에 HSI를 사용해서 60KHz 시스템 클럭을 만들어야만 한다. 


    ![06](https://drive.google.com/uc?id=1TA1yYszCQQyLiGPoQck1mdfdOD9GWRNG)


    ![05](https://drive.google.com/uc?id=1b5474VKImneva6KYTGW8WdXaAkdWUksd)


3. 그리고 데이터 오류 체크를 위해 CRC 또한 Enable을 하도록 한다


    ![07](https://drive.google.com/uc?id=1o9WeMudwl2uJ_pzIRn7q0FHkKDnexTz2)


4. 그리고 KEIL IDE로 code 생성을 하도록 한다. 그리고 빌드 옵션을 설정한 후에 빌드를 하고, 디버그를 통해 무한루프까지 문제없이 run이 되는지 확인하도록 한다


    ![08](https://drive.google.com/uc?id=1S1c6xd9OMtQZzAuCoaycdhqg4TJH9Lkb)


    ![09](https://drive.google.com/uc?id=1H7ly1LfYSE5EFt1zRh0agF-YEL3cRVe-)


    ![10](https://drive.google.com/uc?id=1dDO8Uj9ihqbNa5u8bIYEZexqG4sGwiXz)


    ![11](https://drive.google.com/uc?id=1C1DK2TiWsorvAPkD4P2_m1WrB7YIrVzE)


    ![12](https://drive.google.com/uc?id=1V9O4tZ6Fca2wZSwKIkLw2ghNuTyyr8o8)

    
---
### 코드 분석

CubeMX로 만든 코드에 대해서 한 줄씩 분석하려고 한다. 초기에 while문을 들어가기 전 초기화를 진행하게 되는데, CubeMX에서 설정한 Peripheral들이 보이게 된다


![13](https://drive.google.com/uc?id=1-fdZxw-2sViBI-5rpdItbDgMiU4UhkVN)


그리고 `SystemClock_Config`에서 현재 클럭은 HSI 내부 오실레이터를 사용하며, PLL로 60Mhz까지 클럭을 올려준 것을 볼 수 있다


![14](https://drive.google.com/uc?id=1CVlHTSjPD9O9p6zpTIYPayr5wp6FBcog)


그리고 `HAL init`에서는 Flash 초기화 및 Systick 1ms 타이머를 초기화 하는 것을 볼 수 있다


![15](https://drive.google.com/uc?id=191zfGOTllqK5oYuiQHdVDrHUk3mqEdcr)


![16](https://drive.google.com/uc?id=1oux8i0a8LE3m8_MeHXaL2U2KSbG39VzA)


그리고 USART1은 ST-LINK 포트인 VCP와 연결되어 있다. 따라서 따로 컨버터를 연결할 필요가 없다


![17](https://drive.google.com/uc?id=1BcTP_IMjJT0LHipYKu35Op41X9BWDRhv)