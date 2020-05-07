---
layout: post
title:  "[Bootloader] 3. 부트로더 개발 환경 구성"
date:   2020-04-03
categories: ST Bootloader
---

이 포스팅은 `ST 보드의 Bootloader와 관련한` 포스팅임을 알려드립니다.

---
### KEIL-MDK5

기존의 이클립스 기반의 IDE는 모든 OS에서 사용할 수 있다는 장점이 있지만, 사용자가 시스템과 MCU를 이해하고 있어야만 확장이 쉽게 가능하다. 따라서, Window에서만 사용이 가능하지만 더 편리한 KEIL-MDK5를 사용하도록 할 것이다.


1. [KEIL 사이트](https://www2.keil.com/mdk5)에 접속하도록 한다. 사이트 설명을 보면 IDE, 컴파일러, 여러 Device 지원, ARM CMSIS, Middleware까지 모두 지원하는 것으로 보인다


    ![01](https://drive.google.com/uc?id=1RxE1nidiiPM_ZfwihuUWy2ykY3dZflCz)


2. 현재 창에서 더 밑으로 내려가면 프로그램 edition 목록들이 나열되어 있다. 32K 코드 사이즈까지만 지원하는 무료 버전을 다운로드 받도록 한다. 회원 정보를 입력하면 바로 실행파일을 다운로드할 수 있다


    ![02](https://drive.google.com/uc?id=1lX6zeV_sx6HsCSnJhopzNTbzW6ayIHBX)


3. 실행파일을 기본 설정으로 하고 다운로드를 시작한다. 설치가 모두 완료되면 다음 경로(C:\Keil_v5\UV4)로 이동하면 uVision IDE 실행파일을 찾을 수 있다. UV4을 실행하도록 한다

    
    ![03](https://drive.google.com/uc?id=1_solOiKK1gSgqsgbVAuRU-ESS-s06wC7)


4. 그리고 Pack Installer를 실행해서 현재 보드인 429보드를 검색하도록 한다. 그러면 옆에 pack들이 나열되어 있는데 MCU 개발에 있어 필요한 주변장치 드라이버, Startup 등은 DFP 모두 담겨져 있다. 따라서 다음을 ONLINE 경로를 통해서 다운로드 받도록 한다


    ![04](https://drive.google.com/uc?id=10nq0BQjL8pFF8RieqJ3IK3vALHiSRu0b)


    nucleo-f446을 검색해서도 DFP를 다운로드 받도록 한다. 이 보드의 장점은 KEIL에서 BSP까지 지원을 해준다는 것이다. 따라서 해당 라이브러리를 이용해서 쉽게 펌웨어를 작성할 수 있게 된다


    ![05](https://drive.google.com/uc?id=1372KFkpxqEF6CfCc43Q1Vp58vzk8GPuP)


    만일 보드가 지원이 되지 않는다면 디바이스 검색으로 DFP를 다운로드 받아 펌웨어를 작성할 수 있다


5. 프로젝트 생성 화면을 열면 전에 받은 Pack 기반으로 나열되게 된다. 원하는 Device를 선택한다

    
    ![06](https://drive.google.com/uc?id=14A5DSXj-eFNqEiv0WsTchy-yAxcR2W2H)


6. 그러면 아래 화면으로 넘어가게 되는데, board 설정을 알맞게 한 후 startup 코드를 반드시 추가해야만 한다. 그런데 startup을 추가하면 아래와 같이 노란색 경고문이 뜬다. 이것은 depending 문제를 말하게 된다. 따라서 이땐 왼쪽 하단에 resolve를 누르는 것이 좋다. 그러면 해결되는 것을 볼 수 있다


    ![07](https://drive.google.com/uc?id=1B8vAxu46kC3LKg3YXY4G1vAia_4gMuVz)


    ![08](https://drive.google.com/uc?id=1N6SFWCOHFHQSU7COAHl4UUJXo4IHqw93)


7. startup.s는 어셈블리 파일로, 벡터 테이블과 stack, heap 영역을 정의하는 구간이다. 그리고 startup.c는 main.c로 넘어가기 전 시스템 clock setting이 이뤄지게 된다

    
    ![09](https://drive.google.com/uc?id=1IfHBuzYfIEWLyVws0zllOWh2aRlNCT-d)


    ![10](https://drive.google.com/uc?id=19Hlchwr271fMeDaCfaZwDFhL4vxpOHtB)


8. 좀 더 자세히 보면, 실제 전원이 바로 켜지면 Reset Handler가 실행이 된다. 따라서 startup.s에서 보면 starup.c의 systemInit 후에 main function을 찾게 된다. 따라서 main.c를 생성해서 작성해야만 한다. 그래서 SourceGroup 폴더에 main.c를 작성한 후 빌드를 한다

    
    ![11](https://drive.google.com/uc?id=16WjgqGAGL_HnEPNkqpNDLvY9ruagRlo7)
    
    
    ![12](https://drive.google.com/uc?id=1TpMoWyElSoPruvYyNsK93sd0UwM5F1Ap)


    ![13](https://drive.google.com/uc?id=1ec2MyP7qQON-QBeXxekQg5VnHKatypyW)


---
### LED blink

1. 현재 테스트 보드는 429ZI-DISC1다. 먼저 보드의 User-Manual 문서를 열도록 한다. 문서 하단으로 내려가면 회로도를 볼 수 있는데, 2개의 LED를 찾도록 한다. PG13, PG14가 핀 번호로 보여진다. [링크](https://www.st.com/resource/en/user_manual/dm00093903-discovery-kit-with-stm32f429zi-mcu-stmicroelectronics.pdf)는 다음과 같다


    ![14](https://drive.google.com/uc?id=1ctxmbl3kJfN-U245GQmE3q4POiZ4K-dN)


2. 그리고 KEIL에서는 보드에 한정해 BSP Example을 제공해주고 있다. 실제로 429ZI-DISC1 또한 간단한 Example을 제공받게 된다. 현재 아래 사진은 469 보드로 되어 있다. 이는 잘못된 사진으로 반드시 429 보드로 다시 설정해야만 한다

    
    ![15](https://drive.google.com/uc?id=12CYcUdtNXGozEaUg7PSalac8x0-f72_J)


    ![16](https://drive.google.com/uc?id=1Cp_llOwZ4w8Bj_-CLNvIuv7dnpj7zFgm)


    그리고 classic과 CubeMx 선택란이 있는데 classic으로 선택한다. CubeMx를 선택하면 해당 소프트웨어로 넘어가 남은 configuration을 진행하게 된다


    ![17](https://drive.google.com/uc?id=17cAkGV0Ci81EmA5wwN6tvE9BhPo_A5sv)


    다음은 보드 이름을 정정한 사진이다


    ![19](https://drive.google.com/uc?id=1HTuwPqqUyTwe54vnWhE6WHQtmKdrwl1y)


3. 그리고 다음 BSP 소스코드를 참조해서, LED를 켜보도록 한다


    ![20](https://drive.google.com/uc?id=1uxW6_J55jfR9YjlMNdQriavX5AnBgooL)


    ![21](https://drive.google.com/uc?id=1mCs8ogPWf0LLPqq17AOyfUJRMAaCGLBm)


    ![22](https://drive.google.com/uc?id=1_WY1RAN3beOhrXBOuB-DZiADJ30RhGK2)


4. 그리고 빌드 옵션을 설정해야 하는데, 크게는 ST-LINK 디버거 설정이다


    ![23](https://drive.google.com/uc?id=1vJS1aRxHZlcKRh2C5fAIJx2pJlfy3unk)


    ![24](https://drive.google.com/uc?id=1Zu_MiNtUu90aqqhKZKfvqfZ-F5x6rqp8)


    ![25](https://drive.google.com/uc?id=1zBnydLaKwwmOBN7j3icwytHRD-rCnHaX)


---
### OpenSTM32 System-Workbench 설치

1. 위에서 설치한 KEIL은 윈도우에서만 사용이 가능한 IDE다. 따라서 다른 운영체제라면 다른 IDE를 고려해야만 한다. 그 중 하나가 `OpenSTM32 System-Workbench`로 생각할 수 있다. 이 소프트웨어는 윈도우, 리눅스, 맥 모두 지원하고 있다. 설치를 위해 위 [링크](openstm32.org/HomePage)로 접속한 이후 계정을 생성해서 등록하고, 원하는 OS용을 다운로드 받도록 한다


    ![26](https://drive.google.com/uc?id=1IfNPIC1MkceQzFOsu6J4Z5d7yVmw17pm)


2. 그리고 설치파일을 실행시켜 다운로드 받도록 한다


    ![27](https://drive.google.com/uc?id=1cKX2VEvrAGqejbIZs-ODWyvpZX6WgPb0)


3. 모든 설정은 기본적으로 진행하도록 한다. 그러면 실행파일과 드라이버 설치가 이뤄질 것이다


    ![28](https://drive.google.com/uc?id=1rWlVEF06LIu9bgrcwm4npHI8_xUqhHl9)


    ![29](https://drive.google.com/uc?id=16jT09hXsNRASOxsQDYv1uU7B3n_BaAs6)


4. 설치가 모두 완료되면, 기본 경로(C:\Ac6\SystemWorkbench)인 곳에 Eclipse Workbench 실행파일이 생성되었을 것이다. 실행시켜, 원하는 폴더를 지정한 후 실행시킨다. 처음 시작한다면 윈도우 용 ARM 툴체인을 자동적으로 설치하니까 중간에 끄지 말아야 한다


    ![30](https://drive.google.com/uc?id=1hulSF_yerwlLFzmEXUQY1jFlQZEZC8LF)


    ![31](https://drive.google.com/uc?id=1cb-tSQL3hcDgMtkzwvc8nLWsE9o3UYRb)