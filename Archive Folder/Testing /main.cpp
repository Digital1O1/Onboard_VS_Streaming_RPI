#include <libcamera/libcamera.h>
#include <libcamera/request.h>
#include <iostream>

int main()
{
    libcamera::CameraManager manager;
    manager.start();

    // const std::vector<libcamera::Camera *> cameras = manager.cameras();
    const std::vector<std::shared_ptr<libcamera::Camera>> cameras = manager.cameras();

    if (cameras.empty())
    {
        std::cerr << "No cameras found." << std::endl;
        return -1;
    }

    libcamera::Camera *camera = cameras[0];

    if (camera->acquire())
    {
        std::cerr << "Failed to acquire the camera." << std::endl;
        return -1;
    }

    const std::vector<libcamera::Stream *> streams = camera->streams();
    if (streams.empty())
    {
        std::cerr << "No streams available." << std::endl;
        camera->release();
        return -1;
    }

    libcamera::Stream *stream = streams[0];

    if (camera->allocateBuffers(stream))
    {
        std::cerr << "Failed to allocate buffers." << std::endl;
        camera->release();
        return -1;
    }

    libcamera::Request request;
    request.addBuffer(stream->buffers().front());

    if (camera->queueRequest(&request))
    {
        std::cerr << "Failed to queue request." << std::endl;
        camera->release();
        return -1;
    }

    libcamera::Event event;
    if (camera->waitForEvent(&event))
    {
        std::cerr << "Error waiting for event." << std::endl;
        camera->release();
        return -1;
    }

    if (event.type != libcamera::EventType::RequestComplete)
    {
        std::cerr << "Unexpected event type." << std::endl;
        camera->release();
        return -1;
    }

    libcamera::Request *completedRequest = event.request;

    const libcamera::FrameBuffer *frameBuffer = completedRequest->outputBuffer(stream);
    const uint8_t *rawData = static_cast<const uint8_t *>(frameBuffer->data());

    // Process or save the raw data as needed

    camera->release();
    return 0;
}
