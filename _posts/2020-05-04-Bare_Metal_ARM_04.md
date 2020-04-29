---
layout: post
title:  "[Bare Metal Embedded] Write Linker Script"
date:   2020-05-04
categories: ARM bare_metal
---

이 포스팅은 `ARM bare metal`과 관련한 포스팅임을 알려드립니다.

---
### Linker Script

링커 스크립트는 서로 다른 오브젝트 파일들이 하나의 파일로 합쳐질 때의 어떻게 영역을 할당할지에 대한 내용이 담긴 텍스트 파일이다. 즉 파일들은 결국 링커 스크립트에서 작성된 주소로 서로 다른 섹션들이 할당되게 된다. 그 섹션들은 메모리 영역에서 정의되는 것은 코드, 데이터가 된다. 링커 스크립트는 GNU 링커 명령어를 사용해서 쓰여지며, ld 파일 확장자를 가지게 된다

링커 명령어는 다음과 같다

* ENTRY
* MEMORY
* SECTIONS
* KEEP
* ALIGN
* AT>

---
### ENTRY

이 명령어는 Entry Point Address를 지정하는 데 사용된다. 현재 우리 코드에서는 전체적으로 봤을 때 `Reset_Handler`가 된다. __의무로 작성되어야 할 명령어는 아니지만, GDB 디버거를 위해선 반드시 첫 번째 진입 위치를 알려야하기 때문에 사용되어야 한다.__

Syntax는 ENTRY(__symbol_name_)처럼 사용한다. startup에서 작성한 심볼을 써주면 된다. 예를 들어 ENTRY(Reset_handler)처럼 작성하도록 한다


![01]()


---
### MEMORY

메모리 영역에서는 예를 들어 FLASH와 RAM 영역을 정의하게 된다. 즉, FLASH 코드 영역과 RAM 데이터 영역을 정의하게 된다. 전형적으로 링커 스트립트 파일에 메모리 섹션은 하나는 정의되어 있다

문법은 다음과 같다

MEMORY 
{
    name(attr):ORIGIN=originAddr,LENGTH=len
}

attr 속성 문자는 다음과 같다
* R : Read-Only
* W : Read and Write
* X : Execute
* A : Allocate
* I,L : Initialize
* ! : Invert attr

예를 들어, FLASH 1MB, SRAM 128KB 영역이 존재하는 메모리가 있다고 가정하자

