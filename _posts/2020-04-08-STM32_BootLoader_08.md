---
layout: post
title:  "[Bootloader] 8. Bootloader 커맨드 데이터 포맷 정의 및 버전 읽기"
date:   2020-04-08
categories: ST Bootloader
---

이 포스팅은 `ST 보드의 Bootloader와 관련한` 포스팅임을 알려드립니다.

---

이제부터 여러 부트로더 커맨드를 정의할 것이다. 커맨드는 대부분 이 [링크](https://www.st.com/resource/en/application_note/cd00264342-usart-protocol-used-in-the-stm32-bootloader-stmicroelectronics.pdf)를 참조할 것이다

여러 부트로더 명령어 셋들이 제시되어 있다


![01](https://drive.google.com/uc?id=1uEXM_kQ4foZxzvJu1DxJ-lIAuLBYDXoV)


예를 들어 부트로더 버전을 가져오는 것에 대한 커맨드 포맷을 정해보려고 한다. 다음을 참고해서 재정의해보려고 한다


![02](https://drive.google.com/uc?id=1ZPMWlU92bKZPAviBSAynLC65KXUz-8db)


![03](https://drive.google.com/uc?id=1jDw2AMUQaDmflH1NxNRFtpIgft-ZlxVN)


나머지 명령은 구현할 때 자세히 서술하려고 한다

---
### 부트로더 명령 읽기 코드 틀 잡기

1. 이제 부트로더 명령을 읽는 코드의 틀을 잡으려고 한다. 이전까지 App_Jump를 구현했고 이제는 반대로 부트로더 프로그램을 구현해보려고 한다. 기존 부르토더 프로젝트를 열도록 한다. 함수 이름은 `read_bootloader_cmd`로 정해서 작성할 것이다


    ![04](https://drive.google.com/uc?id=1eza-a4BUiT9GI8LbfVdLzwOPMRPcdVUg)


2. 현재 함수명이 정의가 되어있지 않는다는 경고가 나오니 프로토타입을 main.h에 적도록 한다


    ![05](https://drive.google.com/uc?id=10RI2I1owvWAEmc09MECGwnjECwEz0wru)


3. 그리고 `read_bootloader_cmd`을 정의하도록 한다. 먼저 len을 입력받고, 그 len만큼 명령을 읽어드리도록 한다. 그래서 마지막으로 switch-case 문으로 명령에 따라서 처리를 구분하도록 한다. __cmd_rx_buffer와 MAX_CMD_LEN은 global 변수로 정의했다. 참고로 MAX_CMD_LEN은 256으로 정의했다.__


    ![06](https://drive.google.com/uc?id=1f39_kP-DqNqAXaMMNQ3npZtD6NyJiQ3b)


4. 그리고 switch 문을 간략하게 작성해보려고 한다


    ```cpp
    switch(cmd_rx_buffer[1]) {
        case GET_VER:
            break;
        case GET_HELP:
            break;
        case GET_ID:
            break;
        default:
            print_debug_msg("Invalild Command\r\n");
            break;
    }
    ```

    `main.h`에 명령어 정의를 하도록 한다


    ```cpp
    /* define main.h */
    #define GET_VER 0x01
    #define GET_HELP 0x02
    #define GET_ID 0x03
    ```

5. 따라서 최종 코드 구현은 다음과 같다

    
    ![07](https://drive.google.com/uc?id=11tGOu0EaawwtjWF06Uz7G8vVbPmudSzc)


---
### GET_VER Flow Chart 구현

그러면 GET_VER 명령이 입력되었을 때의 함수인 `bootloader_getver_cmd`을 구현하려고 한다. 먼저 `bootloader_getver_cmd`는 메세지 길이 이후에 나오는 커맨드가 일치했을 때 호출하게 된다

그러면 함수에 들어갔을 때 바로 해야되는 것은 나머지 바이트에 포함되어 있는 CRC 체크를 해야만 한다. CRC가 맞지 않는다면 NACK을 보낸 후 함수를 종료하고, 그렇다면 ACK를 전달한 후 부트로더 버전을 담아서 보내주면 된다

Flow Chart에 대해서는 PlantUML 이미지 추가하도록 한다

정리하자면 공통적으로 모든 명령에서 사용할 것은 다음과 같다

1. CRC 체크 함수
2. ACK, NACK 보내는 함수
3. 명령에 대한 답장을 보내는 함수

---
### ACK, NACK 함수 구현

상황에 따라 호출하는 ACK, NACK을 호출하는 함수를 호출하려고 한다. ACK을 보낼 때는 ACK 후에 HOST가 받을 길이 정보도 추가적으로 담아서 보낼 것이다. 그리고 NACK은 NACK만 보내도록 할 것이다. 다음 장에서 HOST 개발을 할 것인데, 이때 ACK, NACK을 서로 약속해야만 한다. __따라서 ACK은 0xA5, NACK은 0x7F로 약속할 것이다.__

1. ACK, NACK 정의

    ```cpp
    #define ACK     0xA5
    #define NACK    0x7F
    ```

2. send_ack(길이)

    ```cpp
    void send_ack(int reply_len)
    {
        uint8_t ack_buf[2];
        buf[0] = ACK;
        buf[1] = reply_len;
        HAL_UART_Transmit(&huart1, ack_buf, 2, HAL_MAX_DELAY);
    }
    ```

3. send_nack()

    ```cpp
    void send_nack(void)
    {
        uint8_t nack;
        nack = NACK;
        HAL_UART_Transmit(&huart1, &nack, 1, HAL_MAX_DELAY);
    }
    ```

---
### CRC 검사 함수 구현

CRC Peripheral에 대해서 간략히 알아보고자 한다. CRC는 Cyclic Redundancy Check 데이터의 오류를 검출하는 기술이다. 오류는 감지하지만 수정은 하지 않는다는 것이 특징이다. 구현은 쉽지만 데이터 무결성에 있어서는 강력하다. STM에서 사용하는 CRC에 대한 내용은 다음 [링크](https://www.st.com/resource/en/application_note/dm00068118-using-the-crc-peripheral-in-the-stm32-family-stmicroelectronics.pdf)를 참조해서 쉽게 파악할 수 있다

몇가지 그림을 통해서 STM에서의 CRC Peripheral 내부 동작을 알 수 있다

1. Block Diagram


    ![08](https://drive.google.com/uc?id=1-9Fr7OViAlNwrdt4Bx7j7I6B0PLCLi62)


2. CRC 알고리즘 순서도 및 실제 계산법


    ![09](https://drive.google.com/uc?id=13TczAkw8wXjA6AcDQgzCNdgVeXFkm7HM)


    ![10](https://drive.google.com/uc?id=1Y0_XAjU26fseHXL1dnW0JNWmnE6tPHtm)


3. STM 시리즈 안의 실제 CRC 초기값과 Poly 정의값


    ![11](https://drive.google.com/uc?id=1kV7nom4I3KM5Qr-ovpGDblluuwLRaCzI) 


4. 따라서 현재 부트로더에서 검사하는 CRC32는 HOST에서 전달된 CRC 값이다. 어떻게 구현이 되어 있는지 한 번 확인해보도록 하자. HOST는 단순 윈도우 프로그램이기 때문에, STM CRC 라이브러리를 사용할 수 없기 때문에, 직접 위 알고리즘대로 구현을 해야만 한다


    ```python
    def get_crc(buff, length):
    Crc = 0xFFFFFFFF
    for data in buff[0:length]:
        Crc = Crc ^ data
        for i in range(32):
            if(Crc & 0x80000000):
                Crc = (Crc << 1) ^ 0x04C11DB7
            else:
                Crc = (Crc << 1)
    return Crc
    ```
