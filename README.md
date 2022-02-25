# SmartECC

```bash
src/
├── fault/ 
│   └── Packet.hh Packet.cc
│
├── mem_global/
│  └── Faultmap.hh Faultmap.cc :결함셀 정보 저장. SRAM 구현부
│
├── mem_local/ 
│  ├── hsiao.hh hsiao.cc :Syndrome/Candidator Generator 통합본. (72,64) Hsiao Code(1960) 기반.
│  ├── CNN.hh CNN.cc :비트별 에러확률 계산 딥러닝 구현부
│  └── MLD.hh MLD.cc :Ranking 구현부

├── utils/  
│  ├── ...  :기타 파라미터 
│
└── Tester.hh Tester.cc :테스트 코드
└── main.cc
``` 

## How to run
```bash
./test.out -input "input data" -error_length "# of error" -error_pos "position of error"

ex) When 0x01 was encoded, 2 and 5 bit of codeword is inverted due to failure.
./test.out -input 1 -error_length 2 -error_pos 2 5;
``` 


## (2022-02-22) New Update -- CNN
<p align="center">
<img width=480 src=https://user-images.githubusercontent.com/48650641/155073647-97f533cd-8fed-4e51-aed8-4f51cc7a7f65.gif>
</p>

  - mem_global/의 **Faultmap** 으로 부터 셀별 Weakness 정보를 받아 온다.
  - Data Dependent Failure + Retention Failure의 패턴을 학습

## (2022-02-22) New Update -- FaultMap
<p align="center">
<img width="795" alt="faultmap" src="https://user-images.githubusercontent.com/48650641/155074681-b44b933b-1b6d-4d10-98b5-4502ae01de3c.png">
</p>

  - FLOWER/Fame(HPCA2020) 기반 FaultMap 구현
  - **Notice**) Wide-IO SRAM이 요구됨. 개선 사항 (연구 진행중)
  
## Documentation
For detail, please refer to (업데이트중) [SmartECC Documentation](https://quark-lupin-c50.notion.site/SmartECC-Architecture-9310b61452514e469ecf84551de7adca )
