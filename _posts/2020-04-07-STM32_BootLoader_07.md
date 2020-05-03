---
layout: post
title:  "[Bootloader] Bootloader에서 App 코드로 점프하기"
date:   2020-04-07
categories: ST Bootloader
---

이 포스팅은 `ST 보드의 Bootloader와 관련한` 포스팅임을 알려드립니다.

---

이전까지 부트로더 코드를 작성하기 위한 환경구성을 했다. 실제 부트로더의 용도는 펌웨어를 플래시에 쓰기 위한 도구라고 보면 된다. 즉, Flash와 사용자 코드롤 Jump할지를 부팅 후에 결정지어야만 한다. User Code로 Jump 하기 위한 대략적인 개요는 포인터를 App의 Reset Handler(Flash Sector2)로 옮겨서 실행시키는 것이다. 결정짓는 기준으로는 User 버튼이 눌렸는지로 결정하려고 한다

먼저 429ZI-DISC1의 User Button 회로도를 보려고 한다. 기본 풀다운 회로로 되어 있다. 따라서 기본 상태는 0이 입력될 것이고, 누르면 1이 뜰 것이다


![01](https://drive.google.com/uc?id=1NRZT3GBZ6UXLUkhu0ErhPNMs3umPAMHQ)


그리고 코드는 다음과 같이 작성할 수 있다. 만일 GPIO_PIN_SET이라면 BootLoader 모드로 들어가게 된다. 아니라면 Jump_to_app이 실행되게 된다


![02](https://drive.google.com/uc?id=1-vwMNQZZ2NWmJ8vyLtvxCpMjMXQ7yaIL)


그리고 실제 프로토 함수를 작성하면 다음과 같다. 반드시 main.h에 미리 선언도 추가해야 한다. 그래야 빌드 오류가 생기지 않는다


![03](https://drive.google.com/uc?id=14gVGhB-I2z6NYnYeB0VGhP0Ox8ljuUM0)


---
### USER_APP 작성

새 프로젝트를 생성해서 USER_APP을 작성하려고 한다. USER_APP은 간단하게 버튼 인터럽트를 통해서 보드에 내장되어 있는 LED를 Toggle 시키는 것을 하려고 한다

1. 새 프로젝트를 Board_Selector로 생성하도록 한다. 그리고 기본 SYS_CLK는 60MHz로 맞추도록 한다


    ![04](https://drive.google.com/uc?id=1rb4cjx2ksqpdhKtIPdZW2g4augGZqScT)


2. 그리고 USART1으로 기본 Debug Console로 사용하도록 한다


    ![05](https://drive.google.com/uc?id=1UId6p7Bu968RjhUsGmdva7NNXNXCWrBs)


3. 버튼 인터럽트를 설정하려고 한다. 여기서는 인터럽트에 대해서는 자세하게 다루지 않으려고 한다. CubeMX에서 GPIO 설정에 대해서 Interrupt 설정을 하면 NVIC 탭이 생성된다


    ![06](https://drive.google.com/uc?id=10Q5vK49oGS6-5BSpF-OjuckjYGeinx7E)


    ![07](https://drive.google.com/uc?id=1nlMZ5RFeP2CGfKQHzPVAyQpkNWiZsXoe)


4. 그리고 KEIL로 프로젝트를 생성하도록 한다. 그리고 빌드를 한 후 `stm32f4xx_it.c`를 찾아본 후 `EXTI0_IRQHandler` 함수를 찾아보도록 한다. 그리고 다시 `HAL_GPIO_EXTI_IRQHandler`로 들어가보도록 한다. 그리고 마지막으로 `HAL_GPIO_EXTI_Callback` 함수를 검색하면 여기에 LED 코드를 작성하면 된다. weak 옵션 함수이니, main에서 재정의하면 된다


    ![08](https://drive.google.com/uc?id=1iNkOZxs3XqEg8R9Ht0CyJ6KXAz3XPPE7)


    다음과 같이 RED_LED를 Toggle 시키는 코드를 작성하도록 한다


    ![09](https://drive.google.com/uc?id=1sdVwZcRwiwxGXO9DcXKNeor0qW8EyLy0)


---
### USER_APP Flash 주소 변경

1. 이전에 USER_APP은 FLASH 섹터2부터 작성을 하려고 했다. RM 문서에 따르면 섹터2의 시작주소는 0x08008000이다 따라서 KEIL IDE에서 Target 세팅에서 ROM 시작주소를 변경하도록 할 것이다


    ![10](https://drive.google.com/uc?id=1T5pKjKzrlPBCrr5tAwg4YXfQlVLQVexR)


    ![11](https://drive.google.com/uc?id=1vN2Alb6HTO4gjkXeVKcQZ84GGDrpomXH)


    ![12](https://drive.google.com/uc?id=14WGMDjrgS_Ntby_CkmIm7kz7zoo0osSB)



2. 그리고 Chip Erase을 위해서 위 [링크](https://www.st.com/en/development-tools/stsw-link004.html)에서 ST-LINK Utility를 다운로드 받도록 한다. 그리고 Target 탭에서 CHIP-ERASE를 통해 FULL-CHIP-ERASE를 실행하도록 한다


    ![13](https://drive.google.com/uc?id=1O1FyFT7o60OUaJbpB8Gye09AjOHzu1gH)


    ERASE를 수행한 이후 연결을 끊고, 위에서 작성한 소스코드를 다운하도록 한다


    ![14](https://drive.google.com/uc?id=1lIm0iV9mFj9kNTMg3z6p6bAMQVY2oakp)


    그리고 ST-LINK Utility를 연결해서 0x08000000번지를 검색하도록 한다. 정상적으로 다운이 되었다면 여전히 0xFFFFFFFF로 초기화되어야 한다. __만일 되어있지 않다면 소스코드를 재빌드한 후 다운로드 하도록 한다.__


    ![15](https://drive.google.com/uc?id=18rjZ2zqEv6lmGKoD-jlRFU4Qu6eQmAix)


    그리고 실제 다운로드 한 0x08008000 메모리 섹터를 검색하도록 한다. USER_APP 코드가 정상적으로 다운로드 되었다는 것을 확인할 수 있다


    ![16](https://drive.google.com/uc?id=1ijx8FXOhT1cn6IJgySQqejjZHDtUuzJz)


3. 그리고 BootLoader 코드를 다운로드 하도록 한다. 그리고 ST-LINK Utility를 열어서, 0x08000000을 확인하도록 한다


    ![17](https://drive.google.com/uc?id=1D0JejJA9Y3vdK-ipqQgk_oFPTuNNTfHW)


---
### VTOR

VTOR은 Vector Table Offset Register로 이미 ST의 칩의 VTOR은 플래시 메모리의 주소인 0x08000000임을 알 수 있었다. 그런데 현재 다음 그림을 보면 프로젝트 2개가 하나의 FLASH에 내장되어 있고, 다음과 같이 표현할 수 있다


![18](https://drive.google.com/uc?id=14UGlvdW4inSE_vX4wQ40sZNimgN83Z3q)


그래서 만일 Bootloader에서 App으로 뛰는 순간 VTOR을 바꿔줘야 한다. 따라서 VTOR은 플래시 메모리의 섹터 2의 시작주소가 되어야만 한다. __그리고 반드시 Startup 코드에 VTOR을 설정하는 부분이 있다. Default로는 0x00으로 되어있는데, 0x8000으로 수정하도록 한다. 먼저 Startup 코드에서 SystemInit 함수를 찾고, 거기서 VTOR을 설정하는 부분을 검색해야만 한다__ 


![19](https://drive.google.com/uc?id=1bJy1p2_B0Kzwwza2Gh0uHgFK3caNKPdF)


![20](https://drive.google.com/uc?id=1K80w3RH_umMzSsAwQGzGFfhDuUB1DKYi)

---
### Jump to User App

그러면 사전에 작성했던 `jump_to_user_app` 함수에 대해서 작성해보려고 한다. 플래시 메모리 섹터 2의 MSP와 리셋 핸들러 주소를 지정해주는 것을 볼 수 있다. __set_MSP 함수 안에 들여다보면 결국에는 `msp` 어셈블러로 설정하는 것을 볼 수 있다


```cpp
#define USER_APP_FLASH_BASE_ADDR	0x8008000

void jump_to_user_app(void) {
	void (*app_reset_handler)(void);
	
	uint32_t msp_value = *(volatile uint32_t *)(USER_APP_FLASH_BASE_ADDR);
	
	__set_MSP(msp_value);
	
	uint32_t reset_handler_addr = *(volatile uint32_t*)(USER_APP_FLASH_BASE_ADDR + 4);
	app_reset_handler = (void *)reset_handler_addr;
	
	app_reset_handler();
}
```


그리고 APP 코드에서 인터럽트를 통한 LED 토글을 실행하면 다음과 같이 실행을 시킬 수 있다. LED 토글 시 flag라는 변수로 write 하는 것이 올바른 방법이다.


![21](https://drive.google.com/uc?id=1RmiLqD6xeOSW04vkWXxWjyiSoHPARSUZ)