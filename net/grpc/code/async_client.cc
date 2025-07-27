#include <stdlib.h>
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "sensor.grpc.pb.h"
#include "sensor.pb.h"

class SensorClient
{
    std::unique_ptr<Sensor::Stub> _stub;
    grpc::CompletionQueue _cq;

    class Call
    {
    public:
        ReadReply reply;
        ReadRequest request;
        grpc::ClientContext context;
        grpc::Status status;
        std::unique_ptr<grpc::ClientAsyncResponseReader<ReadReply>> response_reader;
    };

public:
    SensorClient(std::shared_ptr<grpc::Channel> channel) : _stub{Sensor::NewStub(channel)} {}

    void GetData()
    {
        Call *rpc = new Call;

        /* Make response reader, prepare for request and start the call. */
        rpc->response_reader =
            this->_stub->AsyncReadSensor(&rpc->context, rpc->request, &this->_cq);

        sleep(5);
        std::cout << "Finished\n";
        /* Actually request to receive server response. */
        rpc->response_reader->Finish(&rpc->reply, &rpc->status, rpc);
    }

    void SensorDataHandler()
    {
        void *tag;
        bool ok = false;
        Call *rpc = nullptr;

        while (this->_cq.Next(&tag, &ok))
        {
            rpc = static_cast<Call *>(tag);

            if (!ok || !rpc)
            {
                std::cout << "Failed to read the queue!" << std::endl;
                continue;
            }

            std::cout << "Received new response" << std::endl;

            if (rpc->status.ok())
            {
                std::cout << "Sensor data, level: " << rpc->reply.level()
                          << " , temp: " << rpc->reply.home_data().temperature()
                          << std::endl;
            }
            else
            {
                std::cout << "Failed to read the queue, err: "
                          << rpc->status.error_message() << std::endl;
            }

            delete rpc;
        }
    }
};

int main()
{
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel("127.0.0.1:5902", grpc::InsecureChannelCredentials());

    SensorClient client{channel};

    // Can run sender in one thread and response handler in other.
    client.GetData();
    std::cout << "Sent get data request!" << std::endl;

    client.GetData();
    std::cout << "Sent get data request!" << std::endl;


    client.SensorDataHandler();
}
