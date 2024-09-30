기존의 간단한 socket 을 사용한 네트우크 통신은 blockSocket이다.
말 그대로 해당 라인이 끝나지않으면 영원히 코드 밑으로  동작 하지 않는다.
여러가지 상황에 빠르게 대처해야하는 서버의 특성상 이렇게 뻗어버리면 안되니까 non_blocking 방식의 Socket을 사용하였다. 

허나 그래서 문제가 모든게 그럼 사라젔는가?

아니다.

non blocking으로 소켓을 설정하게 되어 따라오는 문제가 여러가지 생겼다.

소켓통신 특성상 만약 connect accept bind listen send recv 등을 실행할떄 원래 블락에서는 에러 또는 예외 발생시 에러에 담아주고 리턴을 해주고 그것을 통해 사용자는 문제점에 대해 쉽게 접근 할 수 있었다.
그러나 non blocking을 하게 됨 으로써 일단은 패스시켜버리는 것이다. 
그로 인해 간단하게 제공되는 block소켓통신에 비해 예외처리 문과 반복문이 남용되는 상황이 발생된 것 이다. (그로 인해 성능이 block보다 더 내려감 ㅎ)

현재 이 프로젝트의 경우 딱 논블로킹으로 돌아가는 수준에서 그친 수준의 코드이다.

위에서 서술한 문제점을 수정한 버전은 다른 프로젝트에서 개선 할 것이다.

개선 프로젝트 url 
무작정 IOCP 를 사용하기 보다는 적재적소에 맞게 사용 할 수도 있으니 공부해두자.
또한 block,non_block, synchronous ,asynchronous , 멀티스레드 는 전부 다른 개념이다.
각 모델별로 위의 특성이 다르기 때문에 참고하자

동기식 모델이나 non-blocking
1. select 모델 사용 : https://github.com/ohgeehyun/Select_Model_Socket
2. WSAEvent모델 사용 : https://github.com/ohgeehyun/WSAEventSelect_model_socket

비동기식 모델 의 non-blocking
1.https://github.com/ohgeehyun/Overlapped_model-Event-
https://github.com/ohgeehyun/Overlapped_Model-CallBack-

ps.)   
non-block은 코드는 막힘없이 실행되지만 실제로는 메인코드는 실행되면서 함수는 당장 완료가 되지 않았을 뿐 실행되고있다. 
asynchronous의 경우는 "바로는 실행 안해도 된다." 라는 선택지가 주어진 것 이다.
그렇기에 block과synchronous는 엄연히 다르다.

또한 asynchronous또한 멀티스레드와는 엄연히 다르다.
asynchronous 는 하나의 스레드에서 실행시점및 완료 를 지금 당장이아닌 나중에로 미룬 것이고
멀티스레드는 하나의 스레드로 다른 하나의 일을 당장 수행하는것이다.

ps2.)
Reactor Pattern ~뒤늦게  (논블로킹 소켓  상태 확인 후  뒤늦게 recv send호출)

Proactor Pattern ~미리   (Overlapped WSA) recv send를 호출 이후 일처리

