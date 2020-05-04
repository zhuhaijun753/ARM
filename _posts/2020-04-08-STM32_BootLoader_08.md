---
layout: post
title:  "[Bootloader] Bootloader 커맨드 데이터 포맷 정의 및 버전 읽기"
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
### GET_VER 구현

그러면 GET_VER 명령이 입력되었을 때의 함수인 `bootloader_getver_cmd`을 구현하려고 한다




---
9장은 부트로더 호스트 개발하기
