---
layout: post
title:  "bare_metal for ARM Target"
date:   2020-05-01
categories: ARM bare_metal
---

이 포스팅은 `ARM bare metal`과 관련한 포스팅임을 알려드립니다.

---
### Bare metal

임베디드 시스템에서 베어메탈은 운영체제 환경이 없는 하드웨어 환경을 말한다. 보통 이런 환경을 기본적인 펌웨어 환경이라고 일컫는다. 다시 말해 하나의 while 루프에서 실행되는 펌웨어를 작성하게 된다. 일반적인 베어메탈 프로그램에는 최소 부트로더가 있고, 이는 응용 프로그램으로 넘어가게 된다




---
### ToolChain

크로스 컴파일을 위해서 툴체인을 설치하도록 할 것이다. 툴체인은 다음과 같다

1. 크로스 컴파일을 위한 컴파일, 어셈블, 링크까지를 가능하게 해주는 binary의 집합체이다
2. 또한 타겟에 대한 디버깅을 위한 바이너리도 포함하고 있다
3. 또한 실행파일을 분석하는 도구도 제공해주고 있다. 실행파일에 대한 정보 뿐만 아니라 포멧 변환, C 라이브러리까지 제공하고 있다

유명한 툴체인은 총 2가지가 있다

* GNU Tools(GCC) for ARM Embedded Processors (Free Open Source)
* armcc from ARM Ltd (Keil, Not Free)

무료 버전인 GCC를 사용할 것이다

---

1. 툴체인을 설치하기 전 pc에 이미 설치되어 있는지 확인하도록 한다. 윈도우 명령 프롬포트에 `arm-none-eabi-gcc`를 검색해서 PATH에 있는지 확인한다


    ![01](https://drive.google.com/uc?id=1Q4-I0z-DPD2rPWxFlY8ZgSq7u1LOOSCA)


2. 없다면 따로 설치를 해야한다. 사이트는 구글에 arm-gcc를 검색해서 다음 [링크](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)로 진입하도록 한다. 그리고 다운로드 링크로 가도록 한다


    ![02](https://drive.google.com/uc?id=1_frO8NmaSUTzHO_-WyGYVFYxuvD1R3ZT)


    그리고 윈도우 OS 버전의 가장 최신 툴체인을 다운로드 받도록 한다


    ![03](https://drive.google.com/uc?id=1Ul5nsml1OSD396mRMVPBdlHi7W0TiPKl)


3. 설치를 완료하고 PATH를 지정하면, 다음 커맨드 창에서 다시 입력하면 PATH에 지정된 것을 확인할 수 있다


    ![05](https://drive.google.com/uc?id=1Qkto_B4dzThb_V1xeBs0rylkMWZEBmDO)

    그리고 설치 폴더로 가면 위에 언급한 컴파일러, 어셈블러, 링커를 포함한 여러 유용한 바이너리 도구인 gdb, objdump도 확인할 수 있다

    ![04](https://drive.google.com/uc?id=1glB9BfX5F_f92R3BwYDHG_zMWwAoTn6s)


4. 다음 사진 출처 링크는 [다음](https://www.youtube.com/watch?v=qWqlkCLmZoE)과 같다. 이는 컴파일을 통해서 마지막 elf, bin 파일이 나오는 상세한 과정이다

    실제로 우리는 일반적인 컴파일에서는 .i 파일과, .s 파일을 볼 수 없다

    ![06](https://drive.google.com/uc?id=1htwYQkHBoI2osa9y0QUIAnSVp2hRHCAe)


    ![07](https://drive.google.com/uc?id=1DegfXl2S_83VOEcVfpqU9YJFwyaHDT5Y)


    __그리고 이 모든 과정은 arm-none-eabi-gcc 하나의 명령으로만 수행이 된다는 점이다.__


---
### 빌드 실습

1. 위 유튜브 영상으로 제공하는 소스코드를 통해서 실습을 해보려고 한다. 아래 [링크](https://github.com/niekiran/baremetalembedded)에서 소스코드를 적절한 경로 아래에 clone 하도록 한다


    ![08](https://drive.google.com/uc?id=1B5eXaNGhDRDWFxtLo9j6RgnEMpTggLdx)


2. 그리고 설치한 툴체인을 이용해 바이너리를 생성할 것이다. __위에서 언급했듯이 arm-none-eabi-gcc는 모든 컴파일 과정을 내포하고 있다. 따라서 옵션이 없다면 바로 바이너리 파일을 생성하게 된다. 그래서 먼저 .o 파일만 생성하기 위해서는 -c 옵션을 추가해야만 한다.__


    해당 [링크](https://gcc.gnu.org/onlinedocs/gcc-9.3.0/gcc/Overall-Options.html#Overall-Options)로 가면 gcc 공식 사이트에서 매뉴얼을 확인할 수 있다. -c 옵션은 link를 전까지 수행하는 옵션이라고 명시되어 있다


    ![09](https://drive.google.com/uc?id=1j1W2rIqg4bUahilzlEAFkUUsZuWwlXsC)


    ```bash
    $ arm-none-eabi-gcc -c main.c
    ```

    하지만 컴파일이 되지 않는다. 그 이유는 에러를 읽어보면 아키텍쳐가 정확히 명명되지 않는 것이다. 이는 ARM에는 여러 아키텍쳐가 존재하고 있고, 이에 대해서 정확히 기입해야 한다


    ![10](https://drive.google.com/uc?id=1UYhOUmS6Ah8cjMkBoYSDEPLAUjqqvNRx)


    이는 -march 혹은 -mcpu 옵션으로 지정할 수 있다. 실제로 march를 바로 알기는 어렵고 자신의 MCU를 검색해야 하지만 mcpu는 cortex 계열 형태로 기입이 가능하기 때문에 쉽게 옵션을 지정할 수 있다


    ![11](https://drive.google.com/uc?id=1tF6jzWwEYaW10zKbPZ-Vl7XvNlv_MU1I)


    ![12](https://drive.google.com/uc?id=1PAt8Rodo4_e0x_vxOQH9ReiHZyE8IUru)
    

    ```bash
    $ arm-none-eabi-gcc -c -mcpu=cortex-m4 main.c
    ```


    ![13](https://drive.google.com/uc?id=1zfSDHbXG8HaEjNtGJpi2rCH_c3HK-gv2)


3. 컴파일이 잘 실행되어 .o 파일이 생성된 것 같지만, 명령어 셋도 주의깊게 봐야 한다. 왜냐하면 기본적으로 명령어 셋은 arm으로 되어 있기 때문이다. 실제 우리가 쓰는 보드의 칩은 thumb만 지원하게 된다


    ![14](https://drive.google.com/uc?id=1XA4sA098cGg0h-jN1Yfci-H3dLqB-HxD)


    ![15](https://drive.google.com/uc?id=1LhtruLJxttXudVcvaRWxlLa2Ge-i5CFy)


    ```bash
    $ arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb main.c
    ```

4. 그리고 어셈블러 실행 전까지 진행하고 싶다면 -S 옵션으로 실행하도록 한다


    ```bash
    $ arm-none-eabi-gcc -S -mcpu=cortex-m4 -mthumb main.c
    ```

    ![16](https://drive.google.com/uc?id=1ep9Gl2gzl1ospgcGJWcKNCbfNwTsi6Yu)

