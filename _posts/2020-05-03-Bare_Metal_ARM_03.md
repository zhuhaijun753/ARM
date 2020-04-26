---
layout: post
title:  "[Bare Metal Embedded] Write Startup File"
date:   2020-05-03
categories: ARM bare_metal
---

이 포스팅은 `ARM bare metal`과 관련한 포스팅임을 알려드립니다.

---

startup file에 대해서 작성하는 법을 배우려고 한다. startup 파일은 임베디드 프로젝트에서 매우 중요한 역할을 한다

1. startup 파일은 궁극적으로 main.c를 호출하게 된다
2. startup 파일은 코드가 올바르게 동작하도록 하는 환경설정에 대한 내용을 담고 있다
3. startup 파일을 프로세서 의존적이며, 벡터 테이블과 스택 영역을 할당하게 된다. 그리고 .bss, .data 섹션 초기화도 책임지게 된다

---

따라서 startup 파일은 작성하는 순서는 다음과 같다

1. MCU에 맞게 벡터 테이블을 작성하도록 한다
2. SRAM에 .data와 .bss 섹션을 복사하게 된다
3. 그리고 메인 함수를 호출하게 된다

---
### vector table

벡터 테이블의 시작은 0x0000_0004에서 시작된다. 실제로 내부 Flash 메모리에 저장되며, STM32 MCU의 Flash 주소는 0x0800_0000부터 시작한다. 따라서 0x0800_0000인 메인 스택 포인터를 시작으로 0x0800_0004부터 벡터 테이블의 시작인 리셋 핸들러가 위치하게 된다

![01]()

stack은 맨 끝에서 아래로 내려오는 형태다


