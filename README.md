# oo-cryprosystem
Cryptographic System - Object Oriented Analysis &amp; Design 

For more detailed description is in the file description.pdf

**시나리오** <br> 
- 사용자는 데이터를 암호화하여 안전하게 보호하고 싶다. (기밀성) <br>

**요구사항** <br>
- 사용자는 키를 발급받는다.
- 사용자는 암호 모듈을 선택한다.
- 입력한 평문을 발급받은 키와 선택한 암호 모듈을 이용하여 암호화한다.
- 키를 일정 횟수 이상 이용하면 키를 재발급한다.

**GRASP Pattern** <br>
- Low Coupling
- High Cohesion
- Don't talk with strangers

**Design Patterns** <br>
- Singleton pattern
- Factory pattern
