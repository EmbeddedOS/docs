#include <iostream>

#include <grpcpp/grpcpp.h>

#include "sensor.grpc.pb.h"
#include "sensor.pb.h"

class SensorClient
{
    std::unique_ptr<Sensor::Stub> stub;

public:
    SensorClient(std::shared_ptr<grpc::Channel> channel) : stub{Sensor::NewStub(channel)} {}

    void GetSensorData()
    {
        grpc::ClientContext client_context{};
        ReadRequest req{};
        ReadReply reply{};
        grpc::Status status{};

        req.set_sensor_type(3);

        status = stub->ReadSensor(&client_context, req, &reply);
        if (status.ok())
        {
            std::cout << "Sensor data, level: " << reply.level() << " , temp: " << reply.home_data().temperature() <<  std::endl;;
        }
        else
        {
            std::cerr << "Failed to read sensor, err: " << status.error_code() << ": " << status.error_message()
                      << std::endl;
        }
    }

};

int
main()
{
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel("127.0.0.1:5902", grpc::InsecureChannelCredentials());
    SensorClient client{channel};
    client.GetSensorData();
}