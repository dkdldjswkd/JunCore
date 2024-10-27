#include <boost/asio.hpp>
#include <iostream>
#include <functional>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <cstdint>
#include <cstring>

// 예제 함수
void exampleFunction(int a, int b, int c)
{
    std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;
}

int main()
{
    // std::bind를 사용하여 exampleFunction을 바인딩
    auto boundFunction = std::bind(exampleFunction, std::placeholders::_1, 2, std::placeholders::_2);

    // boundFunction을 호출하면서 인수를 전달
    boundFunction(10, 20); // 출력: a: 10, b: 20

    return 0;
}

//#include <boost/asio.hpp>
//#include <iostream>
//#include <functional>
//#include <vector>
//#include <queue>
//#include <memory>
//#include <thread>
//#include <mutex>
//#include <cstdint>
//#include <cstring>
//
//class ServerBase; // 전방 선언
//
//struct Socket : public boost::asio::ip::tcp::socket {
//    using boost::asio::ip::tcp::socket::socket;
//
//    std::queue<std::vector<char>> writeQueue;
//    std::mutex queueMutex;
//    bool sending = false;
//    std::vector<char> readBuffer; // 수신 데이터 버퍼
//    std::size_t totalReceived = 0; // 전체 수신된 바이트 수
//
//    void Send(const std::vector<char>& data) {
//        std::lock_guard<std::mutex> lock(queueMutex);
//        writeQueue.push(data);
//        StartSend();
//    }
//
//    void StartSend() {
//        if (sending || writeQueue.empty()) return;
//
//        sending = true;
//
//        boost::asio::async_write(*this, boost::asio::buffer(writeQueue.front()),
//            [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
//                std::lock_guard<std::mutex> lock(queueMutex);
//                if (!error) {
//                    writeQueue.front().erase(writeQueue.front().begin(), writeQueue.front().begin() + bytes_transferred);
//                    if (writeQueue.front().empty()) {
//                        writeQueue.pop();
//                    }
//                    if (!writeQueue.empty()) {
//                        StartSend();
//                    }
//                    else {
//                        sending = false;
//                    }
//                }
//                else {
//                    std::cout << "Error sending message: " << error.message() << std::endl;
//                }
//            });
//    }
//
//    void StartReceive() {
//        readBuffer.resize(104); // 최대 100바이트 payload + 4바이트 헤더
//        totalReceived = 0; // 수신 바이트 수 초기화
//        async_read_some(boost::asio::buffer(readBuffer),
//            [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
//                if (!error) {
//                    totalReceived += bytes_transferred;
//                    ProcessReceivedData();
//                    StartReceive(); // 다음 수신 대기
//                }
//                else {
//                    std::cout << "Error receiving message: " << error.message() << std::endl;
//                }
//            });
//    }
//
//    void ProcessReceivedData() {
//        std::size_t headerSize = 4; // 헤더 크기
//
//        while (totalReceived >= headerSize) {
//            uint32_t payloadSize = *reinterpret_cast<uint32_t*>(readBuffer.data());
//
//            if (totalReceived >= headerSize + payloadSize) {
//                // 충분한 데이터가 수신된 경우
//                std::vector<char> packet(readBuffer.begin() + headerSize, readBuffer.begin() + headerSize + payloadSize);
//                ServerBase::GetInstance()->OnRecv(packet); // 싱글턴으로 OnRecv 호출
//
//                // 처리된 데이터 제거
//                totalReceived -= (headerSize + payloadSize);
//                std::memmove(readBuffer.data(), readBuffer.data() + (headerSize + payloadSize), totalReceived); // 남은 데이터 앞으로 이동
//            }
//            else {
//                break; // 충분하지 않은 경우
//            }
//        }
//    }
//};
//
//class ServerBase {
//public:
//    static ServerBase* GetInstance() {
//        static ServerBase instance; // 싱글턴 인스턴스
//        return &instance;
//    }
//
//    void Start(boost::asio::io_context& io_context, int port) {
//        _p_io_context = &io_context;
//        _acceptor = new boost::asio::ip::tcp::acceptor(*_p_io_context,
//            boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
//        Accept();
//    }
//
//    void Accept() {
//        auto socket = std::make_shared<Socket>(*_p_io_context);
//        _acceptor->async_accept(*socket,
//            [this, socket](boost::system::error_code error_code) {
//                if (!error_code) {
//                    on_accept(socket);
//                    socket->StartReceive(); // 수신 시작
//                }
//                else {
//                    std::cout << "Error: " << error_code.message() << std::endl;
//                }
//                Accept();
//            });
//    }
//
//    virtual void on_accept(std::shared_ptr<Socket> socket) = 0; // 추상 메서드
//    virtual void OnRecv(const std::vector<char>& packet) = 0; // 패킷 핸들링
//
//protected:
//    ServerBase() = default; // 기본 생성자는 보호
//    boost::asio::io_context* _p_io_context;
//    boost::asio::ip::tcp::acceptor* _acceptor;
//};
//
//struct TestServer : public ServerBase {
//    void on_accept(std::shared_ptr<Socket> socket) override {
//        std::cout << "Accepted a connection." << std::endl;
//    }
//
//    void OnRecv(const std::vector<char>& packet) override {
//        std::cout << "Received payload of size: " << packet.size() << std::endl;
//        // 여기서 패킷에 대한 적절한 핸들링을 구현
//    }
//};
//
//// 사용 예
//int main() {
//    boost::asio::io_context io_context;
//    TestServer server;
//    server.Start(io_context, 12345); // 서버 시작
//    io_context.run();
//    return 0;
//}
