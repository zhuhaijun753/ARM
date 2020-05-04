---
layout: post
title:  "[Bootloader] Command, Debug UART 통신 Test"
date:   2020-04-06
categories: ST Bootloader
---

이 포스팅은 `ST 보드의 Bootloader와 관련한` 포스팅임을 알려드립니다.

---
### Command UART

먼저 기본적으로 Host와 부트로더가 통신하기 위한 디버그 UART는 VCP가 지원되는 USART1을 사용할 것이다. 그리고 메세지를 출력해주는 디버깅 포트는 남은 UART 중 아무거나 선택하면 된다. 먼저 CubeMX에서 429ZI-DISC1 보드를 사용할 때는 Board Selector를 선택한 이후, 디폴트 모드를 사용하지 않는 것으로 하려고 한다

1. CubeMX를 열어, 보드 설정을 하도록 한다. 그리고 선택 후 나오는 팝업인 Default 모드에서는 `NO`를 입력하도록 한다


    ![01](https://drive.google.com/uc?id=1ol5He_pg3HyIEPq4Cb-z-ig95GvZFKlv)


2. 그리고 디버그 UART는 VCP로 선택할 것이기 때문에, USART1을 사용하도록 한다

    
    ![02](https://drive.google.com/uc?id=1K5jRbrAPFAfSzckdQFZFGfXKJKAI2CT4)


3. 클럭은 기본으로 가져가도록 하고, 프로젝트를 다음과 같이 KEIL로 선택해서 생성하도록 할 것이다


    ![03](https://drive.google.com/uc?id=1L3X7C5kUrdKb4cG8K5GqjUyH8RDSS2yU)


    그리고 필요 라이브러리만 복사하도록 하는 옵션도 선택하도록 한다


    ![04](https://drive.google.com/uc?id=1ohrJfPRnSsHBr7FMtxfDirB-19-Wry9K)


    ![05](https://drive.google.com/uc?id=14wn2CllAfPJrb37dQzFadvuNF5Atj5Fg)


4. 그리고 UART 테스트를 위해 다음과 같이 코드를 작성한다


    ![06](https://drive.google.com/uc?id=10GSg6jC2cEkqrDhCaVQ84xBK1fYXloeC)


    그리고 HAL_GetTick() 함수를 따라가보면 uwTick을 반환하는 것을 볼 수 있다. 그리고 uwTick은 SysTick_Handler()에서 1KHz마다 1씩 증가시켜주는 것을 볼 수 있다


    ![07](https://drive.google.com/uc?id=1_dojTPnO2HvX12APDAgNj8MOpVJeDhm4)


    ![08](https://drive.google.com/uc?id=1H9Zzn1sq9ezNvKiBp0KHr--sUh1hu_5j)


5. 그리고 터미널 프로그램으로 확인하도록 한다


    ![09](https://drive.google.com/uc?id=1NUdnrsZ9atCpVWh6hiOUXgZ73cK9JWOk)


---
### Debug UART

Command UART 외에 하나 더 Debug Message 용으로 하나 더 추가해야만 한다. 따라서 CubeMX에서 보드 Select로 프로젝트를 생성하면 사용할 수 있는 것이 USART1과 UART5가 남았다. 따라서 USART1은 VCP와 연결되어 있으므로 나머지 UART5를 사용할 것이다


![10](https://drive.google.com/uc?id=1XPu7gXYVT3ep6FAL6aWvRftMR9pRGU-2)


1. 그리고 코드 생성을 한 후 다음 코드를 추가한 후 UART 테스트를 하도록 한다


    ```cpp
    HAL_UART_Transmit(&huart5, (uint8_t*)("Hello World\r\n"), strlen((const char *)("Hello World\r\n")), HAL_MAX_DELAY);
    uint32_t currentTick = HAL_GetTick();
    while(HAL_GetTick() <= currentTick + 1000);
    ```


2. 코드 다운로드 후 실행을 시켜보도록 한다. 그리고 터미널을 열어 확인하도록 한다


    ![11](https://drive.google.com/uc?id=1sAOl4VZsQQOAm7Wae8jydVl5sWFcbHix)


3. 라이브러리 함수를 따로 만들도록 한다


    ![12](https://drive.google.com/uc?id=19n2AEtv38tB_g7uw-x8SuuPIX_juV6Ug)


---
### 나머지 설정

UART Configuration이 완료되었다면, CRC와 나머지 Clock Enable을 하도록 한다. 이전에 이미 설정했다면 넘어가도록 한다


1. 클럭 설정을 하도록 한다 SYSCLK=60Mhz로 맞추도록 한다


    ![20](https://drive.google.com/uc?id=1HTBJpCRIOVhxJtPnyTdcR0ZuZahCOcyy)


2. 마지막으로 CRC 체크를 하도록 한다


    ![21](https://drive.google.com/uc?id=1BjLAdv3y9pKHyG6efhaUAnnkwcA16CwI)


3. 그리고 프로젝트를 생성해서 Init이 맞는지 확인하도로 한다


    ![22](https://drive.google.com/uc?id=1gspF4nFizA_UozP7b9H0IkNBzNsigxZP)
