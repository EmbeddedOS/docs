#include <iostream>

#include <grpcpp/grpcpp.h>

#include "sensor.grpc.pb.h"
#include "sensor.pb.h"

class SensorServiceImpl final : public Sensor::Service
{
    grpc::Status ReadSensor(grpc::ServerContext *context, const ReadRequest *request,
                            ReadReply *reply) override
    {
        reply->set_level(SENSOR_LEVEL_MEDIUM);
        reply->mutable_home_data()->set_temperature(10);
        return grpc::Status::OK;
    }
};

int main()
{
    SensorServiceImpl service{};
    grpc::ServerBuilder builder{};
    grpc::EnableDefaultHealthCheckService(true);

    builder.AddListeningPort("127.0.0.1:5902", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Listening..." << std::endl;
    server->Wait();
}