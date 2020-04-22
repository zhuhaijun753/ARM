---
layout: post
title:  "[Bootloader] 개발 환경 구성"
date:   2020-04-03
categories: ST Bootloader
---

이 포스팅은 `ST 보드의 Bootloader와 관련한` 포스팅임을 알려드립니다.

---
실제 Keil에서 ...

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