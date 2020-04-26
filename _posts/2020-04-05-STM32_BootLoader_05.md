---
layout: post
title:  "[Bootloader] Custom BootLoader 개요"
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